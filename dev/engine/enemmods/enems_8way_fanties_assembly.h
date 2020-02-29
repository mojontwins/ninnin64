// MTE MK1_NES / AGNES 0.1 R
// Copyleft Mojon Twins 2013, 2019

	if ((rand8 () & 7) < 3) goto enems_8way_fanties_done;

	__asm__ ("lda %v", frame_counter);
	__asm__ ("and %v", _en_mx);
	__asm__ ("bne %g", enems_8way_fanties_done);

	__asm__ ("lda %v", _en_x);
	__asm__ ("sta %v", rdx);
	__asm__ ("lda %v", _en_y);
	__asm__ ("sta %v", rdy);

	// Vertical

 	__asm__ ("cmp %v", pry);
 	__asm__ ("beq %g", enems_8way_fanties_vertical_done);

 	__asm__ ("lda %v", _en_x);
 	__asm__ ("lsr a");
 	__asm__ ("lsr a");
 	__asm__ ("lsr a");
 	__asm__ ("lsr a");
 	__asm__ ("sta %v", cx1);

 	__asm__ ("lda %v", _en_x);
 	__asm__ ("clc");
 	__asm__ ("adc #15");
 	__asm__ ("lsr a");
 	__asm__ ("lsr a");
 	__asm__ ("lsr a");
 	__asm__ ("lsr a");
 	__asm__ ("sta %v", cx2);

 	__asm__ ("lda %v", _en_y);
 	__asm__ ("cmp %v", pry);
 	__asm__ ("bcc %g", enems_8way_fanties_move_down);

 	// Move up
 	__asm__ ("dec %v", _en_y);
 	__asm__ ("lda %v", _en_y);
 	__asm__ ("jmp %g", enems_8way_fanties_vert_cy_set);

enems_8way_fanties_move_down:
 	// Move down
 	__asm__ ("inc %v", _en_y);
 	__asm__ ("lda %v", _en_y);
 	__asm__ ("clc");
 	__asm__ ("adc #15");

enems_8way_fanties_vert_cy_set:
 	__asm__ ("lsr a");
 	__asm__ ("lsr a");
 	__asm__ ("lsr a");
 	__asm__ ("lsr a");
 	__asm__ ("sta %v", cy1);
 	__asm__ ("sta %v", cy2);

 	cm_two_points ();

 	__asm__ ("lda %v", at1);
 	__asm__ ("and #9");
 	__asm__ ("bne %g", enems_8way_fanties_vertical_do);

 	__asm__ ("lda %v", at2);
 	__asm__ ("and #9");
 	__asm__ ("beq %g", enems_8way_fanties_vertical_done);

enems_8way_fanties_vertical_do:
 	__asm__ ("lda %v", rdy);
 	__asm__ ("sta %v", _en_y);
 	
enems_8way_fanties_vertical_done:

	// Horizontal

	__asm__ ("lda %v", _en_x);
 	__asm__ ("cmp %v", prx);
 	__asm__ ("beq %g", enems_8way_fanties_horizontal_done);

 	__asm__ ("lda %v", _en_y);
 	__asm__ ("lsr a");
 	__asm__ ("lsr a");
 	__asm__ ("lsr a");
 	__asm__ ("lsr a");
 	__asm__ ("sta %v", cy1);

 	__asm__ ("lda %v", _en_y);
 	__asm__ ("clc");
 	__asm__ ("adc #15");
 	__asm__ ("lsr a");
 	__asm__ ("lsr a");
 	__asm__ ("lsr a");
 	__asm__ ("lsr a");
 	__asm__ ("sta %v", cy2);

 	__asm__ ("lda %v", _en_x);
 	__asm__ ("cmp %v", prx);
 	__asm__ ("bcc %g", enems_8way_fanties_move_right);

 	// Move left
 	#ifdef FANTY_WITH_FACING
 		__asm__ ("lda %v", en_sprid);
 		__asm__ ("clc");
 		__asm__ ("adc #8");
 		__asm__ ("sta %v", en_sprid);
 	#endif

 	__asm__ ("dec %v", _en_x);
 	__asm__ ("lda %v", _en_x);
 	__asm__ ("jmp %g", enems_8way_fanties_horz_cx_set);

enems_8way_fanties_move_right:
 	// Move down
 	__asm__ ("inc %v", _en_x);
 	__asm__ ("lda %v", _en_x);
 	__asm__ ("clc");
 	__asm__ ("adc #15");

enems_8way_fanties_horz_cx_set:
 	__asm__ ("lsr a");
 	__asm__ ("lsr a");
 	__asm__ ("lsr a");
 	__asm__ ("lsr a");
 	__asm__ ("sta %v", cx1);
 	__asm__ ("sta %v", cx2);

 	cm_two_points ();

 	__asm__ ("lda %v", at1);
 	__asm__ ("and #9");
 	__asm__ ("bne %g", enems_8way_fanties_horizontal_do);

 	__asm__ ("lda %v", at2);
 	__asm__ ("and #9");
 	__asm__ ("beq %g", enems_8way_fanties_horizontal_done);

enems_8way_fanties_horizontal_do:
 	__asm__ ("lda %v", rdx);
 	__asm__ ("sta %v", _en_x);
 	
enems_8way_fanties_horizontal_done:

enems_8way_fanties_done:

#ifdef FANTIES_ALT_ANIMATION
	__asm__ ("lda %v", frame_counter);
	__asm__ ("lsr a");
	__asm__ ("lsr a");
	__asm__ ("and #1");
	__asm__ ("sta %v", en_fr);
#endif

#ifdef ENEMS_USE_MAPPED_SPRITESET
	__asm__ ("lda %v", _en_s);
	__asm__ ("clc");
	__asm__ ("adc %v", en_fr);
	__asm__ ("tay");
	__asm__ ("lda (%v), y", c_enems_ssmap);
	__asm__ ("sta %v", en_sprid);
#else
	en_sprid = c_enems_offset + _en_s + en_fr;
#endif

