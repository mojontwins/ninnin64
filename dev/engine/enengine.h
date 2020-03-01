// MTE MK1_NES / AGNES 0.1 R
// Copyleft Mojon Twins 2013, 2019

// enengine.h
// Enemies Engine & stuff

#ifdef PERSISTENT_ENEMIES
// Copy to RAM addresses and initialize statuses (alive = 1)
void persistent_enems_load (void) {
	#ifdef COMPACTED_ENEMS
		ep_it = 0;
		for (gpit = 0; gpit < l_map_size [level]; ++ gpit) {
			gp_gen = (unsigned char *) c_enems [gpit]; rdm = (gp_gen != 0);
			for (gpjt = 0; gpjt < 3; gpjt ++) {
				if (rdm) if (*gp_gen == 0) rdm = 0;
				if (rdm == 0) ep_t [ep_it] = 0; else
	#else
		gp_gen = (unsigned char *) (c_enems);
		for (ep_it = 0; ep_it < 3 * MAP_W * MAP_H; ++ ep_it) 
	#endif
	{
		// Loads enem #ep_it

		__asm__ ("ldy #0");
		__asm__ ("lda (%v), y", gp_gen);
		__asm__ ("sta %v", rda);
		__asm__ ("iny");
		__asm__ ("lda (%v), y", gp_gen);
		__asm__ ("sta %v", rdb);
		__asm__ ("iny");
		__asm__ ("lda (%v), y", gp_gen);
		__asm__ ("sta %v", rdc);
		__asm__ ("iny");
		__asm__ ("lda (%v), y", gp_gen);
		__asm__ ("sta %v", rdd);

		__asm__ ("ldx %v", ep_it);

		__asm__ ("lda %v", rda);
		__asm__ ("sta %v, x", ep_t);

		__asm__ ("lda %v", rdb);
		__asm__ ("asl a");
		__asm__ ("asl a");
		__asm__ ("asl a");
		__asm__ ("asl a");
		__asm__ ("sta %v, x", ep_x);

		__asm__ ("lda %v", rdb);
		__asm__ ("and #$f0");
		__asm__ ("sta %v, x", ep_y);

		__asm__ ("lda %v", rdc);
		__asm__ ("asl a");
		__asm__ ("asl a");
		__asm__ ("asl a");
		__asm__ ("asl a");
		__asm__ ("sta %v", rdx);

		__asm__ ("lda %v", rdc);
		__asm__ ("and #$f0");
		__asm__ ("sta %v", rdy);

		rdd &= 0xf;
		ep_mx [ep_it] = ADD_SIGN2 (rdx, ep_x [ep_it], rdd);
		ep_my [ep_it] = ADD_SIGN2 (rdy, ep_y [ep_it], rdd);

		gp_gen += 4;
	}

	#ifdef COMPACTED_ENEMS
				++ ep_it;
			}
		}
	#endif
}

void persistent_update (void) {
		__asm__ ("lda %v", on_pant);
		__asm__ ("cmp #99");
		__asm__ ("bne %g", persistent_update_do);
		__asm__ ("rts");

	persistent_update_do:
		__asm__ ("asl a");
		__asm__ ("clc");
		__asm__ ("adc %v", on_pant);
		__asm__ ("tay");

		__asm__ ("ldx #0");
	persistent_update_loop:
		__asm__ ("lda %v, x", en_x);
		__asm__ ("sta %v, y", ep_x);

		__asm__ ("lda %v, x", en_y);
		__asm__ ("sta %v, y", ep_y);

		__asm__ ("lda %v, x", en_t);
		__asm__ ("sta %v, y", ep_t);

		// en_status [gpit] == 0 -> shift left 1
		__asm__ ("lda %v, x", en_status);
		__asm__ ("bne %g", pul_noshift);

		__asm__ ("lda %v, x", en_mx);
		__asm__ ("asl 1");
		__asm__ ("sta %v, y", ep_mx);

		__asm__ ("lda %v, x", en_my);
		__asm__ ("asl 1");
		__asm__ ("sta %v, y", ep_my);
		__asm__ ("jmp %g", persistent_update_continue);

	pul_noshift:
		__asm__ ("lda %v, x", en_mx);
		__asm__ ("sta %v, y", ep_mx);

		__asm__ ("lda %v, x", en_my);
		__asm__ ("sta %v, y", ep_my);

	persistent_update_continue:
		__asm__ ("iny");
		__asm__ ("inx");
		__asm__ ("cpx #3");
		__asm__ ("bne %g", persistent_update_loop);
}
#endif

void enems_arrays_restore (void) {
	__asm__ ("ldy %v", gpit);

	__asm__ ("lda %v", _en_t);
	__asm__ ("sta %v, y", en_t);
	__asm__ ("lda %v", _en_s);
	__asm__ ("sta %v, y", en_s);

	__asm__ ("lda %v", _en_ct);
	__asm__ ("sta %v, y", en_ct);
	__asm__ ("lda %v", _en_maxct);
	__asm__ ("sta %v, y", en_maxct);

	__asm__ ("lda %v", _en_touched);
	__asm__ ("sta %v, y", en_touched);
	__asm__ ("lda %v", _en_cttouched);
	__asm__ ("sta %v, y", en_cttouched);

	__asm__ ("lda %v", _en_x);
	__asm__ ("sta %v, y", en_x);
	__asm__ ("lda %v", _en_y);
	__asm__ ("sta %v, y", en_y);

	__asm__ ("lda %v", _en_mx);
	__asm__ ("sta %v, y", en_mx);
	__asm__ ("lda %v", _en_my);
	__asm__ ("sta %v, y", en_my);
}

void enems_load (void) {
	// Loads enems from n_pant
	
	#ifdef PERSISTENT_ENEMIES
		ep_it = (n_pant << 1) + n_pant;
	#endif
	
	#ifdef COMPACTED_ENEMS
		gp_gen = (unsigned char *) c_enems [n_pant]; rdm = (gp_gen != 0);
	#else
		gp_gen = (unsigned char *) (c_enems) + (n_pant << 3) + (n_pant << 2);
	#endif

	for (gpit = 0; gpit < 3; ++ gpit) {
		#ifdef COMPACTED_ENEMS
			if (rdm) if (*gp_gen == 0) rdm = 0;
			if (rdm == 0) {
				en_t [gpit] = 0;
			} else
		#endif
		{
				__asm__ ("ldy #0");
				__asm__ ("lda (%v), y", gp_gen);
				__asm__ ("sta %v", rda);
				__asm__ ("iny");
				__asm__ ("lda (%v), y", gp_gen);
				__asm__ ("sta %v", rdb);
				__asm__ ("iny");
				__asm__ ("lda (%v), y", gp_gen);
				__asm__ ("sta %v", rdc);
				__asm__ ("iny");
				__asm__ ("lda (%v), y", gp_gen);
				__asm__ ("sta %v", rdd);

			gp_gen += 4;

				__asm__ ("ldx %v", gpit);

				__asm__ ("lda %v", rdb);
				__asm__ ("asl a");
				__asm__ ("asl a");
				__asm__ ("asl a");
				__asm__ ("asl a");
				__asm__ ("sta %v, x", en_x1);

				__asm__ ("lda %v", rdb);
				__asm__ ("and #$f0");
				__asm__ ("sta %v, x", en_y1);

				__asm__ ("lda %v", rdc);
				__asm__ ("asl a");
				__asm__ ("asl a");
				__asm__ ("asl a");
				__asm__ ("asl a");
				__asm__ ("sta %v, x", en_x2);

				__asm__ ("lda %v", rdc);
				__asm__ ("and #$f0");
				__asm__ ("sta %v, x", en_y2);

			#ifdef PERSISTENT_ENEMIES
					__asm__ ("ldx %v", ep_it);

					__asm__ ("lda %v, x", ep_mx);
					__asm__ ("sta %v", _en_mx);
					__asm__ ("lda %v, x", ep_my);
					__asm__ ("sta %v", _en_my);
					__asm__ ("lda %v, x", ep_t);
					__asm__ ("sta %v", _en_t);
					__asm__ ("lda %v, x", ep_x);
					__asm__ ("sta %v", _en_x);
					__asm__ ("lda %v, x", ep_y);
					__asm__ ("sta %v", _en_y);
				
					__asm__ ("inc %v", ep_it);
			#else			
				_en_x = en_x1 [gpit];
				_en_y = en_y1 [gpit];

				_en_maxct = rdd & 0xf0;
				rdt = rdd & 0xf;
				_en_mx = ADD_SIGN2 (en_x2 [gpit], en_x1 [gpit], rdt);
				_en_my = ADD_SIGN2 (en_y2 [gpit], en_y1 [gpit], rdt);
				
				_en_t = rda;				
			#endif

			#ifdef ENEMS_CAN_DIE
				en_life [gpit] = ENEMS_LIFE_GAUGE;
			#endif
			en_status [gpit] = 0;
			
			// Sanitized type
			rdt = _en_t & 0xf;

			switch (rdt) {

			#ifdef ENABLE_PURSUERS		
				case 7:
					#ifdef PLAYER_MOGGY_STYLE
						_en_s = (PURSUERS_CELL - 1) << 1;
						en_alive [gpit] = 0;
						en_ct [gpit] = DEATH_COUNT_EXPRESSION;
					#else
						_en_mx = rdd & 0xf;	// Raw speed
					#endif
					break;
			#endif

			#ifdef ENABLE_8WAY_FANTIES
				case 6:
					_en_mx = rdd & 0xf;
					_en_s = (FANTIES_CELL - 1) << 1;
					#ifdef FANTIES_LIFE
						en_life [gpit] = FANTIES_LIFE;
					#endif
					break;
			#endif

				default:
				/*
				_en_s = (rdt - 1) << 1;
				if (_en_mx == 1 || _en_my == 1) { en_status [gpit] = 1; } 
				else { en_status [gpit] = 0; _en_mx >>= 1; _en_my >>= 1; }
				*/
					__asm__ ("ldx %v", gpit);

					__asm__ ("lda %v", rdt);
					__asm__ ("sec");
					__asm__ ("sbc #1");
					__asm__ ("asl a");
					__asm__ ("sta %v", _en_s);

					__asm__ ("lda %v", _en_mx);
					__asm__ ("cmp #1");
					__asm__ ("beq %g", enems_load_en_status_do);

					__asm__ ("lda %v", _en_my);
					__asm__ ("cmp #1");
					__asm__ ("bne %g", enems_load_en_status_else);

				enems_load_en_status_do:
					__asm__ ("lda #1");
					__asm__ ("sta %v, x", en_status);
					__asm__ ("jmp %g", en_status_done);

				enems_load_en_status_else:
					__asm__ ("lda #0");
					__asm__ ("sta %v, x", en_status);
					__asm__ ("lda %v", _en_mx);
					
					//__asm__ ("lsr a");
					// Needs to be ASR a
					__asm__ ("cmp #$80");
					__asm__ ("ror a");
					
					__asm__ ("sta %v", _en_mx);
					__asm__ ("lda %v", _en_my);
					
					//__asm__ ("lsr a");
					// Needs to be ASR a
					__asm__ ("cmp #$80");
					__asm__ ("ror a");

					__asm__ ("sta %v", _en_my);

				en_status_done:
					;

				#ifdef ENABLE_ARROWS
					if (rdt == 8) _en_s = ARROWS_BASE_SPRITE + (_en_mx < 0);
				#endif

				#ifdef ENABLE_DROPS
					if (rdt == 9) _en_s = DROPS_BASE_SPRITE;
				#endif
			}
			
			_en_touched = _en_ct = 0;

			enems_arrays_restore ();
		}
	}
}

void enem_hit (void) {
		__asm__ ("lda #1");
		__asm__ ("sta %v", _en_touched);
		__asm__ ("sta %v", touched);
		__asm__ ("lda #8");
		__asm__ ("sta %v", _en_cttouched);

		__asm__ ("ldx %v", gpit);
		__asm__ ("dec %v, x", en_life);
		__asm__ ("bne %g", enem_hit_not_killed_yet);

		__asm__ ("lda #0");
		__asm__ ("sta %v", _en_t);
		__asm__ ("inc %v", pkilled);

		#ifdef KILLED_X
			P_T (KILLED_X, KILLED_Y, pkilled);
		#endif

		#ifdef FIRE_ON_KILL
			#ifdef ACTIVATE_SCRIPTING
				run_fire_script ();
			#endif
			#ifdef ACTIVATE_HOOKS
				press_fire_at_screen ();
			#endif
		#else
			#ifdef ACTIVATE_HOOKS
				on_enem_killed ();
			#endif
		#endif		

	enem_hit_not_killed_yet:
		sfx_play (SFX_ENHIT, 2);
}

void enems_move (void) {
	#ifndef PLAYER_MOGGY_STYLE	
		pgotten = 0;
		pgtmx = 0;
		pgtmy = 0;
	#endif
	
	// Updates sprites
	touched = 0;
	for (gpit = 0; gpit < 3; ++ gpit) {
		en_sprid = 0xff;

		// Make local copies
		__asm__ ("ldy %v", gpit);

		__asm__ ("lda %v, y", en_t);
		__asm__ ("sta %v", _en_t);
		__asm__ ("lda %v, y", en_s);
		__asm__ ("sta %v", _en_s);

		__asm__ ("lda %v, y", en_ct);
		__asm__ ("sta %v", _en_ct);
		__asm__ ("lda %v, y", en_maxct);
		__asm__ ("sta %v", _en_maxct);
		
		__asm__ ("lda %v, y", en_touched);
		__asm__ ("sta %v", _en_touched);
		__asm__ ("lda %v, y", en_cttouched);
		__asm__ ("sta %v", _en_cttouched);
		
		__asm__ ("lda %v, y", en_x);
		__asm__ ("sta %v", _en_x);
		__asm__ ("lda %v, y", en_y);
		__asm__ ("sta %v", _en_y);

		__asm__ ("lda %v, y", en_mx);
		__asm__ ("sta %v", _en_mx);
		__asm__ ("lda %v, y", en_my);
		__asm__ ("sta %v", _en_my);
		
		#ifdef ENEMS_CAN_DIE
			if (_en_touched) {				
				if (_en_cttouched) {
					-- _en_cttouched;
					en_sprid = EXPL_SPR;
					goto enems_move_loop_continue;	
				} else _en_touched = 0;
			}
		#endif
		
		if (_en_t) {
			
			// Gotten preliminary:
			gpjt = (prx + 11 >= _en_x && prx <= _en_x + 11);
		
			if (_en_mx != 0) {
				en_fr = ((_en_x) >> 4) & 1;
			} else if (_en_my != 0) {
				en_fr = ((_en_y) >> 4) & 1;
			} else {
				en_fr = (frame_counter >> 3) & 1;
			}

			// Major type
			switch (_en_t & 0xf) {
				case 1:
				case 2:
				case 3:
				case 4:
					#include "engine/enemmods/enems_lineal.h"
					break;

				#ifdef ENABLE_8WAY_FANTIES
					case 6:
						#include "engine/enemmods/enems_8way_fanties_assembly.h"
						break;
				#endif

				#ifdef ENABLE_PURSUERS
					case 7:
						#ifdef PLAYER_MOGGY_STYLE									
							#include "engine/enemmods/enems_pursuers_topdown.h"
						#else
							#include "engine/enemmods/enems_pursuers_sideview.h"								
						#endif
						break;
				#endif

				#if defined (ENABLE_ARROWS) || defined (ENABLE_DROPS)
					#ifdef ENABLE_ARROWS
						case 8:
					#endif
					#ifdef ENABLE_DROPS
						case 9:
					#endif
						#include "engine/enemmods/enems_droparrows.h"
						break;
				#endif
			}

			// Subtype
			#ifdef ENABLE_ENEM_SUBTYPES
				if (_en_ct) -- _en_ct; else	{
					_x = _en_x + 4; _y = _en_y + 4; _t = 0xff;
					switch (_en_t & 0xf0) {
						#ifdef ENABLE_HORZ_SIGHTSHOOTERS
							case 0x10:
								#include "engine/enemmods/enems_horz_sightshooters.h"
								break;
						#endif
						#ifdef ENABLE_VERT_SIGHTSHOOTERS
							case 0x20:
								#include "engine/enemmods/enems_vert_sightshooters.h"
								break;
						#endif
						#ifdef ENABLE_TIMED_SHOOTERS
							case 0x30:	// Up     _t = 0
							case 0x40:	// Right  _t = 2
							case 0x50:	// Down   _t = 4
							case 0x60:	// Left   _t = 6
								_t = ((_en_t >> 4) - 3) << 1;
								break;
						#endif
						#ifdef ENABLE_GYROSHOOTER
							case 0x70:
								__asm__ ("ldx %v", gpit);
								__asm__ ("inc %v, x", en_cur_d);
								__asm__ ("lda %v, x", en_cur_d);
								__asm__ ("and #7");
								__asm__ ("sta %v, x", en_cur_d);
								__asm__ ("sta %v", _t);
								break;
						#endif
					}
					if (_t != 0xff) {
						cocos_add ();
						_en_ct = _en_maxct ? _en_maxct : ENEM_COCOS_FREQ;
					}
				}
			#endif

			#ifndef PLAYER_MOGGY_STYLE
				// Moving platforms
				#include "engine/enemmods/enems_platforms.h"
			#endif

			#ifdef PLAYER_STEPS_ON_ENEMS
				if (
					_en_t != 4
					#ifdef STEP_MIN_KILLABLE
						&& _en_t >= STEP_MIN_KILLABLE
					#endif			
				) {
					// Step over enemy
					IF_COLLIDE (prx, pry, _en_x, _en_y - 4) {
						if (pry + 2 < _en_y && pvy > 0 && !pgotten && !ppossee) {
							pvy = -PLAYER_VY_JUMP_INITIAL;
							enem_hit ();
							
							goto enems_move_loop_continue;
						}
					}
				}
			#endif

			#ifdef ENABLE_HITTER
				if (
					_en_t != 4
					#if defined (ENABLE_ARROWS) && !defined (KILLABLE_ARROW)
						&& _en_t != 8
					#endif
					#if defined (ENABLE_DROPS) && !defined (KILLABLE_DROPS)
						&& _en_t != 9
					#endif
				) {
					if (hitter_on && hitter_hit_y) {
						IF_COLLIDE_IN (hitter_hit_x, hitter_hit_y, _en_x, _en_y) {
							enem_hit ();
							goto enems_move_loop_continue;
						}
					}
				}
			#endif

			// Collide <-> player
			if (!touched) {
				IF_COLLIDE (prx, pry, _en_x, _en_y) {
					#if defined (ENABLE_PURSUERS) && defined (PLAYER_MOGGY_STYLE)
						if (_en_t != 7 || en_alive [gpit] == 2) 
					#endif
					#ifndef PLAYER_MOGGY_STYLE
						if (_en_t != 4)
					#endif				
					{
						touched = phit = 1;
						en_life [gpit] = 1; enem_hit ();
					}
				}
			}
		} else {
			en_sprid = 0xff;
		}

enems_move_loop_continue:
		if (en_sprid == 0xff) oam_2x2_meta_spr (0, 240, gpit << 4, 0, 252);
		else oam_2x2_meta_spr (_en_x, _en_y + SPRITE_ADJUST, gpit << 4, spriteset_pals [en_sprid], en_sprid << 2);
		
		enems_arrays_restore ();
	}	
}
