// MTE MK1_NES / AGNES 0.1 R
// Copyleft Mojon Twins 2013, 2019

switch (en_alive [gpit]) {
	case 0:
		// IDLE
		if (en_ct [gpit]) {
			-- en_ct [gpit];
			_en_y = 240;
			en_sprid = 0xff;
		} else {
			en_alive [gpit] = 1;
			_en_x = en_x1 [gpit];
			_en_y = en_y1 [gpit];
			en_rawv [gpit] = 1 << (rand8 () % 5);
			if (en_rawv [gpit] > 4) en_rawv [gpit] = 1;
			if (en_rawv [gpit] == 1) en_status [gpit] = 1; else en_rawv [gpit] >>= 1;
			en_ct [gpit] = 50 + (rand8 () & 31);										
			en_sprid = EXPL_SPR;
		}
		break;
	case 1:
		// Appearing
		if (en_ct [gpit]) {
			-- en_ct [gpit];
		} else {
			en_alive [gpit] = 2;
		}
		break;
	case 2:
		// Pursuing
		if (pstate == EST_NORMAL && (!en_status [gpit] || half_life)) {
			_en_mx = add_sign (((prx >> 2) << 2) - _en_x, en_rawv [gpit]);
			_en_my = add_sign (((pry >> 2) << 2) - _en_y, en_rawv [gpit]);

			_en_y += _en_my;
			#ifdef WALLS_STOP_ENEMIES
				etx1 = (_en_x + 4) >> 4;
				etx2 = (_en_x + 11) >> 4;
				
				if (_en_my < 0) {
					ety1 = (_en_y + 8) >> 4;
					
					if (attr (etx1, ety1) || attr (etx2, ety1)) {
						_en_y = ((ety1 + 1) << 4) - 8;
					}	
				} else if (_en_my > 0) {
					ety1 = (_en_y + 15) >> 4;
					if (attr (etx1, ety1) || attr (etx2, ety1)) {
						_en_y = (ety1 - 1) << 4;
					}
				}
			#endif

			_en_x += _en_mx;
			#ifdef WALLS_STOP_ENEMIES
				// Collision detection
				ety1 = (_en_y + 8) >> 4;
				ety2 = (_en_y + 15) >> 4;
				
				if (_en_mx < 0) {
					etx1 = (_en_x + 4) >> 4;
					
					if (attr (etx1, ety1) || attr (etx1, ety2)) {
						_en_x = ((etx1 + 1) << 4) - 4;									
					}
				} else if (_en_mx > 0) {
					etx1 = (_en_x + 11) >> 4;									
					
					if (attr (etx1, ety1) || attr (etx1, ety2)) {
						_en_x = ((etx1 - 1) << 4) + 4;
					}	
				}
			#endif	
		}

		en_sprid = c_enems_offset + _en_s + en_fr;									
		break;
}					
