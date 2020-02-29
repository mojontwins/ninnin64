// MTE MK1_NES / AGNES 0.1 R
// Copyleft Mojon Twins 2013, 2019

// frame.h
// Custom game frame

unrle_vram (hud_rle, 0x2000);
if (kids_mode) {
	vram_adr (0x2000+26*32+1);
	VRAM_PUTN (62);
}
