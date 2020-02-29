// MTE MK1_NES / AGNES 0.1 R
// Copyleft Mojon Twins 2013, 2019

#ifdef PLAYER_HAS_SWIM
	if (vertical_engine_type != ENGINE_TYPE_SWIM)
#endif
{
	if (_en_t == 4 && gpjt && !pgotten && !pj) {
		// Horizontal moving platforms
		if (_en_mx) {
			if (pry + 16 >= _en_y && pry + 12 <= _en_y) {
				pgotten = 1;
				pgtmx = _en_mx << (FIXBITS - en_status [gpit]);
				pry = _en_y - 16; py = pry << FIXBITS;							
			}
		}
		
		// Vertical moving platforms
		if (
			(_en_my < 0 && pry + 17 >= _en_y && pry + 12 <= _en_y) ||
			(_en_my > 0 && pry + 16 + _en_my >= _en_y && pry + 12 <= _en_y)
		) {
			pgotten = 1;
			pgtmy = _en_my << (FIXBITS - en_status [gpit]);
			pry = _en_y - 16; py = pry << FIXBITS;
			pvy = 0;
		}
	}
}
