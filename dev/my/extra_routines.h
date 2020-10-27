// MTE MK1_NES / AGNES 0.1 R
// Copyleft Mojon Twins 2013, 2019

// Extra routines.

// This code is executed right before waiting for VBLANK each game loop.

if (f1 && !paused) {
	// Lava!
	if (lava_ticker) -- lava_ticker; else {
		sfx_play (SFX_BREAKB, 0);

		lava_ticker = lava_delay;

		// Advance lava
		lava_height --;
		lava_calc ();

		if (lava_on_screen) lava_draw_buff ();		
	}

	// Make screen flicker
	rda = frame_counter & 0xf;
	scroll (jitter [rda], 240-8 + jitter [15-rda]);

	// Check!
	if (pry <= 176 && pry + scrpixoffset > lava_height_pixels - 12) {
		pstate = EST_NORMAL; phit = 1; pneedsreset = 1;
	}
}
