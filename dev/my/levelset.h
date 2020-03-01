// MTE MK1_NES / AGNES 0.1 R
// Copyleft Mojon Twins 2013, 2019

#ifdef MAP_IN_CHR_ROM
	const unsigned char l_map_chr_rom_bank [] = {
		MAP_00_CHRROM, MAP_01_CHRROM, MAP_04_CHRROM, MAP_03_CHRROM,
		MAP_02_CHRROM, MAP_05_CHRROM, MAP_09_CHRROM, MAP_06_CHRROM,
		MAP_08_CHRROM, MAP_0A_CHRROM, MAP_0B_CHRROM, MAP_07_CHRROM
	};

	const unsigned int * const l_map [] = {
		map_00_scr_offsets, map_01_scr_offsets, map_04_scr_offsets, map_03_scr_offsets,
		map_02_scr_offsets, map_05_scr_offsets, map_09_scr_offsets, map_06_scr_offsets,
		map_08_scr_offsets, map_0a_scr_offsets, map_0b_scr_offsets, map_07_scr_offsets,
	};
#else
	#ifdef MAP_FORMAT_RLE44
		const unsigned char * const * const l_map [] = 
	#else
		const unsigned char * const l_map [] = 
	#endif
		{ map_0, map_1, map_4, map_3, map_2, map_5, map_6, map_7, map_8, map_9, map_A };
#endif

const unsigned char l_map_size [] = {
	MAP_00_MAXPANTS, MAP_01_MAXPANTS, MAP_04_MAXPANTS, MAP_03_MAXPANTS,
	MAP_02_MAXPANTS, MAP_05_MAXPANTS, MAP_09_MAXPANTS, MAP_06_MAXPANTS,
	MAP_08_MAXPANTS, MAP_0A_MAXPANTS, MAP_0B_MAXPANTS, MAP_07_MAXPANTS
};

const unsigned char l_map_w [] = {
	MAP_00_W, MAP_01_W, MAP_04_W, MAP_03_W,
	MAP_02_W, MAP_05_W, MAP_09_W, MAP_06_W,
	MAP_08_W, MAP_0A_W, MAP_0B_W, MAP_07_W
};

const unsigned char * const l_locks [] = {
	0, map_01_locks, 0, map_03_locks, 
	map_02_locks, 0, 0, map_06_locks,
	0, 0, 0, map_07_locks
};

#ifdef COMPACTED_ENEMS
	const unsigned char * const * const l_enems [] = 
#else
	const unsigned char * const l_enems [] = 
#endif
{
	enems_0, enems_1, enems_4, enems_3, 
	enems_2, enems_5, enems_9, enems_6, 
	enems_8, enems_A, enems_B, enems_7
};

const unsigned char * const l_hotspots [] = {
	hotspots_0, hotspots_1, hotspots_4, hotspots_3, 
	hotspots_2, hotspots_5, hotspots_9, hotspots_6, 
	hotspots_8, hotspots_A, hotspots_B, hotspots_7
};

const unsigned char * const l_pal_bg [] = {
	mypal_game_bg0, mypal_game_bg1, mypal_game_bg4, mypal_game_bg3, 
	mypal_game_bg2, mypal_game_bg5, mypal_game_bg1, mypal_game_bg3, 
	mypal_game_bg7, mypal_game_bg8, mypal_game_bg9, mypal_game_bg6
};

const unsigned char * const l_pal_spr [] = {
	mypal_game_fg0, mypal_game_fg1, mypal_game_fg3, mypal_game_fg0, 
	mypal_game_fg2, mypal_game_fg4, mypal_game_fg1, mypal_game_fg3, 
	mypal_game_fg3, mypal_game_fg3, mypal_game_fg3, mypal_game_fg5
};

#ifndef ONLY_ONE_MAIN
	const unsigned char l_player_offset [] = {
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	};
#endif

const unsigned char l_bolts_max [] = {
	MAP_00_N_LOCKS, MAP_01_N_LOCKS, MAP_04_N_LOCKS, MAP_03_N_LOCKS, 
	MAP_02_N_LOCKS, MAP_05_N_LOCKS, MAP_09_N_LOCKS, MAP_06_N_LOCKS, 
	MAP_08_N_LOCKS, MAP_0A_N_LOCKS, MAP_0B_N_LOCKS, MAP_07_N_LOCKS
};

#ifdef MAPPED_TILESETS
	const unsigned char * const l_tsmap [] = {
		tsmap0, tsmap1, tsmap4, tsmap3, 
		tsmap2, tsmap0, tsmap7, tsmap3, 
		tsmap6, tsmap7, tsmap8, tsmap5
	};
#endif

#ifdef ENEMS_USE_MAPPED_SPRITESET
	const unsigned char * const l_enems_ssmap [] = {
		enems_ssmap0, enems_ssmap1, enems_ssmap4, enems_ssmap3, 
		enems_ssmap2, enems_ssmap0, enems_ssmap1, enems_ssmap3, 
		enems_ssmap4, enems_ssmap2, enems_ssmap2, enems_ssmap5
	};
#else
	const unsigned char l_enems_offset [] = {
		16, 24, 16
	};
#endif

const unsigned char l_alt_bg [] = {
	42, 34, 34, 42, 
	42, 42, 0, 42,
	34, 0, 34, 0
};

const unsigned char l_scr_ini [] = {
	0, 14, 0, 7, 
	8, 0, 0, 1,
	0, 18, 0, 20
};

const unsigned char l_ini_x [] = {
	2, 7, 2, 2, 
	2, 2, 1, 7, 
	1, 12, 2, 2
};

const unsigned char l_ini_y [] = {
	9, 9, 8, 6,
	9, 9, 9, 8, 
	7, 7, 6, 9
};

#if defined (ENEMS_CAN_DIE) && defined (NEED_BADDIES_COUNT)
	const unsigned char l_baddies_count [] = {
		KILLABLE_ENEMS_0, KILLABLE_ENEMS_1, KILLABLE_ENEMS_4, KILLABLE_ENEMS_3, 
		KILLABLE_ENEMS_2, KILLABLE_ENEMS_5, KILLABLE_ENEMS_9, KILLABLE_ENEMS_6, 
		KILLABLE_ENEMS_8, KILLABLE_ENEMS_A, KILLABLE_ENEMS_B, KILLABLE_ENEMS_7, 
	};
#endif

const unsigned char l_pfacing [] = {
	1, 0, 1, 1, 
	1, 1, 1, 0, 
	1, 1, 1, 1
};

const unsigned char l_ppu_mask [] = {
	0x1e, 0x1e, 0x5e, 0x1e, 
	0x9e, 0x9e, 0x5e, 0x9e,
	0x3e, 0x5e, 0x5e, 0x3e
};

const unsigned char l_music [] = {
	2, 4, 2, 5, 
	3, 4, 6, 5,
	3, 6, 2, 7
};
