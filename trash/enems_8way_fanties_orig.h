// MTE MK1 NES 0.1
// Copyleft Mojon Twins 2013

// Original (not used) implementation - Speeds work
// like linear enemies. As a side effect, this required you to place
// your 8way fanties in the ponedor.exe application this way:
// [6]
//    \
//     (1)

if (!en_status [gpit] || half_life) {
	rdx = _en_x; rdy = _en_y;

	// Vertical
	
	if (_en_y != pry) {
		cx1 = _en_x >> 4;
		cx2 = (_en_x + 15) >> 4;

		if (_en_y > pry) {
			_en_y -= _en_my;
			cy1 = cy2 = _en_y >> 4;
		} else {
			_en_y += _en_my;
			cy1 = cy2 = (_en_y + 15) >> 4;
		}

		cm_two_points ();
		if (at1 | at2) _en_y = rdy;
	} 

	// Horizontal

	#ifdef FANTIES_ALT_ANIMATION
		en_fr = (frame_counter >> 2) & 1;
	#endif

	#ifdef ENEMS_USE_MAPPED_SPRITESET
		en_sprid = c_enems_ssmap [_en_s + en_fr];
	#else
		en_sprid = c_enems_offset + _en_s + en_fr;
	#endif

	if (_en_x != prx) {
		cy1 = _en_y >> 4;
		cy2 = (_en_y + 15) >> 4;

		if (_en_x > prx) {
			_en_x -= _en_mx;
			cx1 = cx2 = _en_x >> 4;
			#ifdef FANTY_WITH_FACING
				en_sprid += 8;
			#endif
		} else {
			_en_x += _en_mx;
			cx1 = cx2 = (_en_x + 15) >> 4;
		}

		cm_two_points ();
		if (at1 | at2) _en_x = rdx;
	}
}
