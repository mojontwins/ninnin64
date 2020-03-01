// MTE MK1_NES / AGNES 0.1 R
// Copyleft Mojon Twins 2013, 2019

// Custom renderer.

//_t = c_tsmap [_t];				// _t is transformed, rdt has original
	__asm__ ("ldy %v", _t);
	__asm__ ("lda (%v), y", c_tsmap);
	__asm__ ("sta %v", _t);

rdb = rand8 ();

// In level 6, paint rocks at the edges.
if (level == 6) {
	rda = rdm & 0xf; if (rda == 0 || rda == 0xf || (rda == 0xd && n_pant && n_pant < 9)) _t = 22;
}

if (rdt == 0) {
	if (rdm > 15) {
		// rda = map_buff [rdm - 16];
			__asm__ ("lda %v", rdm);
			__asm__ ("sec");
			__asm__ ("sbc #16");
			__asm__ ("tay");
			__asm__ ("lda %v, y", map_buff);
			__asm__ ("sta %v", rda);

		if (rda == 3 || rda == 17) {
			_t = rda;
			goto custom_renderer_done;
		} else if (rda == 22 || rda == 23) {
			if ((deco_flags & DECO_STALACTITES) && (rdb & 1)) _t = 9;
			goto custom_renderer_done;
		} else if (rda == 1) {
			_t = 2;
			goto custom_renderer_done;
		} else if (rda == 2 || rda == 16) {
			if ((rdb & 7) == 1) _t = 31;
			goto custom_renderer_done;
		}
	}
	if (rdm) {
		// rda = map_buff [rdm - 1];
			__asm__ ("lda %v", rdm);
			__asm__ ("sec");
			__asm__ ("sbc #1");
			__asm__ ("tay");
			__asm__ ("lda %v, y", map_buff);
			__asm__ ("sta %v", rda);
			
		if (rda == 37 || rda == 38) {
			// _t = rda + 1;
			// As we don't need rda anymore:
				__asm__ ("inc %v", rda);
				__asm__ ("lda %v", rda);
				__asm__ ("sta %v", _t);
				
			goto custom_renderer_done;
		}
	}
	if (c_alt_bg && (rdb & 0x1f) <2) {
		if (c_alt_bg == 34) { 
			// rda = rdm - 1;
				__asm__ ("lda %v", rdm);
				__asm__ ("sta %v", rda);
				__asm__ ("dec %v", rda);

			//if (map_buff [rda] == c_tsmap [0]) {
				__asm__ ("ldx %v", rda);
				__asm__ ("ldy #0");
				__asm__ ("lda %v, x", map_buff);
				__asm__ ("cmp (%v), y", c_tsmap);
				__asm__ ("bne %g", custom_renderer_done);
				
				// map_buff [rda] = 33;
				__asm__ ("lda #33");
				__asm__ ("sta %v, x", map_buff);

				// _t = 34;
				__asm__ ("lda #34");
				__asm__ ("sta %v", _t);
			//}
		} else _t = c_alt_bg;
	}
} else if (_t == 22) {
	// Random T or T + 1
	_t += (rdb & 1);
	if (rdm > 15) {
	// rda = map_buff [rdm - 16];
		__asm__ ("lda %v", rdm);
		__asm__ ("sec");
		__asm__ ("sbc #16");
		__asm__ ("tay");
		__asm__ ("lda %v, y", map_buff);
		__asm__ ("sta %v", rda);

		if (rda == 25) _t = 24;
	} 
} else if (_t == TILE_RAY) {
	++ ray_tiles;
} else if (_t == TILE_FACE) {
	++ face_tiles;
} 

custom_renderer_done:
