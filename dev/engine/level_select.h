// MTE MK1_NES / AGNES 0.1 R
// Copyleft Mojon Twins 2013, 2019

// 8 bits
/*
c_enems_offset = l_enems_offset [gpit];
c_player_offset = l_player_offset [gpit];
c_bolts_max = l_bolts_max [gpit];
c_alt_bg = l_alt_bg [gpit];

#ifdef ENEMS_CAN_DIE
	c_baddies_count = l_baddies_count [gpit];
#endif

#ifdef MAP_IN_CHR_ROM
	c_map_chr_rom_bank = l_map_chr_rom_bank [gpit];
#endif
*/

	__asm__ ("ldx %v", level);

#ifndef ENEMS_USE_MAPPED_SPRITESET
	__asm__ ("lda %v, x", l_enems_offset);
	__asm__ ("sta %v", c_enems_offset);
#endif
	
#ifndef ONLY_ONE_MAIN
	__asm__ ("lda %v, x", l_player_offset);
	__asm__ ("sta %v", c_player_offset);
#endif
	
	__asm__ ("lda %v, x", l_bolts_max);
	__asm__ ("sta %v", c_bolts_max);
	__asm__ ("lda %v, x", l_alt_bg);
	__asm__ ("sta %v", c_alt_bg);
	__asm__ ("lda %v, x", l_map_w);
	__asm__ ("sta %v", c_map_w);
	__asm__ ("lda %v, x", l_ppu_mask);
	__asm__ ("sta %v", c_ppu_mask);

#if defined (ENEMS_CAN_DIE) && defined (NEED_BADDIES_COUNT)
	__asm__ ("lda %v, x", l_baddies_count);
	__asm__ ("sta %v", c_baddies_count);
#endif

#if defined (MAP_IN_CHR_ROM)
	__asm__ ("lda %v, x", l_map_chr_rom_bank);
	__asm__ ("sta %v", c_map_chr_rom_bank);
#endif

// 16 bits
/*
c_map = l_map [gpit];
c_locks = l_locks [gpit];
c_enems = l_enems [gpit];
c_hotspots = l_hotspots [gpit];
c_tsmap = l_tsmap [gpit];
*/

	__asm__ ("lda %v", level);
	__asm__ ("asl a");
	__asm__ ("tay");

	__asm__ ("lda %v, y", l_map);
	__asm__ ("sta %v", c_map);
	__asm__ ("lda %v+1, y", l_map);
	__asm__ ("sta %v+1", c_map);

	__asm__ ("lda %v, y", l_locks);
	__asm__ ("sta %v", c_locks);
	__asm__ ("lda %v+1, y", l_locks);
	__asm__ ("sta %v+1", c_locks);

	__asm__ ("lda %v, y", l_enems);
	__asm__ ("sta %v", c_enems);
	__asm__ ("lda %v+1, y", l_enems);
	__asm__ ("sta %v+1", c_enems);

	__asm__ ("lda %v, y", l_hotspots);
	__asm__ ("sta %v", c_hotspots);
	__asm__ ("lda %v+1, y", l_hotspots);
	__asm__ ("sta %v+1", c_hotspots);

#ifdef MAPPED_TILESETS 
	__asm__ ("lda %v, y", l_tsmap);
	__asm__ ("sta %v", c_tsmap);
	__asm__ ("lda %v+1, y", l_tsmap);
	__asm__ ("sta %v+1", c_tsmap);
#endif

#ifdef ENEMS_USE_MAPPED_SPRITESET
	__asm__ ("lda %v, y", l_enems_ssmap);
	__asm__ ("sta %v", c_enems_ssmap);
	__asm__ ("lda %v+1, y", l_enems_ssmap);
	__asm__ ("sta %v+1", c_enems_ssmap);
#endif

/*
pal_bg (l_pal_bg [gpit]);
pal_spr (l_pal_spr [gpit]);
*/

	__asm__ ("lda %v, y", l_pal_bg);
	__asm__ ("ldx %v+1, y", l_pal_bg);
	__asm__ ("jsr %v", pal_bg);

	__asm__ ("lda %v", level);
	__asm__ ("asl a");
	__asm__ ("tay");
	
	__asm__ ("lda %v, y", l_pal_spr);
	__asm__ ("ldx %v+1, y", l_pal_spr);
	__asm__ ("jsr %v", pal_spr);
