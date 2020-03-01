// MTE MK1_NES / AGNES 0.1 R
// Copyleft Mojon Twins 2013, 2019

// Add the vars & consts you need

unsigned char opcoins, pcoins;
unsigned char screen_clear_list [24]; 
unsigned char ray_tiles;
unsigned char face_tiles;
unsigned char face_count;
unsigned char face_cleared;

unsigned char f0, f1, f2;   // All purpose flags, used in hooks

#define MAP_CLEAR_LIST_ADDRESS  0x2C00

unsigned char deco_flags;
#define DECO_STALACTITES        1

// Special tile values
#define TILE_FENCE              10
#define TILE_SPIKES             12
#define TILE_CHAIN              17
#define TILE_RAY                27
#define TILE_FACE               36

// Attribute list modifier arrays

const unsigned char alm0 [] = { 0x34, 48, 0x38, 48, 0x3B, 48, 0x9E, 0, 0xAE, 0, 0xFF };
const unsigned char alm1 [] = { 0x57, 48, 0x54, 48, 0xff };

// Level names

const unsigned char level_names [] = 
	"THE BOSQUE\x00"
	"THE DESERT\x00"
	"GOOD MARSH\x00"
	"DE NADAL 1\x00"
	"CUEVA CAVE\x00"
	"NOSHE WOOD\x00"
	"LAVA PIT!!\x00"
	"DE NADAL 2\x00"
	"MARSHY GOO\x00"
	"HELL PIT!!\x00"
	"ICY DUSTER\x00"
	"EL CASTLER\x00";

// Smart gadget to make the lava level
// lava_top is 32 bytes to be copied to an update sequence.
// lava_body contains 64 bytes from which 32 will be copied to an
// update sequence, starting at (rand8() & 0x1f).

const unsigned char lava_top [] = "\xA8\xA9\xA8\xA9\xA8\xA9\xA8\xA9\xA8\xA9\xA8\xA9\xA8\xA9\xA8\xA9\xA8\xA9\xA8\xA9\xA8\xA9\xA8\xA9\xA8\xA9\xA8\xA9\xA8\xA9\xA8\xA9";
const unsigned char lava_body [] = "\xE4\xE5\xE6\xE7\xAA\xAB\xE5\xAB\xE6\xAA\xAB\xE4\xE5\xE6\xAB\xE5\xAA\xAB\xAA\xE6\xE5\xE4\xAB\xE6\xAA\xAB\xE5\xE4\xE6\xAA\xAB\xAA\xE5\xE6\xE4\xE5\xAB\xE6\xAA\xE5\xAB\xE4\xE6\xE5\xAB\xAA\xE5\xAB\xE4\xE6\xE5\xAA\xAB\xE5\xE4\xE6\xE5\xAA\xAB\xE5\xE4\xE6\xE5\xE4";

// Lava stuff

unsigned char lava_delay;
const signed char jitter [] = { -1, 1, 0, -1, 0, 1, -1, 0, -1, 0, 1, -1, 0, 1, 0, 1 };
unsigned char lava_ticker;
unsigned char lava_height;
unsigned char lava_cur_height;
unsigned char lava_on_screen;
unsigned int lava_height_pixels;

// Multiply by 32 LUT

const unsigned int shl5 [] = {
	0x0000, 0x0020, 0x0040, 0x0060, 0x0080, 0x00a0, 0x00c0, 0x00e0,
	0x0100, 0x0120, 0x0140, 0x0160, 0x0180, 0x01a0, 0x01c0, 0x01e0,
	0x0200, 0x0220, 0x0240, 0x0260, 0x0280, 0x02a0, 0x02c0, 0x02e0,
	0x0300, 0x0320, 0x0340, 0x0360, 0x0380, 0x03a0 //, 0x03c0, 0x03e0
};
