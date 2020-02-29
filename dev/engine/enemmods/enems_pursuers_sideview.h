// MTE MK1_NES / AGNES 0.1 R
// Copyleft Mojon Twins 2013, 2019

// Pursuers in side view are increibly simple

if (_en_x > prx) _en_x -= _en_mx;
else if (_en_x < prx) _en_x += _en_mx;
#ifdef ENEMS_USE_MAPPED_SPRITESET
	en_sprid = c_enems_ssmap [((PURSUERS_CELL - 1) << 1) + ((frame_counter >> 2) & 1)];
#else
	en_sprid = c_enems_offset + ((PURSUERS_CELL - 1) << 1) + ((frame_counter >> 2) & 1);
#endif
