// MTE MK1_NES / AGNES 0.1 R
// Copyleft Mojon Twins 2013, 2019

// Extra functions. Add here helper functions you need elsewhare (in hooks)

// Custom function
void clear_tile_rdn (void) {
	// Check buffer to substitute tile rdn for c_tsmap [0]
		__asm__ ("lda #0");
		__asm__ ("sta %v", rdx);
		__asm__ ("sta %v", rdy);
		__asm__ ("tax");
	substitute_10_loop:
		__asm__ ("lda %v, x", map_buff);
		__asm__ ("cmp %v", rdn);
		__asm__ ("bne %g", substitute_10_continue);

		__asm__ ("lda %v", rdx);
		__asm__ ("sta %v", _x);
		__asm__ ("lda %v", rdy);
		__asm__ ("sta %v", _y);
		__asm__ ("ldy #0");
		__asm__ ("lda (%v), y", c_tsmap);
		__asm__ ("sta %v", _t);

		__asm__ ("stx %v", rdm);
		map_set ();
		__asm__ ("ldx %v", rdm);

	substitute_10_continue:
		__asm__ ("inc %v", rdx);
		__asm__ ("lda %v", rdx);
		__asm__ ("cmp #16");
		__asm__ ("bne %g", substitute_10_noincy);
		__asm__ ("inc %v", rdy);
		__asm__ ("lda #0");
		__asm__ ("sta %v", rdx);

	substitute_10_noincy:
		__asm__ ("inx");
		__asm__ ("cpx #192");
		__asm__ ("bne %g", substitute_10_loop);
}

// Custom function 
void lava_launch (void) {
	sfx_play (SFX_BREAKH, 0);
	f1 = 1;
	pal_bg (mypal_game_bg8);
	lava_ticker = lava_delay;
}

// Custom function
void lava_draw_off (void) {
	// Draws lava from lava_cur_height to bottom.
	// Lava top
	rdy = lava_cur_height + TOP_ADJUST;
	vram_write ((unsigned char *) lava_top, NAMETABLE_A + shl5 [rdy], 32);
	
	// Lava body
	for (gpit = lava_cur_height + 1; gpit < 24; gpit ++)
		vram_write ((unsigned char *) lava_body + (rand8 () & 0x1f), NAMETABLE_A + shl5 [gpit + TOP_ADJUST], 32);

	// And the attributes
		__asm__ ("lda %v", rdy);
		__asm__ ("and #$fc");
		__asm__ ("asl a");
		__asm__ ("sta %v", rdy);
	
	memfill (attr_table + rdy, 0xaa, 64-rdy);	
}

// Custom function
void lava_draw_buff (void) {
	if (lava_cur_height == 23) return;

	rdy = lava_cur_height + TOP_ADJUST;
	gp_addr = NAMETABLE_A + shl5 [rdy];

	// Draws lava_top and a strip of lava_body to the update list.
	ul = &update_list [update_index];

		__asm__ ("ldy #0");
		__asm__ ("lda %v + 1", gp_addr);
		__asm__ ("ora #%b", NT_UPD_HORZ);
		__asm__ ("sta (%v), y", ul);
		
		__asm__ ("iny");
		__asm__ ("lda %v", gp_addr);
		__asm__ ("sta (%v), y", ul);
		
		__asm__ ("iny");
		__asm__ ("lda #64");
		__asm__ ("sta (%v), y", ul);

	// Now copy 32 patterns from lava_top to ul
	memcpy (ul + 3, lava_top, 32);

	// Now copy 32 patterns from lava_body to ul
	memcpy (ul + 35, lava_body + (rand8 () & 0x1f), 32);

	// Don't forget to update update_index!
	update_index += (3 + 64);

	// Attributes
	gp_addr = NAMETABLE_A + 0x3c0 + ((rdy & 0xfc) << 1);

	ul = &update_list [update_index];

		__asm__ ("ldy #0");
		__asm__ ("lda %v + 1", gp_addr);
		__asm__ ("ora #%b", NT_UPD_HORZ);
		__asm__ ("sta (%v), y", ul);
		
		__asm__ ("iny");
		__asm__ ("lda %v", gp_addr);
		__asm__ ("sta (%v), y", ul);
		
		__asm__ ("iny");
		__asm__ ("lda #8");
		__asm__ ("sta (%v), y", ul);

	// Fill with 8 0b10101010 to set all to pal #2
	memfill (ul + 3, 0xAA, 8);

	// Don't forget to update update_index!
	update_index += (3 + 8);
}

// Custom function
void lava_calc (void) {
	// lava_height contains the absolute height. First of all
	// we must divide it by 24 to see if lava is currently on
	// screen.
	
	// Which screen row? 
	
	rdb = (level == 9 ? (n_pant >> 1) : n_pant);

		__asm__ ("lda %v", lava_height);

	// Divide by 24 by Omegamatrix
	// temp store is rda.
		
		__asm__ ("lsr");
		__asm__ ("lsr");
		__asm__ ("lsr");
		__asm__ ("sta %v", rda);
		__asm__ ("lsr");
		__asm__ ("lsr");
		__asm__ ("adc %v", rda);
		__asm__ ("ror");
		__asm__ ("lsr");
		__asm__ ("adc %v", rda);
		__asm__ ("ror");
		__asm__ ("lsr");

	// Currently, lava is at screen row A

		__asm__ ("cmp %v", rdb);
		__asm__ ("bne %g", lava_off_screen);

	// Lava is on screen!
	// Calculate lava_height % 24.
	// As v = (lava_height / 24) is on A...

		__asm__ ("asl a");
		__asm__ ("asl a");
		__asm__ ("asl a");
		__asm__ ("sta %v", rda);	// rda = v*8
		__asm__ ("asl a");			// A = v*16
		__asm__ ("clc");
		__asm__ ("adc %v", rda);
		__asm__ ("sta %v", rda);	// rda = v*8+v*16 = v*24

		__asm__ ("lda %v", lava_height);
		__asm__ ("sec");
		__asm__ ("sbc %v", rda);
		__asm__ ("sta %v", lava_cur_height);

		lava_height_pixels = lava_height << 3;
		lava_on_screen = 1;
		return;

	lava_off_screen:
		lava_on_screen = 0;
}
