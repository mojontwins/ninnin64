// MTE MK1_NES / AGNES 0.1 R
// Copyleft Mojon Twins 2013, 2019

// pres.h
// Game Over, Game Ending & other special screens

void cls (void) {
	vram_adr (0x2000); vram_fill (0, 1024);
}

void bat_in (void) {
	ppu_on_all ();
	fade_delay = 4;
	fade_in ();
}

void bat_out (void) {
	while (1) {
		pad_read ();
		if (pad_this_frame & (PAD_A|PAD_START|PAD_SELECT|PAD_B)) break;
	}
	
	music_stop ();
	fade_out ();
	ppu_off ();
}

void title (void) {
	pal_bg (mypal_game_bg0);
	un_rle_screen2 ((unsigned char *) rlets_0);
	pal_bright (0);

	PR_STR (10, 2, "MOJON TWINS\"");
	PR_STR (10, 23, "START=NORMAL%SELECT=KIDS!");
	PR_STR (29, 14, "V3");

	bat_in ();
	music_play (M_TITLE);
	bat_out ();
}

void fixed_screen (void) {
	pal_bg (mypal_game_bg0);
	pal_spr (mypal_game_fg0);
	if (rdn < 2) cls (); else un_rle_screen2 ((unsigned char *) rlets_1);
	switch (rdn) {
		case 0x00: 
			// New level screen:
			PR_STR (11, 14, (unsigned char *) (level_names + ((level << 3) + (level << 1) + level)));
			music_play (M_FIXED);
			break;

		case 0x01:
			// Game over screen:
			PR_STR (11, 13, "GAME OVER!");
			if (kids_mode) {
				PR_STR (9, 15, "START=CONTINUE");
			}
			music_play (M_GOVER);
			break;

		case 0x02: 
			// Game ending screen:
			PR_STR (7, 10, "  SORRY NINJAJAR%% BUT YER NOVIA IS%%IN ANOTHER CASTLE!%%     THE END?");
			oam_2x2_meta_spr (32, 183, 0, 1, 0x4c);
			oam_2x2_meta_spr (208, 183, 16, 0, 0x14);
			music_play (M_ENDING);
			break;

		case 0x03:
			// In another castle:			
			PR_STR (7, 10, "  SORRY NINJAJAR%% BUT YER NOVIA IS%%IN ANOTHER CASTLE!");
			oam_2x2_meta_spr (32, 183, 0, 1, 0x4c);
			oam_2x2_meta_spr (208, 183, 16, 0, 0x14);
			break;
	}

	bat_in ();
	bat_out ();

	oam_hide_rest (0);
}

/*
// Mojontwins logo
signed int lower_end;
void credits (void) {
	pal_bg (mypal_game_bg0);
	pal_spr (mypal_game_fg0);

	vram_adr (0x23c0);
	vram_fill (0x00,64);
		
	lower_end = 0; rdy = 240;

	PR_STR (9, 22, "CHERIL  PERILS");
	PR_STR (0, 24, "COPYLEFT 2013 BY THE MOJON TWINS%BY: ANJUEL - DAVIDIAN - NA-TH-AN%   NESLIB & SFX CODE BY SHIRU%    NOT LICENSED BY NINTENDO");
	
	pal_bright (0);
	ppu_on_all ();
	fade_delay = 4;
	fade_in ();
	sfx_play (7, 1);
	while (lower_end < 300) {
		pad_read (); if (pad_this_frame) break;
		oam_2x2_meta_spr (102, rdy,  0, 1, 240);
		oam_2x2_meta_spr (118, rdy, 16, 0, 244);
		oam_2x2_meta_spr (134, rdy, 32, 1, 248);
		if (rdy > 112) rdy --;
		ppu_waitnmi ();
		lower_end ++;
	};
	fade_out ();
	ppu_off ();
}
*/
