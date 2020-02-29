// MTE MK1_NES / AGNES 0.1 R
// Copyleft Mojon Twins 2013, 2019

if (!en_status [gpit] || half_life) {
	#ifdef WALLS_STOP_ENEMIES
		// Collision. Acts as "everything is a wall". Refine if needed for future games!
		// Revise this code to make it better when it's needed!
		
		en_colly = 0;
		if (_en_my) {
			cx1 = _en_x >> 4;
			cx2 = (_en_x + 15) >> 4;
			if (_en_my < 0) {
				cy1 = cy2 = (_en_y - 1) >> 4;	
			} else if (_en_my > 0) {
				cy1 = cy2 = (_en_y + 16) >> 4;
			}
			cm_two_points ();
			if (at1 || at2) en_colly = 1;
		}
		
		en_collx = 0;
		if (_en_mx) {
			cy1 = _en_y >> 4;
			cy2 = (_en_y + 15) >> 4;
			if (_en_mx < 0) {
				cx1 = cx2 = (_en_x - 1) >> 4;
			} else if (_en_mx > 0) {
				cx1 = cx2 = (_en_x + 16) >> 4;
			}
			cm_two_points ();
			if (at1 || at2) en_collx = 1;
		}
		
		_en_x += _en_mx;
		_en_y += _en_my;

		if (_en_x == en_x1 [gpit] || _en_x == en_x2 [gpit] || en_collx) _en_mx = -_en_mx;
		if (_en_y == en_y1 [gpit] || _en_y == en_y2 [gpit] || en_colly) _en_my = -_en_my;
	#else
		// No collision, very simple:

		/*
		_en_x += _en_mx;
		_en_y += _en_my;

		if (_en_x == en_x1 [gpit] || _en_x == en_x2 [gpit]) _en_mx = -_en_mx;
		if (_en_y == en_y1 [gpit] || _en_y == en_y2 [gpit]) _en_my = -_en_my;						
		*/

		__asm__ ("ldx %v", gpit);

		__asm__ ("lda %v", _en_x);
		__asm__ ("clc");
		__asm__ ("adc %v", _en_mx);
		__asm__ ("sta %v", _en_x);

		__asm__ ("cmp %v, x", en_x1);
		__asm__ ("beq %g", enems_lineal_horz_chgdir);

		__asm__ ("cmp %v, x", en_x2);
		__asm__ ("bne %g", enems_lineal_horz_done);

	enems_lineal_horz_chgdir:
		__asm__ ("lda #0");
		__asm__ ("sec");
		__asm__ ("sbc %v", _en_mx);
		__asm__ ("sta %v", _en_mx);

	enems_lineal_horz_done:

		__asm__ ("lda %v", _en_y);
		__asm__ ("clc");
		__asm__ ("adc %v", _en_my);
		__asm__ ("sta %v", _en_y);

		__asm__ ("cmp %v, x", en_y1);
		__asm__ ("beq %g", enems_lineal_vert_chgdir);

		__asm__ ("cmp %v, x", en_y2);
		__asm__ ("bne %g", enems_lineal_vert_done);

	enems_lineal_vert_chgdir:
		__asm__ ("lda #0");
		__asm__ ("sec");
		__asm__ ("sbc %v", _en_my);
		__asm__ ("sta %v", _en_my);		

	enems_lineal_vert_done:
		;
	#endif						
}

#ifdef ENEMS_USE_MAPPED_SPRITESET
	en_sprid = c_enems_ssmap [_en_s + en_fr];
#else
	en_sprid = c_enems_offset + _en_s + en_fr;
#endif
#ifdef ENEMS_WITH_FACING
	if (_en_mx < 0) en_sprid += 8;
	else if (_en_mx == 0 && _en_my < 0) en_sprid += 8;
#endif
