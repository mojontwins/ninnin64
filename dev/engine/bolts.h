// MTE MK1_NES / AGNES 0.1 R
// Copyleft Mojon Twins 2013, 2019

// bolts.h
// Lock management

void bolts_load (void) {
	if (c_locks == 0) { c_bolts_max = 0;  return; }

		__asm__ ("ldx #0");
		__asm__ ("ldy #0");

	bolts_load_loop:
		__asm__ ("lda (%v), y", c_locks);
		__asm__ ("iny");
		__asm__ ("sta %v, x", lknp);

		__asm__ ("lda (%v), y", c_locks);
		__asm__ ("iny");
		__asm__ ("sta %v, x", lkyx);

		__asm__ ("lda #1");
		__asm__ ("sta %v, x", lkact);

		__asm__ ("inx");
		__asm__ ("cpx %v", c_bolts_max);
		__asm__ ("bne %g", bolts_load_loop);
}

void bolt_clear (void) {
		__asm__ ("ldx #0");

	bolt_clear_loop:
		__asm__ ("lda %v, x", lknp);
		__asm__ ("cmp %v", n_pant);
		__asm__ ("bne %g", bolt_clear_loop_continue);
		__asm__ ("lda %v, x", lkyx);
		__asm__ ("cmp %v", rdd);
		__asm__ ("bne %g", bolt_clear_loop_continue);

		__asm__ ("lda #0");
		__asm__ ("sta %v, x", lkact);

	bolt_clear_loop_continue:
		__asm__ ("inx");
		__asm__ ("cpx %v", c_bolts_max);
		__asm__ ("bne %g", bolt_clear_loop);
}
	