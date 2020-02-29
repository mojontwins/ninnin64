// RAM (BSS)

// Update list
#define UPDATE_LIST_SIZE 30
unsigned char update_index;
unsigned char update_list [UPDATE_LIST_SIZE * 3];

unsigned char map_attr [192];
unsigned char map_buff [192];
unsigned char attr_table [64];
unsigned char kids_mode;

unsigned char c_ppu_mask;

unsigned char ppu_mask_xor_bits_fiddle;

#ifdef ENABLE_BREAKABLE
	#ifndef BREAKABLES_SOFT
		unsigned char brk_buff [192];
	#endif
	#ifdef BREAKABLE_ANIM
		unsigned char brkf [BREAKABLE_MAX];
		unsigned char brkx [BREAKABLE_MAX];
		unsigned char brky [BREAKABLE_MAX];
	#endif
#endif

unsigned char fade_delay;
unsigned char hyx [MAP_W * MAP_H];
unsigned char ht [MAP_W * MAP_H];
unsigned char hact [MAP_W * MAP_H];
unsigned int scrpixoffset;

#ifndef DEACTIVATE_KEYS
	unsigned char lkyx [MAX_CERROJOS], lknp [MAX_CERROJOS], lkact [MAX_CERROJOS];
	unsigned char yx;
#endif

#if defined (ACTIVATE_SCRIPTING) || defined (ACTIVATE_HOOKS)
	#ifdef ACTIVATE_SCRIPTING
		void msc_init_all (void);
		void run_script (void);
		unsigned char script_result;
		unsigned char *script;
	#else
		unsigned char win_game;
	#endif

	#define MAX_FLAGS 8
	unsigned char flags [MAX_FLAGS];
	unsigned char script_something_done;
#endif

// Baddies count. Properly fill this value!
#ifdef ENEMS_CAN_DIE
	unsigned char c_baddies_count;
	#define BADDIES_COUNT c_baddies_count
#endif

#ifdef PERSISTENT_ENEMIES
	// Persistent enemies
	unsigned char ep_x [NENEMS * MAP_W * MAP_H];
	unsigned char ep_y [NENEMS * MAP_W * MAP_H];
	signed char ep_mx [NENEMS * MAP_W * MAP_H];
	signed char ep_my [NENEMS * MAP_W * MAP_H];
	unsigned char ep_t [NENEMS * MAP_W * MAP_H];
	unsigned char ep_it;
#endif

// More enems
#if defined (ENABLE_PURSUERS) && defined (PLAYER_MOGGY_STYLE)
	unsigned char en_alive [NENEMS], en_rawv [NENEMS];
#endif
#if defined (ENABLE_ENEM_SUBTYPES)
	unsigned char en_maxct [NENEMS];
	#if defined (ENABLE_GYROSHOOTER)
		unsigned char en_cur_d [NENEMS];
	#endif
#endif

// Die & respawn
#ifdef DIE_AND_RESPAWN
	unsigned char n_pant_safe;
	signed int px_safe, py_safe;
	unsigned char psafe, pscreenissafe;
#endif

unsigned char c_bolts_max;

unsigned char paused;

#ifdef ENABLE_COCOS
	unsigned char coco_slots_i;
	unsigned char coco_slots [COCOS_MAX];
	unsigned char coco_x [COCOS_MAX], coco_y [COCOS_MAX];
	signed char coco_mx [COCOS_MAX], coco_my [COCOS_MAX];
#endif

// More player (mostly flags)

unsigned char pstate, pctstate;
unsigned char phit;

#ifdef PLAYER_ALTERED_HORIZONTAL_MOVEMENT
	#ifdef ENABLE_SLIPPERY
		unsigned char pslippery;
	#endif

	#ifdef ENABLE_QUICKSANDS
		unsigned char pquicksanded;
	#endif
#endif
