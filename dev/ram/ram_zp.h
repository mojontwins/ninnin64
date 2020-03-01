// RAM (ZEROPAGE)

unsigned char i, gpit, gpjt, gpaux;
unsigned char pad, pad_this_frame;
unsigned char half_life, game_over;
unsigned char frame_counter;

unsigned char n_pant, on_pant, level;

unsigned char *gp_gen;
const unsigned char *gp_map;
unsigned char *ul;
unsigned int gp_addr;
unsigned char rdx, rdy, rdt, rdct, rdi;
unsigned char rda, rdb, rdc, rdd, rdm, rdn;
unsigned char rlem, rleb, rler;

unsigned char olife, okeys, oobjs, oammo, okilled;

unsigned char cx1, cx2, cy1, cy2, t1, t2, at1, at2;

// Enemies
unsigned char _en_x, _en_y;
signed char _en_mx, _en_my;
unsigned char _en_touched, _en_cttouched;
unsigned char _en_t, _en_s, _en_ct, _en_maxct;

unsigned char en_x [NENEMS], en_y [NENEMS];
unsigned char en_x1 [NENEMS], en_y1 [NENEMS];
unsigned char en_x2 [NENEMS], en_y2 [NENEMS];
signed char en_mx [NENEMS], en_my [NENEMS];
unsigned char en_status [NENEMS];
unsigned char en_t [NENEMS], en_s [NENEMS];
unsigned char en_life [NENEMS];
unsigned char en_ct [NENEMS];
unsigned char en_fr, en_sprid;
unsigned char en_collx, en_colly;
unsigned char touched;
unsigned char en_touched [NENEMS], en_cttouched [NENEMS];

// Main player
unsigned char vertical_engine_type;
signed int px, py;
#if FIXBITS == 6
	signed int pvx, pvy;
#else
	signed char pvx, pvy;
#endif
unsigned char pfacing, pfr, pctfr, psprid;
unsigned char pj, pctj, pjb, pgotten, ppossee;
unsigned char pobjs, plife, pkeys, pkilled, pneedsreset;
unsigned char pammo, pfiring;
#if defined (PLAYER_PUSH_BOXES) && defined (FIRE_TO_PUSH)
	unsigned char pushed_any;
#endif
signed int pgtmx, pgtmy;

#ifdef PLAYER_ALTERED_HORIZONTAL_MOVEMENT
	#if FIXBITS == 6
		signed int player_ax, player_rx, player_vx_max;
	#else
		signed char player_ax, player_rx, player_vx_max;
	#endif
#endif

// Aux player
unsigned char prx, pry;
#ifdef PLAYER_MOGGY_STYLE
	unsigned char pfacingv, pfacingh;
#endif
unsigned char hitv, hith;

// Bullets
#ifdef PLAYER_CAN_FIRE
	unsigned char bst [MAX_BULLETS], bx [MAX_BULLETS], by [MAX_BULLETS], bmx [MAX_BULLETS], bmy [MAX_BULLETS];
	unsigned char btx, bty;
#endif

// Hotspots
unsigned char hrx, hry, hrt;

// Process breakable?
#ifdef BREAKABLE_ANIM
unsigned char do_process_breakable;
#endif

unsigned char ft;

// Gfx sets
unsigned char c_enems_offset;
unsigned char c_player_offset;

// Level control
unsigned char ts_offs, c_alt_bg;

// Current level
#ifdef MAP_IN_CHR_ROM
    const unsigned int *c_map;
    unsigned char c_map_chr_rom_bank;
#else
	#ifdef MAP_FORMAT_RLE44
		const unsigned char * const *c_map;	
	#else
		const unsigned char *c_map;
	#endif
#endif

const unsigned char *c_locks;

#ifdef COMPACTED_ENEMS
	const unsigned char * const *c_enems;
#else
	const unsigned char *c_enems;
#endif
const unsigned char *c_hotspots;
unsigned char c_map_w;

#ifdef MAPPED_TILESETS
	const unsigned char *c_tsmap;
#endif
#ifdef ENEMS_USE_MAPPED_SPRITESET
	const unsigned char *c_enems_ssmap;
#endif

unsigned char _x, _y, _t, _n;
unsigned char _x1, _y1, _x2, _y2;
unsigned char *_s;

#if defined (ACTIVATE_SCRIPTING) || defined (ACTIVATE_HOOKS)
	#ifdef ENABLE_FIRE_ZONE
		unsigned char f_zone_ac, fzx1, fzx2, fzy1, fzy2;
		unsigned char firezoned;
	#endif
#endif

#ifdef ENABLE_HITTER
	unsigned char hitter_on, hitter_x, hitter_y, hitter_frame;
	unsigned char hitter_hit_x, hitter_hit_y;
#endif

#ifdef ENABLE_COCOS
	unsigned char _coco_x, _coco_y;
	unsigned char coco_it;
#endif

