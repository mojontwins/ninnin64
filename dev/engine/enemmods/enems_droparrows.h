// MTE MK1_NES / AGNES 0.1 R
// Copyleft Mojon Twins 2013, 2019

if (!en_status [gpit] || half_life) {
		__asm__ ("ldx %v", gpit);

		__asm__ ("lda %v", _en_x);
		__asm__ ("clc");
		__asm__ ("adc %v", _en_mx);
		__asm__ ("sta %v", _en_x);

		__asm__ ("cmp %v, x", en_x2);
		__asm__ ("bne %g", enems_droparrow_horz_done);

	enems_droparrow_horz_reset:
		__asm__ ("lda %v, x", en_x1);
		__asm__ ("sta %v", _en_x);

	enems_droparrow_horz_done:

		__asm__ ("lda %v", _en_y);
		__asm__ ("clc");
		__asm__ ("adc %v", _en_my);
		__asm__ ("sta %v", _en_y);

		__asm__ ("cmp %v, x", en_y2);
		__asm__ ("bne %g", enems_droparrow_vert_done);

	enems_droparrow_vert_reset:
		__asm__ ("lda %v, x", en_y1);
		__asm__ ("sta %v", _en_y);

	enems_droparrow_vert_done:
		;
}

en_sprid = _en_s;
