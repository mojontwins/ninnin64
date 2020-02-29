// MTE MK1_NES / AGNES 0.1 R
// Copyleft Mojon Twins 2013, 2019

#ifdef MAP_IN_CHR_ROM
	const unsigned char l_map_chr_rom_bank [] = {
		MAP_00_CHRROM, MAP_01_CHRROM, MAP_04_CHRROM, MAP_03_CHRROM,
		MAP_02_CHRROM, MAP_05_CHRROM, MAP_06_CHRROM, MAP_07_CHRROM,
		MAP_08_CHRROM, MAP_09_CHRROM, MAP_0A_CHRROM, MAP_0B_CHRROM
	};

	const unsigned int * const l_map [] = {
		map_00_scr_offsets, map_01_scr_offsets, map_04_scr_offsets, map_03_scr_offsets,
		map_02_scr_offsets, map_05_scr_offsets, map_06_scr_offsets, map_07_scr_offsets,
		map_08_scr_offsets, map_09_scr_offsets, map_0A_scr_offsets, map_0B_scr_offsets,
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
	MAP_0_MAP_SIZE, MAP_1_MAP_SIZE, MAP_4_MAP_SIZE, MAP_3_MAP_SIZE, MAP_2_MAP_SIZE, MAP_5_MAP_SIZE, MAP_6_MAP_SIZE, MAP_7_MAP_SIZE, MAP_8_MAP_SIZE, MAP_9_MAP_SIZE, MAP_A_MAP_SIZE
};

const unsigned char l_map_w [] = {
	10, 8, 12, 8, 8, 12, 4, 4, 10, 1, 2
};

const unsigned char * const l_locks [] = {
	0, map_1_locks, 0, map_3_locks, map_2_locks, 0, map_6_locks, map_7_locks, 0, 0, 0
};

#ifdef COMPACTED_ENEMS
	const unsigned char * const * const l_enems [] = 
#else
	const unsigned char * const l_enems [] = 
#endif
{
	enems_0, enems_1, enems_4, enems_3, enems_2, enems_5, enems_6, enems_7, enems_8, enems_9, enems_A
};

const unsigned char * const l_hotspots [] = {
	hotspots_0, hotspots_1, hotspots_4, hotspots_3, hotspots_2, hotspots_5, hotspots_6, hotspots_7, hotspots_8, hotspots_9, hotspots_A
};

const unsigned char * const l_pal_bg [] = {
	mypal_game_bg0, mypal_game_bg1, mypal_game_bg4, mypal_game_bg3, mypal_game_bg2, mypal_game_bg5, mypal_game_bg3, mypal_game_bg6, mypal_game_bg7, mypal_game_bg1, mypal_game_bg8
};

const unsigned char * const l_pal_spr [] = {
	mypal_game_fg0, mypal_game_fg1, mypal_game_fg3, mypal_game_fg0, mypal_game_fg2, mypal_game_fg4, mypal_game_fg3, mypal_game_fg5, mypal_game_fg3, mypal_game_fg1, mypal_game_fg3 
};

#ifndef ONLY_ONE_MAIN
	const unsigned char l_player_offset [] = {
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	};
#endif

const unsigned char l_bolts_max [] = {
	N_BOLTS_0, N_BOLTS_1, N_BOLTS_4, N_BOLTS_3, N_BOLTS_2, N_BOLTS_5, N_BOLTS_6, N_BOLTS_7, N_BOLTS_8, N_BOLTS_9, N_BOLTS_A
};

#ifdef MAPPED_TILESETS
	const unsigned char * const l_tsmap [] = {
		tsmap0, tsmap1, tsmap4, tsmap3, tsmap2, tsmap0, tsmap3, tsmap5, tsmap6, tsmap7, tsmap7
	};
#endif

#ifdef ENEMS_USE_MAPPED_SPRITESET
	const unsigned char * const l_enems_ssmap [] = {
		enems_ssmap0, enems_ssmap1, enems_ssmap4, enems_ssmap3, enems_ssmap2, enems_ssmap0, enems_ssmap3, enems_ssmap5, enems_ssmap4, enems_ssmap1, enems_ssmap2
	};
#else
	const unsigned char l_enems_offset [] = {
		16, 24, 16
	};
#endif

const unsigned char l_alt_bg [] = {
	42, 34, 34, 42, 42, 42, 42, 0, 34, 0, 0
};

const unsigned char l_scr_ini [] = {
	0, 14, 0, 7, 8, 0, 1, 20, 0, 0, 18
};

const unsigned char l_ini_x [] = {
	2, 7, 2, 2, 2, 2, 7, 2, 1, 1, 12
};

const unsigned char l_ini_y [] = {
	9, 9, 8, 6, 9, 9, 8, 9, 10, 9, 7
};

#if defined (ENEMS_CAN_DIE) && defined (NEED_BADDIES_COUNT)
	const unsigned char l_baddies_count [] = {
		KILLABLE_ENEMS_0, KILLABLE_ENEMS_1, KILLABLE_ENEMS_4, KILLABLE_ENEMS_3, KILLABLE_ENEMS_2, KILLABLE_ENEMS_5, KILLABLE_ENEMS_6, KILLABLE_ENEMS_7, KILLABLE_ENEMS_8, KILLABLE_ENEMS_9, KILLABLE_ENEMS_A
	};
#endif

const unsigned char l_pfacing [] = {
	1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 0
};

const unsigned char l_ppu_mask [] = {
	0x1e, 0x1e, 0x5e, 0x1e, 0x9e, 0x9e, 0x9e, 0x3e, 0x3e, 0x5e, 0x5e
};
