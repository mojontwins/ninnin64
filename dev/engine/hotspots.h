// MTE MK1_NES / AGNES 0.1 R
// Copyleft Mojon Twins 2013, 2019

// hotspots.h
// Hotspot management

void hotspots_load (void) {
	// Copies hotspots from ROM to RAM and initializes them
	
		__asm__ ("ldx #0");
		__asm__ ("ldy #0");
	hotspots_load_loop:
		__asm__ ("lda (%v), y", c_hotspots);
		__asm__ ("iny");
		__asm__ ("sta %v, x", ht);
	
		__asm__ ("lda (%v), y", c_hotspots);
		__asm__ ("iny");
		__asm__ ("sta %v, x", hyx);

		__asm__ ("lda #1");
		__asm__ ("sta %v, x", hact);

		__asm__ ("inx");
		__asm__ ("cpx #%b", MAP_W * MAP_H);
		__asm__ ("bne %g", hotspots_load_loop);
}

// v0.1: Simple. Modify/expand when needed.
void hotspots_create (void) {
		__asm__ ("lda #0");
		__asm__ ("sta %v", hrt);

		__asm__ ("ldx %v", n_pant);
		
		__asm__ ("lda %v, x", hact);
		__asm__ ("beq %g", hotspots_create_done);

		__asm__ ("lda %v, x", ht);
		__asm__ ("beq %g", hotspots_create_done);

		__asm__ ("sta %v", hrt);

		__asm__ ("lda %v, x", hyx);
		__asm__ ("and #$f0");
		__asm__ ("sta %v", hry);

		__asm__ ("lda %v, x", hyx);
		__asm__ ("asl a");
		__asm__ ("asl a");
		__asm__ ("asl a");
		__asm__ ("asl a");
		__asm__ ("sta %v", hrx);

		__asm__ ("lsr a");
		__asm__ ("lsr a");
		__asm__ ("lsr a");
		__asm__ ("sta %v", _x);

		__asm__ ("lda %v", hry);
		__asm__ ("lsr a");
		__asm__ ("lsr a");
		__asm__ ("lsr a");
		__asm__ ("clc");
		__asm__ ("adc #%b", TOP_ADJUST);
		__asm__ ("sta %v", _y);

		__asm__ ("lda %v", hrt);
		__asm__ ("clc");
		__asm__ ("adc #%b", HOTSPOTS_TILE_BASE);
		__asm__ ("sta %v", _t);

		draw_tile ();
	hotspots_create_done:
		;
}
