// MTE MK1_NES / AGNES 0.1 R
// Copyleft Mojon Twins 2013, 2019

// Uses neslib and related tools by Shiru

#define DEBUG

#include "neslib.h"

// **************
// * const data *
// **************

#include "definitions.h"
#include "config.h"
#include "assets/palettes.h"
#include "my/palettes_extra.h"
#include "assets/metasprites.h"
#include "assets/chr_rom_maps.h"
#include "assets/enems0.h"
#include "assets/enems1.h"
#include "assets/enems2.h"
#include "assets/enems3.h"
#include "assets/enems4.h"
#include "assets/enems5.h"
#include "assets/enems6.h"
#include "assets/enems7.h"
#include "assets/enems8.h"
#include "assets/enems9.h"
#include "assets/enemsA.h"
#include "assets/enemsB.h"
#include "assets/screens.h"
#ifdef MAPPED_TILESETS
	#include "my/tsmap.h"
#endif
#ifdef ENEMS_USE_MAPPED_SPRITESET
	#include "my/enems_ssmap.h"
#endif
#include "my/levelset.h"
#include "assets/hud_rle.h"

#ifdef ACTIVATE_SCRIPTING
extern const unsigned char *e_scripts [];
extern const unsigned char *f_scripts [];
#endif

// Push to zero page:
#pragma bssseg (push,"ZEROPAGE")
#pragma dataseg(push,"ZEROPAGE")

#include "ram/ram_zp.h"

// Everything else on normal RAM
#pragma bssseg (push,"BSS")
#pragma dataseg(push,"BSS")

#include "ram/ram_bss.h"
#include "my/custom_vars.h"

// *************
// Main includes
// *************

#include "engine/printer.h"
#include "engine/general.h"
#ifdef ACTIVATE_HOOKS
	#include "my/extra_functions.h"
	#include "my/hooks.h"
#endif
#ifndef DEACTIVATE_KEYS
	#include "engine/bolts.h"
#endif
#ifdef ENABLE_BREAKABLE
	#include "engine/breakable.h"
#endif
#include "engine/hotspots.h"
#ifdef ENABLE_HITTER
	#include "engine/hitter.h"
#endif
#ifdef ENABLE_COCOS
	#include "engine/silly_cocos.h"
#endif
#include "engine/player.h"
#include "engine/enengine.h"
#include "my/pres.h"

// **************
// Main functions
// **************

void prepare_scr (void) {
	set_rand (n_pant + 1);

	fade_delay = 1;
	if (!ft) fade_out (); else ft = 0;

	#ifdef PERSISTENT_ENEMIES
		// Preserve enems
		persistent_update ();
	#endif

	enems_load ();

	#ifdef ENABLE_FIRE_ZONE
		f_zone_ac = 0;
	#endif

	// Disable sprites and tiles so we can write to VRAM.
	ppu_off ();
	#ifdef ACTIVATE_HOOKS
		exiting_screen ();
	#endif

	draw_scr ();
	hotspots_create ();
	pscreenissafe = 1;

	#ifdef ACTIVATE_HOOKS
		entering_screen ();
	#endif
	
	// Write attributes
	vram_write (attr_table, 0x23c0, 48);

	// Clear line of text
	#ifdef LINE_OF_TEXT
		PR_STR (LINE_OF_TEXT_X, LINE_OF_TEXT, "                              ");
	#endif

	enems_move ();
	player_move ();

	if (psafe) player_set_safe ();

	#ifdef PLAYER_CAN_FIRE		
		memfill (bst, 0, MAX_BULLETS);
		bullets_move ();
	#endif

	#ifdef ENABLE_HITTER
		hitter_on = 0;
	#endif

	// Reenable sprites and tiles now we are finished.
	ppu_on_all ();

		__asm__ ("lda %v", c_ppu_mask);
		__asm__ ("and %v", ppu_mask_xor_bits_fiddle);
		__asm__ ("beq %g", init_ppu_mask_nope);
		__asm__ ("lda %v", c_ppu_mask);
		__asm__ ("eor %v", ppu_mask_xor_bits_fiddle);
		__asm__ ("jmp %g", init_ppu_mask_set);
	init_ppu_mask_nope:
		__asm__ ("lda %v", c_ppu_mask);
	init_ppu_mask_set:
		__asm__ ("sta $2001");

	#ifdef ACTIVATE_SCRIPTING
		// Entering any script
		script = (unsigned char *) e_scripts [MAP_W * MAP_H + 1];
		run_script ();
		// This room script
		script = (unsigned char *) e_scripts [n_pant];
		run_script ();
	#endif

	#ifdef ENABLE_COCOS
		cocos_init ();
		cocos_do ();
	#endif

	#ifdef PLAYER_HAS_SWIM
		if (vertical_engine_type == ENGINE_TYPE_SWIM)
			player_set_safe ();
	#endif

	fade_in ();
}

void game_loop (void) {
	n_pant = l_scr_ini [level];
	on_pant = 99;

	hotspots_load ();

	#ifndef DEACTIVATE_KEYS		
		bolts_load ();
	#endif		

	player_init ();

	#ifdef PERSISTENT_ENEMIES
		persistent_enems_load ();
	#endif		
	
	#ifdef ACTIVATE_SCRIPTING
		msc_init_all ();
	#endif

	#ifdef ACTIVATE_HOOKS
		memfill (flags, 0, MAX_FLAGS);
		entering_level ();
	#endif

	half_life = win_game = game_over = 0;
	olife = oammo = oobjs = okeys = 0x99;
	pneedsreset = 0;

	music_play (l_music [level]);
	set_vram_update (update_list);
	update_list [0] = 0xff;
	ppu_on_all ();

	#ifdef ACTIVATE_SCRIPTING
		script_result = 0;
		// Entering game script
		script = (unsigned char *) e_scripts [MAP_W * MAP_H];
		run_script ();
	#endif

	ft = 1;	while (1) {
		half_life ^= 1;
		++ frame_counter;
		update_index = 0;
		pad_read ();

		if (paused == 0) {
			phit = 0;

			#if defined (ACTIVATE_SCRIPTING) || defined (ACTIVATE_HOOKS)
				#ifdef ENABLE_FIRE_ZONE
					rdx = prx + 8; rdy = pry + 8;
					firezoned = (f_zone_ac && (rdx >= fzx1 && rdx <= fzx2 && rdy >= fzy1 && rdy <= fzy2));
					if ((pad_this_frame & PAD_B) || firezoned) 
				#else
					if (pad_this_frame & PAD_B) 
				#endif
				{
					#ifdef ACTIVATE_SCRIPTING
						run_fire_script ();
					#else
						press_fire_at_screen ();
					#endif
				}
			#endif

			player_move ();

			#ifdef ENABLE_HITTER
				if (hitter_on) hitter_do ();
				else oam_spr (0, 240, 0, 0, 252);
			#endif

			enems_move ();
			cocos_do ();

			#ifdef PLAYER_CAN_FIRE
				bullets_move ();
			#endif
		}

		#ifdef BREAKABLE_ANIM
			if (do_process_breakable) breakable_do_anim ();
		#endif

		// Hotspot interaction
		if (hrt) {
			IF_COLLIDE_IN (prx + 8, pry + 8, hrx, hry) {
				_x = hrx >> 4; _y = hry >> 4; _t = map_buff [(hry & 0xf0) | _x]; map_set ();					
				rda = 0; switch (hrt) {
					#ifndef DEACTIVATE_OBJECTS
						case 1:
							++ pobjs;
							#ifdef ACTIVATE_HOOKS
								on_obj_got ();
							#endif
							#ifdef OBJECTS_X
								P_T (OBJECTS_X, OBJECTS_Y, pobjs);
							#endif								
							rda = SFX_OBJECT;
							break;
					#endif
					#ifndef DEACTIVATE_KEYS
						case 2:
							++ pkeys;
							rda = SFX_USE;
							break;
					#endif
					case 3:
						plife += PLAYER_REFILL;
						#ifndef PLAYER_DONT_LIMIT_LIFE
							if (plife > PLAYER_LIFE) plife = PLAYER_LIFE;
						#endif
						rda = SFX_USE;
						break;
					#if defined (PLAYER_CAN_FIRE) && defined (MAX_AMMO)
						case 4:
							if (MAX_AMMO - pammo > AMMO_REFILL)
								pammo += AMMO_REFILL;
							else
								pammo = MAX_AMMO;
							break;
							rda = SFX_USE;
					#endif
				}
				
				hry = 240;
				hact [n_pant] = 0;
				if (rda) sfx_play (rda, 1);
			}
		}

		// Pause?
		if (pad_this_frame & PAD_START) {
			paused ^= 1;
			music_pause (paused);
			pal_bright (4 - paused);				
		}

		// Change screen
		// Very basic. Extend when needed.
		if (prx == 0 && pvx < 0) {
			n_pant --;
			prx = 240; px = prx << FIXBITS;
		} else if (prx == 240 && pvx > 0) {
			n_pant ++;
			prx = 0; px = prx << FIXBITS;
		} 
		if (pry > 192 && pry <= 248 && pvy < 0 && n_pant >= c_map_w) {
			n_pant -= c_map_w;
			pry = 176; py = pry << FIXBITS;
		} else if (pry >= 176 && pry < 192 && pvy > 0) {
			n_pant += c_map_w;
			pry = py = 0;
		}

		#ifdef ACTIVATE_HOOKS
			if (on_pant != n_pant) flick_screen ();
		#endif
		
		// Update frame
		
		#ifdef KEYS_X
			if (okeys != pkeys) {
				okeys = pkeys;
				P_T (KEYS_X, KEYS_Y, pkeys);
			}
		#endif
		#ifdef LIFE_X
			if (olife != plife) {
				olife = plife;
				P_T (LIFE_X, LIFE_Y, plife);
			}
		#endif
		#ifdef AMMO_X
			if (oammo != pammo) {
				oammo = pammo;
				P_T (AMMO_X, AMMO_Y, pammo);
			}
		#endif
		#ifdef ACTIVATE_HOOKS
			custom_hud ();
		#endif

		#include "my/extra_routines.h"

		// Sync
		close_update_cycle ();
		if (pstate != EST_FLICKERING) if (phit) player_kill ();

		// Change screen
		if (on_pant != n_pant) {
			prepare_scr ();
			on_pant = n_pant;
		}

		// Conditions
		
		#ifdef ACTIVATE_SCRIPTING
			win_game = script_result;
		#endif

		#ifdef PLAYER_MAX_OBJECTS
			wingame = (pobjs == PLAYER_MAX_OBJECTS);
		#endif

		if ((pad & (PAD_UP|PAD_B|PAD_SELECT)) == (PAD_UP|PAD_B|PAD_SELECT)) win_game = 1;

		if (game_over || win_game || pneedsreset) break;
	}
	music_stop (); 
	set_vram_update (0);
	fade_delay = 4;
	fade_out ();
	oam_clear ();
	ppu_off ();
}

void main(void) {
	scroll (0, 240-8);

	ppu_mask_xor_bits_fiddle = ppu_system () ? 0x60 : 0;

	while (1) {
		title ();
		kids_mode = (pad_this_frame & PAD_SELECT);

		level = 0;
		plife = kids_mode ? 10 : 5;
		
		#ifdef ACTIVATE_HOOKS
			entering_game ();
		#endif
		
		while (1) {
			rdn = 0; fixed_screen ();
			#include "my/frame.h"
			#include "engine/level_select.h"
			game_loop ();

			if (game_over) {
				rdn = 1; fixed_screen ();
				if (!(kids_mode && (pad_this_frame & PAD_START))) break;
				else plife = kids_mode ? 10 : 5;
			} else if (!pneedsreset) {
				++ level;
				if (level == MAX_LEVELS) {
					rdn = 2; fixed_screen ();
					break;
				}
			}
		}
	}
}

#ifdef ACTIVATE_SCRIPTING
#include "assets/mscnes.h"
#endif
