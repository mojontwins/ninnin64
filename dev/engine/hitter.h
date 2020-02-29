// MTE MK1_NES / AGNES 0.1 R
// Copyleft Mojon Twins 2013, 2019

const unsigned char hitter_offs [] = {12, 14, 16, 16, 12};
#define HITTER_FRAMES 5

void hitter_do (void) {
	//hitter_y = pry + HITTER_Y_OFFS; hitter_hit_y = hitter_y + 4;
		__asm__ ("lda %v", pry);
		__asm__ ("clc");
		__asm__ ("adc #%b", HITTER_Y_OFFS);
		__asm__ ("sta %v", hitter_y);
		//__asm__ ("clc");	// Carry is NOT set here
		__asm__ ("adc #4");
		__asm__ ("sta %v", hitter_hit_y);

	/*
	if (pfacing) {
		hitter_x = prx + hitter_offs [hitter_frame];
		hitter_hit_x = hitter_x + 7;
	} else {
		hitter_x = prx + 8 - hitter_offs [hitter_frame];
		hitter_hit_x = hitter_x;
	}
	*/
		__asm__ ("ldx %v", hitter_frame);
		__asm__ ("lda %v", pfacing);
		__asm__ ("beq %g", hitter_facing_left);

	// Hitter facing right
		__asm__ ("lda %v", prx);
		__asm__ ("adc %v, x", hitter_offs);
		__asm__ ("sta %v", hitter_x);
		__asm__ ("adc #7");
		__asm__ ("jmp %g", hitter_facing_done);

	hitter_facing_left:
		__asm__ ("lda %v", prx);
		__asm__ ("adc #8");
		__asm__ ("sbc %v, x", hitter_offs);
		__asm__ ("sta %v", hitter_x);

	hitter_facing_done:
		__asm__ ("sta %v", hitter_hit_x);
		
	// if (hitter_frame < HITTER_ACTIVE_FRAME_A || hitter_frame > HITTER_ACTIVE_FRAME_B) hitter_hit_y = 0;
		__asm__ ("lda %v", hitter_frame);
		__asm__ ("cmp #%b", HITTER_ACTIVE_FRAME_A);
		__asm__ ("bcc %g", hitter_inactive);

		__asm__ ("cmp #%b", HITTER_ACTIVE_FRAME_B);
		__asm__ ("beq %g", hitter_active);
		__asm__ ("bcc %g", hitter_active);
	hitter_inactive:
		__asm__ ("lda #0");
		__asm__ ("sta %v", hitter_hit_y);
	hitter_active:

	oam_spr (
		hitter_x, hitter_y + SPRITE_ADJUST, 
		0xf9 - pfacing, 1, 252
	);

	#ifdef ENABLE_BREAKABLE
		if (hitter_hit_y) {
			_x = hitter_hit_x >> 4;
			_y = hitter_hit_y >> 4;
			if (map_attr [_x | (hitter_hit_y & 0xf0)] == 24) {
				break_wall ();
				hitter_on = 0;
			}
		}
	#endif

	if (half_life) { ++ hitter_frame; if (hitter_frame == HITTER_FRAMES) hitter_on = 0; }
}
