// rle44mapMK1(TM) map assets\mapa9.h
// Generated by rle44mapMK1.exe
// Copyleft 2015, 2017, 2018 The Mojon Twins

// Map Size Is 1x10
// Screen Size Is 16x12

// Compressed map structure, screens in RLE44, byte = NNNNRRRR, repeat R times tile #N
// Decos follow each screen, Format: [T N YX YX YX YX... (T < 128) | T YX (T >= 128)]

const unsigned char scr_9_00 [] = {0xf0, 0xf0, 0x08, 0xc0, 0x18, 0x05, 0x08, 0xa0, 0x08, 0x15, 0x00, 0x05, 0x18, 0x60, 0x18, 0x05, 0x30, 0x15, 0x28, 0x1b, 0x18, 0x15, 0x60, 0x25, 0x10, 0x15, 0xa0, 0x03, 0x00, 0x18, 0x50, 0x2d, 0x40, 0x15, 0x70, 0x0d, 0xe0, 0x05, 0x20, 0x05, 0x50, 0x05, 0x10, 0x19, 0x05, 0x29, 0x05, 0x59, 0x05, 0x10};
// Size = 50 bytes.
const unsigned char scr_9_01 [] = {0x10, 0x05, 0x20, 0x05, 0x90, 0x09, 0x03, 0x20, 0x03, 0xf0, 0xf0, 0x80, 0xa5, 0x70, 0x7e, 0x70, 0x95, 0xf0, 0x30, 0x05, 0xf0, 0xf0, 0xe0, 0x19, 0xc0};
// Size = 25 bytes.
const unsigned char scr_9_02 [] = {0x20, 0x03, 0x30, 0x03, 0x70, 0x05, 0x10, 0x0d, 0xe0, 0x0d, 0xe0, 0x0d, 0xe0, 0x05, 0xa0, 0x2d, 0x15, 0xf0, 0x20, 0x05, 0xe0, 0x05, 0xf0, 0xf0, 0x10, 0x05, 0xe0, 0x05, 0xe0, 0x05, 0x09, 0x20};
// Size = 32 bytes.
const unsigned char scr_9_03 [] = {0xa0, 0x05, 0xe0, 0x05, 0x09, 0xd0, 0x05, 0xe0, 0x05, 0x09, 0xd0, 0x05, 0xb0, 0x08, 0x10, 0x05, 0x09, 0xa0, 0x05, 0x10, 0x05, 0x80, 0x08, 0x10, 0x05, 0x10, 0x05, 0x09, 0x70, 0x05, 0x10, 0x05, 0x10, 0x05, 0x80, 0x05, 0x10, 0x05, 0xf0, 0x10, 0x19, 0x30, 0x29, 0xb0};
// Size = 44 bytes.
const unsigned char scr_9_04 [] = {0x30, 0x03, 0x10, 0x13, 0x10, 0x03, 0x40, 0x05, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0x90, 0x09, 0xf0, 0xe0, 0x09, 0xf0, 0xe0, 0x09, 0x20};
// Size = 23 bytes.
const unsigned char scr_9_05 [] = {0x10, 0x03, 0x10, 0x03, 0x20, 0x03, 0x00, 0x03, 0xf0, 0x05, 0xf0, 0xf0, 0xd0, 0x05, 0xf0, 0x80, 0x05, 0xf0, 0x20, 0x05, 0x70, 0x05, 0xf0, 0xf0, 0xd0, 0x09, 0xd0};
// Size = 27 bytes.
const unsigned char scr_9_06 [] = {0x30, 0x03, 0x10, 0x03, 0x10, 0x13, 0x30, 0x15, 0xf0, 0xf0, 0x00, 0x25, 0xf0, 0x30, 0x05, 0xf0, 0xf0, 0xb0, 0x05, 0xf0, 0x20, 0x05, 0xf0, 0xd0, 0x05, 0xf0, 0x05, 0x30};
// Size = 28 bytes.
// Screen 07 is a copy of screen 03.
const unsigned char scr_9_08 [] = {0x60, 0x03, 0x30, 0x03, 0x20, 0x15, 0x00, 0x05, 0xe0, 0x05, 0xe0, 0x05, 0xd0, 0x15, 0x10, 0x0d, 0x90, 0x0d, 0x30, 0x0e, 0x80, 0x25, 0x20, 0x25, 0xf0, 0x90, 0x05, 0x50, 0x05, 0xf0, 0xf0, 0xf0, 0x19, 0x20};
// Size = 34 bytes.
const unsigned char scr_9_09 [] = {0x20, 0x03, 0x80, 0x05, 0xc0, 0x25, 0xf0, 0xf0, 0xb0, 0x05, 0xf0, 0xf0, 0xc0, 0x08, 0x10, 0x0e, 0xb0, 0x05, 0x08, 0x2e, 0x80, 0x18, 0x15, 0x2e, 0x20, 0x28, 0x0a, 0x18, 0x15, 0x78, 0x25, 0x07, 0xb5};
// Size = 33 bytes.

// Compressed map array

const unsigned char * const map_9 [] = {
	scr_9_00, 
	scr_9_01, 
	scr_9_02, 
	scr_9_03, 
	scr_9_04, 
	scr_9_05, 
	scr_9_06, 
	scr_9_03, 
	scr_9_08, 
	scr_9_09
};

#define MAP_9_MAP_SIZE     10

// Total map size in bytes is 316

#define N_BOLTS_9 0