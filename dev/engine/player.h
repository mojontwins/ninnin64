// MTE MK1_NES / AGNES 0.1 R
// Copyleft Mojon Twins 2013, 2019

// player.h
// Player movement & stuff

#ifdef PLAYER_MOGGY_STYLE
	const unsigned char player_frames [] = {0, 1, 2, 3, 4, 5, 6, 7};
#else
	const unsigned char player_frames [] = {
		4, 5, 6, 5,
		0, 1, 2, 1,
		7, 3
	};
	const signed char _pgtmx [] = { -PLAYER_VX_CONVEYOR, PLAYER_VX_CONVEYOR };
#endif

void player_set_safe (void) {
	px_safe = px;
	py_safe = py;
	n_pant_safe = n_pant;
}

void player_init (void) {
	// Init player data
	
	px = (signed int) (l_ini_x [level] << 4) << FIXBITS;
	py = (signed int) (l_ini_y [level] << 4) << FIXBITS;
	player_set_safe ();

	#ifdef PLAYER_MOGGY_STYLE	
		pfacing = FACING_DOWN;
	#else
		pfacing = l_pfacing [level];
		#if defined (PLAYER_HAS_JUMP)
			vertical_engine_type = ENGINE_TYPE_JUMP;
		#elif defined (PLAYER_HAS_JETPAC)
			vertical_engine_type = ENGINE_TYPE_JETPAC;
		#elif defined (PLAYER_HAS_SWIM)
			vertical_engine_type = ENGINE_TYPE_SWIM;
		#endif
	#endif	

	pvx = pvy = pfr = pctfr = pj = pctj = pobjs = pkeys = pkilled = 0;
	
	#ifdef ENABLE_HITTER
		hitter_on = 0;
	#endif

	#if defined (MAX_AMMO) && defined (PLAYER_CAN_FIRE)
		#ifdef INITIAL_AMMO
			pammo = INITIAL_AMMO
		#else
			pammo = MAX_AMMO;
		#endif
	#endif	

	#ifdef PLAYER_CAN_FIRE
		pfiring = 0;			
		memfill (bst, 0, MAX_BULLETS);
	#endif

	pstate = EST_NORMAL;
}

void player_kill (void) {
	if (plife) --plife; else game_over = 1;
	
	#ifdef PLAYER_FLICKERS
		pstate = EST_FLICKERING;
		pctstate = 100;	
	#endif

	sfx_play (SFX_PHIT, 0);
	
	#ifdef DIE_AND_RESPAWN
		music_pause (1);
		delay (60);
		music_pause (0);

		pvx = pvy = pj = 0;

		if (!game_over) {
			px = px_safe;
			py = py_safe;
			n_pant = n_pant_safe;
		}
	#endif	
}

#if defined(PLAYER_PUSH_BOXES) || !defined(DEACTIVATE_KEYS)
	#pragma warn (off)
	void process_tile (unsigned char qt, unsigned char x0, unsigned char y0
		#ifdef PLAYER_PUSH_BOXES
		, unsigned char x1, unsigned char y1
		#endif
	) {
		_x = x0; _y = y0;

		#ifdef PLAYER_PUSH_BOXES
			#ifdef FIRE_TO_PUSH
				if (pad & PAD_B) {
					pfiring = 1;
			#endif
				if (_x > 0 && _x < 15 && _y > 0 && _y < 11) {
					if (qt == PUSHABLE_TILE && attr (x1, y1) == 0) {
						#ifdef FIRE_TO_PUSH
							pushed_any = 1;
						#endif
						_t = 0; map_set ();
						_x = x1; _y = y1; _t = PUSHABLE_TILE; map_set ();
						sfx_play (SFX_TILE, 1);
					}
				}
			#ifdef FIRE_TO_PUSH
				}
			#endif	
		#endif
		#ifndef DEACTIVATE_KEYS
			if (qt == LOCK_TILE && pkeys) {
				_t = 0; map_set (); // rdd == YX
				bolt_clear ();
				-- pkeys;
				sfx_play (SFX_TILE, 1);
			}
		#endif
	}
	#pragma warn (on)
#endif

#ifdef PLAYER_CAN_FIRE
	#include "engine/playermods/player_bullets.h"
#endif

void player_move (void) {
	if (pstate != EST_NORMAL) {
		-- pctstate;
		if (!pctstate) pstate = EST_NORMAL;
	}
	if (pstate == EST_PARALYZED) pad_this_frame = pad = 0;

	hitv = hith = 0;
	#ifdef FIRE_TO_PUSH
		pushed_any = 0;
	#endif
	//ppossee = 0;

	// ********
	// Vertical
	// ********

	#ifdef PLAYER_MOGGY_STYLE		
		// Poll pad
		if (!(pad & PAD_UP || pad & PAD_DOWN)) {
			pfacingv = 0xff;
			if (pvy > 0) {
				pvy -= PLAYER_RX; if (pvy < 0) pvy = 0;
			} else if (pvy < 0) {
				pvy += PLAYER_RX; if (pvy > 0) pvy = 0;
			}
		}
		
		if (pad & PAD_UP) {
			pfacingv = FACING_UP;
			if (pvy > -PLAYER_VX_MAX) pvy -= PLAYER_AX;
		}
		
		if (pad & PAD_DOWN) {
			pfacingv = FACING_DOWN;
			if (pvy < PLAYER_VX_MAX) pvy += PLAYER_AX;
		}
	#else
		
		#ifdef PLAYER_HAS_SWIM
			if (vertical_engine_type == ENGINE_TYPE_SWIM) {
				if (pad & (PAD_DOWN|PAD_A)) {
					pvy += PLAYER_AY_SWIM;
					if (pvy > PLAYER_VY_SWIM_MAX) pvy = PLAYER_VY_SWIM_MAX;
				} else {
					pvy -= PLAYER_AY_SWIM >> 1;
					if (pvy < -PLAYER_VY_SWIM_MAX) pvy = -PLAYER_VY_SWIM_MAX;
				}
			} else
		#endif
		{
			pvy += pj ? PLAYER_G_JUMPING : PLAYER_G;
			if (pvy > PLAYER_VY_FALLING_MAX) pvy = PLAYER_VY_FALLING_MAX;

			if (pgotten) pvy = 0;
		}

		#ifdef PLAYER_HAS_JETPAC
			if (vertical_engine_type == ENGINE_TYPE_JETPAC) {
				if (pad & PAD_A) {
					pvy -= PLAYER_AY_JETPAC;
					if (pvy < -PLAYER_VY_JETPAC_MAX) pvy = -PLAYER_VY_JETPAC_MAX;
				}
			}
		#endif	
	#endif

	// Move
	py += pvy; if (py < -(8<<FIXBITS)) py = -(8<<FIXBITS);
	pry = py >> FIXBITS;
	ppossee = pquicksanded = 0;

	// Collision
	
	cx1 = (prx + 4) >> 4;
	cx2 = (prx + 11) >> 4;

	#ifdef PLAYER_MOGGY_STYLE
		if (pvy)
	#else
		if (pvy + pgtmy)
	#endif
	{
		#ifdef PLAYER_MOGGY_STYLE
			if (pvy < 0)
		#else
			if (pvy + pgtmy < 0)
		#endif		
		{
			cy1 = cy2 = (pry + 8) >> 4;
			rdb = 8; rdc = (cy1 << 4) + 8;
		}
		#ifdef PLAYER_MOGGY_STYLE			
			else if (pvy > 0)
		#else
			//else if (pvy + pgtmy > 0 && ((pry - 1) & 15) < 8)
			else if (pvy + pgtmy > 0)
		#endif
		{
			cy1 = cy2 = (pry + 16) >> 4;
			#ifndef PLAYER_MOGGY_STYLE
				if (pry > ((cy1 - 1) << 4) + (pvy >> FIXBITS)) goto nopor;
			#endif
			rdb = 12; rdc = (cy1 - 1) << 4;
		} else goto nopor;
		
		cm_two_points ();
		if ((at1 & rdb) || (at2 & rdb)) {
			pvy = 0; pry = rdc; py = pry << FIXBITS; pgotten = 0;
			
			#if (defined(PLAYER_PUSH_BOXES) || !defined(DEACTIVATE_KEYS)) && PLAYER_MOGGY_STYLE
				if (              at1 == 10) process_tile (t1, cx1, cy1
					#ifdef PLAYER_PUSH_BOXES
					, cx1, cy1 + 1
					#endif
				);
				if (cx1 != cx2 && at2 == 10) process_tile (t2, cx2, cy1
					#ifdef PLAYER_PUSH_BOXES
					, cx2, cy1 + 1
					#endif
				);
			#endif
		} else if ((at1 & 1) || (at2 & 1)) hitv = 1;		
	}
	nopor:

	#ifdef DIE_AND_RESPAWN
		psafe = pscreenissafe;
	#endif

	#if defined (ENABLE_QUICKSANDS) || defined (ENABLE_SILLY_SPRINGS)
		cy1 = cy2 = (pry + 15) >> 4; cm_two_points ();

		#ifdef ENABLE_QUICKSANDS		
			if ((at1 == 2) || (at2 == 2)) {
				pquicksanded = 1;
				if (pvy + pgtmy < 0) { pj = 0; }
				else { pvy = PLAYER_VY_SINKING;	ppossee = 1; }
			}
		#endif

		#ifdef ENABLE_SILLY_SPRINGS
			if ((at1 == 48) || (at2 == 48)) {
				if (at1 == 48) _x = cx1; 
				else _x = cx2;
				_y = cy1; _t = SPRING_SPIKE_TILE; map_set ();
				sfx_play (SFX_SPRING, 0);
			}
		#endif
	#endif
	
	cy1 = cy2 = (pry + 16) >> 4; cm_two_points ();
	ppossee = ppossee || (at1 & 12) || (at2 & 12);

	#ifdef ENABLE_SLIPPERY
		pslippery = (at1 & 64) || (at2 & 64);
	#endif

	#ifdef ENABLE_CONVEYORS	
		// Conveyors
		if (ppossee) {
			if (at1 & 32) { 
				pgotten = 1; pgtmy = 0;
				#ifdef DIE_AND_RESPAWN
					psafe = 0;
				#endif 
				pgtmx = _pgtmx [at1 & 1]; 
			}
			if (at2 & 32) { 
				pgotten = 1; pgtmy = 0; 
				#ifdef DIE_AND_RESPAWN
					psafe = 0;
				#endif
				pgtmx = _pgtmx [at2 & 1]; 
			}
		}
	#endif

	#ifdef PLAYER_HAS_JUMP
		// *******************************
		// Jump: PAD_A, change when needed
		// *******************************
		if (vertical_engine_type == ENGINE_TYPE_JUMP) {
			if ((pad_this_frame & PAD_A) && !pj) {
				if (pgotten || ppossee || hitv) {
					pj = 1; pctj = 0; pvy = -PLAYER_VY_JUMP_INITIAL;
					sfx_play (SFX_JUMP, 0);
					#ifdef DIE_AND_RESPAWN
						if (psafe && !pgotten && !hitv && pstate == EST_NORMAL) player_set_safe ();
					#endif	
				}
			} 
			
			if (pad & PAD_A) {
				if (pj) {
					++ pctj; if (pctj == PLAYER_VY_JUMP_A_STEPS) pj = 0;
				}
			} else {
				if (pj) {
					if (pvy < -PLAYER_VY_JUMP_RELEASE) pvy = -PLAYER_VY_JUMP_RELEASE;
				}
				pj = 0;
			}
		}
	#endif

	// **********
	// Horizontal
	// **********

	#ifdef PLAYER_ALTERED_HORIZONTAL_MOVEMENT

		#ifdef ENABLE_SLIPPERY
			__asm__ ("lda %v", pslippery);
			__asm__ ("beq %g", player_move_no_slippery);
			__asm__ ("ldx #%b", PLAYER_AX_SLIPPERY);
			//__asm__ ("ldy #%b", PLAYER_RX_SLIPPERY);
			// I had to do this manual fix as the "value" is (half_life)
			__asm__ ("ldy %v", PLAYER_RX_SLIPPERY);
			__asm__ ("lda #%b", PLAYER_VX_MAX_SLIPPERY);
			__asm__ ("jmp %g", player_move_horizontal_set);
		player_move_no_slippery:
		#endif

		#ifdef ENABLE_QUICKSANDS
			__asm__ ("lda %v", pquicksanded);
			__asm__ ("beq %g", player_move_no_quicksands);
			__asm__ ("ldx #%b", PLAYER_AX_QUICKSANDS);
			__asm__ ("ldy #%b", PLAYER_RX_QUICKSANDS);
			__asm__ ("lda #%b", PLAYER_VX_MAX_QUICKSANDS);
			__asm__ ("jmp %g", player_move_horizontal_set);
		player_move_no_quicksands:
		#endif

		__asm__ ("ldx #%b", PLAYER_AX);
		__asm__ ("ldy #%b", PLAYER_RX);
		__asm__ ("lda #%b", PLAYER_VX_MAX);

	player_move_horizontal_set:
		__asm__ ("stx %v", player_ax);
		__asm__ ("sty %v", player_rx);
		__asm__ ("sta %v", player_vx_max);

	#endif
	
	// Poll pad
	if (!((pad & PAD_LEFT) || (pad & PAD_RIGHT))) {
		#ifdef PLAYER_MOGGY_STYLE
			pfacingh = 0xff;
		#endif
		if (pvx > 0) {
			pvx -= _PLAYER_RX; if (pvx < 0) pvx = 0;
		} else if (pvx < 0) {
			pvx += _PLAYER_RX; if (pvx > 0) pvx = 0;
		}
	}
	
	if (pad & PAD_LEFT) {
		#ifdef PLAYER_MOGGY_STYLE
			pfacingh = FACING_LEFT;
		#endif
		pvx -= _PLAYER_AX;
		if (pvx < -_PLAYER_VX_MAX) pvx = -_PLAYER_VX_MAX;
		#ifndef PLAYER_MOGGY_STYLE			
			pfacing = 0;
		#endif			
	}
	
	if (pad & PAD_RIGHT) {
		#ifdef PLAYER_MOGGY_STYLE
			pfacingh = FACING_RIGHT;
		#endif
		pvx += _PLAYER_AX;
		if (pvx > _PLAYER_VX_MAX) pvx = _PLAYER_VX_MAX;		
		#ifndef PLAYER_MOGGY_STYLE			
			pfacing = 1;
		#endif			
	}
	
	// Move
	px += pvx;
	if (px < (0 << FIXBITS)) px = 0 << FIXBITS;
	if (px > (240 << FIXBITS)) px = 240 << FIXBITS;

	#ifndef PLAYER_MOGGY_STYLE	
		if (pgotten) px += pgtmx;
	#endif	
		
	prx = px >> FIXBITS;

	// Collision

	#ifdef PLAYER_MOGGY_STYLE
		if (pvx) 
	#else
		if (pvx + pgtmx)
	#endif
	{
		cy1 = (pry + 8) >> 4;
		cy2 = (pry + 15) >> 4;
		#ifdef PLAYER_MOGGY_STYLE	
			if (pvx < 0) 
		#else
			if (pvx + pgtmx < 0) 			
		#endif
		{
			cx1 = cx2 = (prx + 4) >> 4;
			rdc = ((cx1 + 1) << 4) - 4;			
		}
		#ifdef PLAYER_MOGGY_STYLE	
			else if (pvx > 0)
		#else
			else if (pvx + pgtmx > 0)
		#endif		
		{
			cx1 = cx2 = (prx + 12) >> 4;
			rdc = ((cx1 - 1) << 4) + 4;
		}
	
		cm_two_points ();
		if ((at1 & 8) || (at2 & 8)) {
			pvx = 0; prx = rdc; px = prx << FIXBITS;
			#if defined(PLAYER_PUSH_BOXES) || !defined(DEACTIVATE_KEYS)
				if (              at1 == 10) process_tile (t1, cx1, cy1
					#ifdef PLAYER_PUSH_BOXES
					, cx1 + 1, cy1
					#endif
				);
				if (cy1 != cy2 && at2 == 10) process_tile (t2, cx1, cy2
					#ifdef PLAYER_PUSH_BOXES
					, cx1 + 1, cy2
					#endif
				);
			#endif
		} else hith = (at1 & 1) || (at2 & 1);
	}

	// Facing
	#ifdef PLAYER_MOGGY_STYLE	
		#ifdef TOP_OVER_SIDE
			if (pfacingv != 0xff) {
				pfacing = pfacingv; 
			} else if (pfacingh != 0xff) {
				pfacing = pfacingh;
			}
		#else
			if (pfacingh != 0xff) {
				pfacing = pfacingh; 
			} else if (pfacingv != 0xff) {
				pfacing = pfacingv;
			}
		#endif
	#endif

	// ************
	// Center point
	// ************

	_x = (prx + 8) >> 4;
	rdm = ((pry + 8) & 0xf0) | _x; cy1 = map_attr [rdm];
	#ifdef ENABLE_TILE_GET
		if (cy1 == 128) {
			_y = (pry + 8) >> 4; _t = c_tsmap [0]; 
			sfx_play (SFX_RING, 0);
			#ifdef ACTIVATE_HOOKS
				on_tile_get ();
			#endif
			map_set ();
		}
	#endif
	#ifdef ENABLE_SILLY_SPRINGS
		if (cy1 == 1) phit = 1;
	#endif

	// *************
	// Killing tiles
	// *************
	
	if (hitv) {
		phit = 1;
		#ifdef FULL_BOUNCE
			pvy = add_sign (-pvy, PLAYER_VX_MAX);
		#endif
		#ifdef DOUBLE_BOUNCE
			pvy = add_sign (-pvy, PLAYER_VX_MAX << 1);
		#endif		
	} else if (hith) {
		phit = 1;
		#ifdef FULL_BOUNCE
			pvx = add_sign (-pvx, PLAYER_VX_MAX);
		#endif
		#ifdef DOUBLE_BOUNCE
			pvx = add_sign (-pvx, PLAYER_VX_MAX << 1);
		#endif		
	}

	// ************
	// Fire bullets
	// ************
	
	#ifdef PLAYER_CAN_FIRE
		#ifdef FIRE_TO_PUSH
			if ((pad & PAD_A) && !pfiring && !pushed_any) 
		#else
			if ((pad & PAD_A) && !pfiring) 
		#endif
		{
			pfiring = 1;
			fire_bullet ();
		}
		
		if (!(pad & PAD_A)) pfiring = 0;
	#endif

	// ******
	// Hitter
	// ******

	#ifdef ENABLE_HITTER
		if (pad_this_frame & PAD_B) {
			if (hitter_on == 0) {
				hitter_on = 1; hitter_frame = 0;
			}
		}
	#endif
	
	// Calc frame
	// Basic, v.1.0
	#ifdef PLAYER_MOGGY_STYLE
		if (pvx != 0 || pvy != 0) {
			++ pctfr;
			if (pctfr == 4) {
				pctfr = 0;
				pfr = !pfr;
				psprid = player_frames [pfacing + pfr];
			}
		}
	#else
		// 0 1 2 3 + facing: walk, 1 = stand. 8 + facing = jump/fall
		#ifdef PLAYER_HAS_SWIM
			if (vertical_engine_type == ENGINE_TYPE_SWIM) {
				psprid = 8 + player_frames [
					(pfacing << 2) + 
					(
						(pad & (PAD_LEFT|PAD_RIGHT|PAD_DOWN|PAD_UP|PAD_A)) ?
						((frame_counter >> 2) & 3) :
						1
					)
				];				
			} else 
		#endif
		{
			if (ppossee || pgotten) {
				psprid = player_frames [(pfacing << 2) + (pvx ? (prx >> 3) & 3 : 1)];
			} else {
				psprid = player_frames [8 + pfacing];
			}
		}
	#endif

	if (pstate != EST_FLICKERING || half_life) {
		rda = psprid + c_player_offset;
		oam_2x2_meta_spr (prx + ((pstate == EST_PARALYZED) & half_life), pry + SPRITE_ADJUST, 128, spriteset_pals [rda], rda << 2);
	} else {
		oam_2x2_meta_spr (prx, 240, 128, 0, 252);
	}
}
