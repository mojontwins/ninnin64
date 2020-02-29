// MTE MK1 NES 0.1
// Copyleft Mojon Twins 2013

if (0 == (frame_counter & _en_mx)) {
	rdx = _en_x; rdy = _en_y;

	// Vertical
	
	if (_en_y != pry) {
		cx1 = _en_x >> 4;
		cx2 = (_en_x + 15) >> 4;

		if (_en_y > pry) {
			-- _en_y;
			cy1 = cy2 = _en_y >> 4;
		} else {
			++ _en_y;
			cy1 = cy2 = (_en_y + 15) >> 4;
		}

		cm_two_points ();
		if (at1 | at2) _en_y = rdy;
	} 

	// Horizontal

	if (_en_x != prx) {
		cy1 = _en_y >> 4;
		cy2 = (_en_y + 15) >> 4;

		if (_en_x > prx) {
			-- _en_x;
			cx1 = cx2 = _en_x >> 4;
			#ifdef FANTY_WITH_FACING
				en_sprid += 8;
			#endif
		} else {
			++ _en_x;
			cx1 = cx2 = (_en_x + 15) >> 4;
		}

		cm_two_points ();
		if (at1 | at2) _en_x = rdx;
	}
}

#ifdef FANTIES_ALT_ANIMATION
	en_fr = (frame_counter >> 2) & 1;
#endif

#ifdef ENEMS_USE_MAPPED_SPRITESET
	en_sprid = c_enems_ssmap [_en_s + en_fr];
#else
	en_sprid = c_enems_offset + _en_s + en_fr;
#endif
	