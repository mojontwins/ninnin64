// MTE MK1_NES / AGNES 0.1 R
// Copyleft Mojon Twins 2013, 2019

// Hooks for custom stuff (replace scripting)

const unsigned char bits [] = {
	1, 2, 4, 8, 16, 32, 64, 128
};

#define PLAYER_TOUCHES(x,y) (rdx==(x) && rdy==(y))

void entering_game (void) {
	pcoins = 0; 
}

void entering_level (void) {
	// Clear tile persistence
	vram_adr (MAP_CLEAR_LIST_ADDRESS);
	vram_fill (0, 1024);
	memfill (screen_clear_list, 0, 24);	

	// Custom decos
	deco_flags = 
		(c_tsmap == tsmap2 || c_tsmap == tsmap8) ? 
			DECO_STALACTITES : 
			0;

	// Reset hud extra var
	opcoins = 0xff;

	// Generally
	f1 = 0;

	// Lava
	lava_height = (level == 6 ? 240 : 255);
	lava_height_pixels = 240 << 3;
	lava_delay = (level == 6 ? 30 : 40);

	if (level == 9) lava_launch ();
}

void flick_screen (void) {
	// Right after flick screen DETECTION.
	rdb = (level == 9) ? (n_pant >> 1) : n_pant;
	scrpixoffset = (rdb << 7) + (rdb << 6);
}

void exiting_screen (void) {
	// This is run when screen is OFF!

	// Save tile persistence
	if (on_pant != 99) {
		rda = on_pant << 3;
		vram_write (screen_clear_list, MAP_CLEAR_LIST_ADDRESS + (rda << 1) + rda, 24);
	}

	// Reset ray tiles counter (filled in custom_renderer.h)
	// And face tiles counter (same)
	ray_tiles = face_tiles = face_count = 0;

	rda = 0;
	if (level == 3 || level == 7) {
		// Levels 3 & 7 have normal & underwater sections

		if (level == 3) {
			if (n_pant == 7) {
				rdi = 1;
				if (on_pant == 15) { rda = 1; pvy = -64;} 
			} else {
				rdi = 0;
				if (n_pant == 15 && on_pant == 7) { rda = 1; pvy = 48; }
			}
		} else  {
			if (n_pant < 2) {
				rdi = 1;
				if (on_pant == n_pant + 4) { rda = 1; pvy = -64; }
			} else {
				rdi = 0;
				if (on_pant < 2) { rda = 1; pvy = 48; }
			}
		}
		if (rda) sfx_play (SFX_FLOAT, 0);

		if (rdi) {
			// Surface
			c_ppu_mask = 0x1e;
			vertical_engine_type = ENGINE_TYPE_JUMP;
			pal_bg (mypal_game_bg0);
			c_alt_bg = 34;
		} else {
			// Underwater
			c_ppu_mask = 0x9e;
			vertical_engine_type = ENGINE_TYPE_SWIM;
			pal_bg (mypal_game_bg3);
			c_alt_bg = 42;
		}
	} else if (level == 11) {
		// Level 7 has indoors & outdoors screens
		c_alt_bg = ((n_pant & 1) == 0 && n_pant < 16) ? 34 : 0;
	} else if (level == 8) {
		// Stalactites in lower level
		deco_flags = n_pant < 10 ? 0 : DECO_STALACTITES;
	}
}

void entering_screen (void) {
	// This is run when screen is OFF!
	// Load tile persistence
	rda = n_pant << 3;
	vram_read (screen_clear_list, MAP_CLEAR_LIST_ADDRESS + (rda << 1) + rda, 24);

	// Modify screen	
	rdd = _x = 0; _y = TOP_ADJUST;

		__asm__ ("ldx #0");
		__asm__ ("stx %v", rdm);

	es_modify_screen_loop:
		__asm__ ("txa");
		__asm__ ("and #7");
		__asm__ ("tay");
		
		__asm__ ("ldx %v", rdd);
		__asm__ ("lda %v, x", screen_clear_list);
		__asm__ ("and %v, y", bits);
		__asm__ ("beq %g", es_modify_screen_skip);

		__asm__ ("ldx %v", rdm);
		__asm__ ("ldy #0");
		__asm__ ("lda (%v), y", c_tsmap);
		__asm__ ("sta %v", _t);
		__asm__ ("sta %v, x", map_buff);
		__asm__ ("tay");
		__asm__ ("lda %v, y", behs);
		__asm__ ("sta %v, x", map_attr);
		draw_tile ();

	es_modify_screen_skip:
		__asm__ ("lda %v", rdm);
		__asm__ ("and #7");
		__asm__ ("cmp #7");
		__asm__ ("bne %g", es_inc_index_skip);

		__asm__ ("inc %v", rdd);

	es_inc_index_skip:
		__asm__ ("lda %v", _x);
		__asm__ ("clc");
		__asm__ ("adc #2");
		__asm__ ("and #31");
		__asm__ ("sta %v", _x);
		__asm__ ("bne %g", es_inc_y_skip);
		__asm__ ("lda %v", _y);
		__asm__ ("clc");
		__asm__ ("adc #2");
		__asm__ ("sta %v", _y);

	es_inc_y_skip:
		__asm__ ("inc %v", rdm);
		__asm__ ("ldx %v", rdm);
		__asm__ ("cpx #192");
		__asm__ ("bne %g", es_modify_screen_loop);

	// Per level

	switch (level) {
		case 11:	
			switch (n_pant) {
				case 1:
				case 3:
					pscreenissafe = 0;
					break;
				case 5:
				case 7:
					// Set spring traps and a hidden path:
					attr_list_mod (alm0);
					break;
				case 13:
					// Turn off platform (enemy 0)
					en_t [0] = 0;
					break;
				case 15:
					map_attr [COORDS (2,3)] = 48;	 	// Set spring trap!
					break;
				case 17:
					fzx1 = 80; fzx2 = fzy2 = 95; fzy1 = 16; f_zone_ac = 1;
					break;
				case 23:
					// Set spring traps and a hidden path:
					attr_list_mod (alm1);
					break;
			}
			break;
		case 6:
			if (n_pant == 0 && f1) {
				_x = 7; _y = 5; _t = 0; draw_tile ();
				_x = 8; _y = 5; _t = 0; draw_tile ();
			}

			// Correct! No break here:
		case 9:

			// Lava precalculations & pre-render			
			lava_calc ();
			if (lava_on_screen) lava_draw_off ();

			break;
	}

	// Generally
	f0 = 0;
}

void press_fire_at_screen (void) {
	// Player_touches
	rdx = (prx + 8) >> 4;
	rdy = (pry + 8) >> 4;
	
	if (firezoned) {
		// Fire zones

		if (level == 11) {
			switch (n_pant) {
				case 17:
					sfx_play (SFX_BREAKH, 0);
					close_update_cycle (); update_index = 0; 
					for (gpit = 6; gpit < 11; ++ gpit) {						
						_y = 6; _x = gpit; _t = 0; map_set ();
					}
					f_zone_ac = 0;
					return;
			}
		}
	} else {
		// Exceptions

		switch (level) {
			case 11:
				switch (n_pant) {
					case 18:
						if (PLAYER_TOUCHES (8, 2) && f0 == 0) {
							sfx_play (SFX_BREAKH, 0);
							close_update_cycle (); update_index = 0;
							for (gpit = 4; gpit < 7; ++ gpit) {
								_y = gpit; _x = 9; _t = 29; map_set ();
							}
							f0 = 1;
							return;
						}
					case 13:
						if (PLAYER_TOUCHES (3, 10) && f0 == 0) {
							sfx_play (SFX_START, 0);
							en_t [0] = 4;
							f0 = 1;
							break;
						}
				}
				break;
			case 6:
				if (n_pant == 9) {
					if (PLAYER_TOUCHES (3, 9) && f1 == 0) {
						lava_launch ();
					}
				}
				break;
		}
	}

	// Tile below to rdt:
		__asm__ ("lda %v", prx);
		__asm__ ("clc");
		__asm__ ("adc #8");
		__asm__ ("lsr a");
		__asm__ ("lsr a");
		__asm__ ("lsr a");
		__asm__ ("lsr a");
		__asm__ ("sta %v", _x);

		__asm__ ("lda %v", pry);
		__asm__ ("clc");
		__asm__ ("adc #16");
		__asm__ ("sta %v", cy1);

		__asm__ ("and #$f0");
		__asm__ ("ora %v", _x);
		__asm__ ("sta %v", rdi);
		__asm__ ("tax");
		__asm__ ("lda %v, x", map_buff);
		__asm__ ("sta %v", rdt);

		__asm__ ("lda %v", cy1);
		__asm__ ("lsr a");
		__asm__ ("lsr a");
		__asm__ ("lsr a");
		__asm__ ("lsr a");
		__asm__ ("sta %v", _y);

	// Code to activate ray tile and open fences
	if (rdt == TILE_RAY) {
		pad_this_frame = 0; 	// so Ninjajar doesn't punch!

		_t = 46; map_set ();
		sfx_play (SFX_OBJECT, 0);
		-- ray_tiles;
		if (ray_tiles == 0) {
			sfx_play (SFX_FANFARE, 0);
			if (level == 11 && n_pant == 20) {
				_t = 16; _y = 5; _x = 9; map_set ();
				_t = 16; _y = 2; _x = 6; map_set ();
			} else {
				rdn = TILE_FENCE; clear_tile_rdn ();
			}
		}
	}

	// Code to activate face tiles and count chain tiles
	if (rdt == TILE_FACE) {
		pad_this_frame = 0; 	// so Ninjajar doesn't punch!

		// How many chain tiles?
		// Beware! This algorithm doesn't check for boundaries!
			rda = 0;
			__asm__ ("ldx %v", rdi);
		count_tiles_loop:
			__asm__ ("txa");
			__asm__ ("sec");
			__asm__ ("sbc #16");
			__asm__ ("tax");
			__asm__ ("lda %v, x", map_buff);
			__asm__ ("cmp #%b", TILE_CHAIN);
			__asm__ ("bne %g", count_tiles_done);
			__asm__ ("inc %v", rda);
			__asm__ ("jmp %g", count_tiles_loop);
		count_tiles_done:

		// Count?
		if (rda == 0) {
			face_count = 0;
			face_cleared = 1;
			rdi = SFX_OBJECT;
		} else if (rda == face_count + 1) {
			face_count = rda;
			face_cleared ++;
			if (face_cleared == face_tiles) {
				rdi = SFX_FANFARE;
				rdn = TILE_SPIKES; clear_tile_rdn ();
			} else rdi = SFX_OBJECT;
		} else {
			face_count = face_cleared = 0;
			rdi = SFX_ENHIT;			
		}
		sfx_play (rdi, 0);
	}
}

void custom_hud (void) {
	if (pcoins != opcoins) {
		P_T (16, 26, pcoins);
		opcoins = pcoins;
	}
}

#ifdef ENABLE_BREAKABLE
	void on_tile_break (void) {
		// Tile is @ (_x, _y). You may substitute _t
		rdt = map_buff [COORDS(_x,_y)];

		// Star box may spawn a coin
		if (rdt == 14 && (rand8 () & 7) < 6) _t = 47;

		// Clear forever
		screen_clear_list [(_y << 1) | (_x > 7)] |= bits [_x & 7];
	}
#endif

#ifdef ENABLE_TILE_GET
	void on_tile_get (void) {
		// Tile is @ (_x, _y), precalc @ rdm.
		// You may substitute _t

		// No need to check for type as only coins can be got in this game
		++ pcoins;

		// Earn the player an extra life
		if (pcoins == (kids_mode ? 25 : 50)) {
			sfx_play (SFX_FANFARE, 0);
			pcoins = 0;
			++ plife;
		}
	}
#endif

#ifndef DEACTIVATE_OBJECTS
	void on_obj_got (void) {
		// Get the onigiri to finish current level

		win_game = (pobjs == 1);
	}
#endif

#ifdef ENEMS_CAN_DIE
	#ifndef FIRE_ON_KILL
		void on_enem_killed (void) {	
			if (level == 11 && n_pant < 4) {
				// Auto open door when all fanties killed
				++ f0; 
				if (f0 == 3) {
					sfx_play (SFX_FANFARE, 0);
					_x = 7; _y = 11; _t = 0; map_set ();
					_x = 8; _y = 11; _t = 0; map_set ();
				}
			}
		}
	#endif
#endif
