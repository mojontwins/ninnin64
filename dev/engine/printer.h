// MTE MK1_NES / AGNES 0.1 R
// Copyleft Mojon Twins 2013, 2019

// printer.h
// Draw map, print text, etcetera.


// fade out
void fade_out (void) {
	for (gpit = 4; gpit != 0xff; -- gpit) {
		pal_bright (gpit); delay (fade_delay);
	}	
}

// fade in
void fade_in (void) {
	for (gpit = 0; gpit < 5; ++ gpit) {
		pal_bright (gpit); delay (fade_delay);
	}	
}

const unsigned char bitmasks [] = {0xfc, 0xf3, 0xcf, 0x3f};

void do_attr_table (void) {
	/*
	rdc = (_x >> 2) + ((_y << 1) & 0xf8); //((_y >> 2) << 3);
	rdb = ((_x >> 1) & 1) + (_y & 2); //(((_y >> 1) & 1) << 1);
	attr_table [rdc] = rda = (attr_table [rdc] & bitmasks [rdb]) | (tileset_pals [_t] << (rdb << 1));	
	*/

		// rdc = (_x >> 2) + ((_y >> 2) << 3);
		// ((_y >> 2) << 3) is the same as (_y << 1) & 0xf8;
		
		__asm__ ("lda %v", _x);
		__asm__ ("lsr a");
		__asm__ ("lsr a");
		__asm__ ("sta %v", rdc);	// rdc = (_x >> 2)

		__asm__ ("lda %v", _y);
		__asm__ ("asl a");
		__asm__ ("and #$f8");		// A = (_y << 1) & 0xf8

		__asm__ ("clc");
		__asm__ ("adc %v", rdc);
		__asm__ ("sta %v", rdc);	// Result

		// rda = attr_table [rdc];
		// rdc is on A
		__asm__ ("tax");
		__asm__ ("lda %v, x", attr_table);
		__asm__ ("sta %v", rda);

		// rdb = ((_x >> 1) & 1) + (((_y >> 1) & 1) << 1);
		// (((_y >> 1) & 1) << 1) is the same as (_y & 2)
		
		__asm__ ("lda %v", _x);
		__asm__ ("lsr a");
		__asm__ ("and #1");
		__asm__ ("sta %v", rdb);	// rdb = ((_x >> 1) & 1)

		__asm__ ("lda %v", _y);
		__asm__ ("and #2");

		__asm__ ("clc");
		__asm__ ("adc %v", rdb);
		__asm__ ("sta %v", rdb);	// Result
		
		//rda = (rda & bitmasks [rdb]) | (c_ts_pals [_t] << (rdb << 1));		
		
		__asm__ ("ldy %v", _t);
		__asm__ ("lda %v, y", tileset_pals);
									// A = c_ts_pals [_t];

		// Shift left (rdb << 1) times

		__asm__ ("ldx %v", rdb);	
		__asm__ ("beq %g", upd_attr_table_loop_0_skip);

	upd_attr_table_loop_0:
		__asm__ ("asl a");
		__asm__ ("asl a");
		__asm__ ("dex");
		__asm__ ("bne %g", upd_attr_table_loop_0);

	upd_attr_table_loop_0_skip:
		__asm__ ("sta %v", i);			// i = (c_ts_pals [_t] << (rdb << 1));

		__asm__ ("lda %v", rda);
		__asm__ ("ldx %v", rdb);
		__asm__ ("and %v, x", bitmasks);
									// A = (rda & bitmasks [rdb]);

		__asm__ ("ora %v", i);
		__asm__ ("sta %v", rda);	// Result

		// attr_table [rdc] = rda;
		// rda is already on A
		__asm__ ("ldx %v", rdc);
		__asm__ ("sta %v, x", attr_table);
}

void calc_tile (void) {
	_t = (16 + _t) << 2;
}

void calc_gp_addr (void) {
	gp_addr = (((_y << 5) | _x) | NAMETABLE_A);
}

void draw_tile (void) {
	do_attr_table ();
	calc_tile ();
	calc_gp_addr ();
	vram_adr (gp_addr);
	VRAM_PUT (_t); ++ _t;
	VRAM_PUT (_t); ++ _t;
	gp_addr += 32;
	vram_adr (gp_addr);
	VRAM_PUT (_t); ++ _t;	
	VRAM_PUT (_t);	
}

void ul_put2c_hrd (void) {
		__asm__ ("lda %v+1", gp_addr);
		__asm__ ("ora #%b", NT_UPD_HORZ);
		__asm__ ("sta (%v), y", ul);

		__asm__ ("iny");
		__asm__ ("lda %v", gp_addr);
		__asm__ ("sta (%v), y", ul);

		__asm__ ("iny");
		__asm__ ("lda #2");
		__asm__ ("sta (%v), y", ul);
}

void ul_put_t_inc (void) {
		__asm__ ("iny");
		__asm__ ("lda %v", _t);
		__asm__ ("sta (%v), y", ul);
		__asm__ ("inc %v", _t);
}

#pragma optimize (off)
void update_list_tile (void) {
	ul = &update_list [update_index];

	do_attr_table ();
	gp_addr = 0x23c0 + rdc;	

	// ul [10] = MSB (gp_addr);
	// ul [11] = LSB (gp_addr);
	// ul [12] = rda;
		__asm__ ("ldy #10");
		__asm__ ("lda %v+1", gp_addr);
		__asm__ ("sta (%v), y", ul);
		__asm__ ("iny");
		__asm__ ("lda %v", gp_addr);
		__asm__ ("sta (%v), y", ul);
		__asm__ ("iny");
		__asm__ ("lda %v", rda);
		__asm__ ("sta (%v), y", ul);

	// tiles
	calc_tile ();
	calc_gp_addr ();

	// ul [0] = MSB (gp_addr) | NT_UPD_HORZ
	// ul [1] = LSB (gp_addr)
	// ul [2] = 2
		__asm__ ("ldy #0");		
	ul_put2c_hrd ();
	
	// ul [3] = _t; ++_t;
	// ul [4] = _t; ++ t;
	ul_put_t_inc ();
	ul_put_t_inc ();		

	// gp_addr += 32;		
		__asm__ ("lda %v", gp_addr);
		__asm__ ("clc");
		__asm__ ("adc #32");
		__asm__ ("sta %v", gp_addr);
		__asm__ ("bcc %g", ult_incsk2);
		__asm__ ("inc %v", gp_addr);
	ult_incsk2:

	// ul [5] = MSB (gp_addr) | NT_UPD_HORZ
	// ul [6] = LSB (gp_addr)
	// ul [7] = 2
		__asm__ ("iny");		
	ul_put2c_hrd ();
	
	// ul [8] = _t; ++_t;
	// ul [9] = _t; ++ t;
	ul_put_t_inc ();
	ul_put_t_inc ();	

	update_index += 13;
}

void p_t (void) {
	// Adapted from code by Bregalad

		// A = number (0-99)
		__asm__ ("lda %v", _t);

	    __asm__ ("ldx #$ff");
	    __asm__ ("sec");
	p_t_loop_0:
		__asm__ ("inx");
	    __asm__ ("sbc #10");
	    __asm__ ("bcs %g", p_t_loop_0);
	    __asm__ ("adc #10");

		// A = lower digit (0-9), X=upper digit(0-9)
		__asm__ ("sta %v", rda);
		__asm__ ("stx %v", _n);

	calc_gp_addr ();
	ul = &update_list [update_index];

		__asm__ ("ldy #0");		
		ul_put2c_hrd ();

		__asm__ ("lda %v", _n);
		__asm__ ("clc");
		__asm__ ("adc #16");
		__asm__ ("iny");
		__asm__ ("sta (%v), y", ul);
		
		__asm__ ("lda %v", rda);
		__asm__ ("clc");
		__asm__ ("adc #16");		
		__asm__ ("iny");
		__asm__ ("sta (%v), y", ul);

	update_index += 5;
}
#pragma optimize (on)

void add_tile (void) {
	#if defined (ENABLE_BREAKABLE) && !defined (BREAKABLES_SOFT)
		brk_buff [rdm] = 1;
	#endif

	_t = rdt;

	#ifdef MAP_CUSTOM_RENDERER
		#include "my/custom_renderer.h"
	#else
		if (c_alt_bg && _t == 0 && (rand8 () & 15) == 1) _t = c_alt_bg;
		else _t = 
		#ifdef MAPPED_TILESETS
			c_tsmap [_t];
		#else
			ts_offs + _t;
		#endif
	#endif

	map_buff [rdm] = _t;		
	map_attr [rdm] = behs [_t];				

	#ifndef MAP_CUSTOM_RENDERER
		draw_tile ();
		_x = (_x + 2) & 31; if (!_x) _y += 2;
	#endif

	++ rdm;
}

void draw_scr (void) {
	_x = 0; _y = TOP_ADJUST;

	#ifdef MAP_FORMAT_RLE44
		#ifdef MAP_IN_CHR_ROM
			bankswitch (c_map_chr_rom_bank);
			vram_adr (c_map [n_pant]);
			rda = VRAM_READ; 	// Dummy read.
		#else
			gp_map = c_map [n_pant];
		#endif

		rdm = 0; while (rdm < 192) {
			
			#ifdef MAP_IN_CHR_ROM
				rda = VRAM_READ;
			#else
				//rda = *gp_map; ++ gp_map;
				__asm__ ("ldy #0");
				__asm__ ("lda (%v), y", gp_map);
				__asm__ ("inc %v", gp_map);
				__asm__ ("bne %g", ds_incd);
				__asm__ ("inc %v+1", gp_map);
			ds_incd:
			#endif

			// rdct = rda;
			// rdt = rda & 0x0f;
				__asm__ ("sta %v", rdct);				
				__asm__ ("and #$0f");
				__asm__ ("sta %v", rdt);
			
			while (rdct >= 16) {
				add_tile ();
				rdct -= 16; 
			} add_tile ();
		}
	#else
		// Draw current screen
		gp_map = c_map + n_pant * 96;
		
		rdm = 0; while (rdm < 192) {
			if ((rdm & 1) == 0) {
				rdt = gp_map [rdm >> 1] >> 4;
			} else {
				rdt = gp_map [rdm >> 1] & 15;
			}
			add_tile ();
		}
	#endif

	bankswitch (DEFAULT_CHR_ROM);

	#ifdef MAP_CUSTOM_RENDERER
		/*
		for (rdm = 0; rdm < 192; ++ rdm) {
			_t = map_buff [rdm];
			draw_tile ();
			_x = (_x + 2) & 31; if (!_x) _y += 2;
		}
		*/
			__asm__ ("ldx #0");
		map_custom_renderer_loop:
			__asm__ ("lda %v, x", map_buff);
			__asm__ ("sta %v", _t);
			__asm__ ("stx %v", rdm);
			draw_tile ();
			__asm__ ("ldx %v", rdm);
			__asm__ ("inx");
			__asm__ ("cpx #192");
			__asm__ ("beq %g", map_custom_renderer_done);
			__asm__ ("lda %v", _x);
			__asm__ ("clc");
			__asm__ ("adc #2");
			__asm__ ("and #31");
			__asm__ ("sta %v", _x);
			__asm__ ("bne %g", map_custom_renderer_loop);
			__asm__ ("lda %v", _y);
			__asm__ ("clc");
			__asm__ ("adc #2");
			__asm__ ("sta %v", _y);
			__asm__ ("jmp %g", map_custom_renderer_loop);

		map_custom_renderer_done:
	#endif
	
	// Clear open locks
	#ifndef DEACTIVATE_KEYS	
		if (c_bolts_max) {
			__asm__ ("ldx #0");

		clear_open_locks_loop:
			__asm__ ("lda %v, x", lknp);
			__asm__ ("cmp %v", n_pant);
			__asm__ ("bne %g", clear_open_locks_continue);
			__asm__ ("lda %v, x", lkact);
			__asm__ ("bne %g", clear_open_locks_continue);

			__asm__ ("lda %v, x", lkyx);		
			__asm__ ("lsr a");
			__asm__ ("lsr a");
			__asm__ ("lsr a");
			__asm__ ("and #$fe");
			__asm__ ("clc");
			__asm__ ("adc #%b", TOP_ADJUST);
			__asm__ ("sta %v", _y);

			__asm__ ("lda %v, x", lkyx);
			__asm__ ("and #$0f");
			__asm__ ("asl a");
			__asm__ ("sta %v", _x);

			__asm__ ("lda #0");
			__asm__ ("sta %v", _t);

			__asm__ ("stx %v", gpit);
			draw_tile ();
			__asm__ ("ldx %v", gpit);

			__asm__ ("lda #0");
			__asm__ ("ldy %v, x", lkyx);
			__asm__ ("sta %v, y", map_attr);
			__asm__ ("sta %v, y", map_buff);

		clear_open_locks_continue:
			__asm__ ("inx");
			__asm__ ("cpx %v", c_bolts_max);
			__asm__ ("bne %g", clear_open_locks_loop);
		}
	#endif

	#ifdef BREAKABLE_ANIM
		do_process_breakable = 0;
		memfill (brkf, 0, BREAKABLE_MAX);
	#endif
}

void pr_str (void) {
	calc_gp_addr ();
	vram_adr (gp_addr);
	while (gpit = *_s ++) {
		if (gpit == '%') { gp_addr += 32; vram_adr (gp_addr); }
		else { 
			//rdi = gpit - 32; VRAM_PUT (rdi); 
			__asm__ ("lda %v", gpit);
			__asm__ ("sec");
			__asm__ ("sbc #32");
			__asm__ ("sta $2007");
		}
	}
}

// V2 also adds attributes to the stream
void un_rle_paint (void) {
	if (rdm == 0) {
		// Mode 0 = paint tiles
		_t = rleb; draw_tile ();
		_x = (_x + 2) & 31; if (!_x) _y +=2;
		++ gpit; if (gpit == 240) {
			rdm = 1; vram_adr (0x23c0);
		}
	} else {
		// Mode 1 = draw attributes
		VRAM_PUT (rleb);
	}
}

void un_rle_screen2 (unsigned char *rle) {
	gp_map = rle;

	rlem = *gp_map; ++ gp_map;	// Read marker
	rdm = _x = _y = gpit = 0;

	while (1) {
		rda = *gp_map; ++ gp_map;
		if (rda == rlem) {
			// Marker. Get run value:
			rler = *gp_map; ++ gp_map;
			if (rler == 0) break;
			while (rler --) un_rle_paint ();
		} else {
			rleb = rda; un_rle_paint ();
		}
	}
}
