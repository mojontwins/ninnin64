// MTE MK1_NES / AGNES 0.1 R
// Copyleft Mojon Twins 2013, 2019

// Silly cocos

// 8 directions, clockwise, first is top.
const signed char _mx [] = { 0, COCOS_V, COCOS_V, COCOS_V, 0, -COCOS_V, -COCOS_V, -COCOS_V };
const signed char _my [] = { -COCOS_V, -COCOS_V, 0, COCOS_V, COCOS_V, COCOS_V, 0, -COCOS_V };

void cocos_init (void) {
		__asm__ ("ldx #0");

	cocos_init_loop:
		__asm__ ("txa");
		__asm__ ("sta %v, x", coco_slots);
		__asm__ ("lda #$ff");
		__asm__ ("sta %v, x", coco_y);
		__asm__ ("inx");
		__asm__ ("cpx #%b", COCOS_MAX);
		__asm__ ("bne %g", cocos_init_loop);

		__asm__ ("lda #%b", COCOS_MAX);
		__asm__ ("sta %v", coco_slots_i);
}

void cocos_add (void) {
	// _x, _y = where; _t = direction (0-7)

		__asm__ ("lda %v", coco_slots_i);
		__asm__ ("bne %g", cocos_add_do);
		__asm__ ("rts");

	cocos_add_do:
		__asm__ ("dec %v", coco_slots_i);
		__asm__ ("ldx %v", coco_slots_i);
		__asm__ ("lda %v, x", coco_slots);
		__asm__ ("tax");

		__asm__ ("lda %v", _x);
		__asm__ ("sta %v, x", coco_x);
		__asm__ ("lda %v", _y);
		__asm__ ("sta %v, x", coco_y);

		__asm__ ("ldy %v", _t);
		__asm__ ("lda %v, y", _mx);
		__asm__ ("sta %v, x", coco_mx);
		__asm__ ("lda %v, y", _my);
		__asm__ ("sta %v, x", coco_my);

	sfx_play (SFX_COCO, 1);
}

void cocos_do (void) {
		__asm__ ("ldx #0");

	cocos_do_loop:
		__asm__ ("stx %v", coco_it);
		
		// Coco is active if Y != 0xff
		__asm__ ("lda %v, x", coco_y);
		__asm__ ("cmp #$ff");
		__asm__ ("beq %g", cocos_do_nococo);

		// Move coco
		__asm__ ("lda %v, x", coco_x);
		__asm__ ("clc");
		__asm__ ("adc %v, x", coco_mx);
		__asm__ ("sta %v, x", coco_x);
		__asm__ ("sta %v", _coco_x);

		__asm__ ("lda %v, x", coco_y);
		__asm__ ("clc");
		__asm__ ("adc %v, x", coco_my);
		__asm__ ("sta %v, x", coco_y);
		__asm__ ("sta %v", _coco_y);

		// Paint coco
		oam_spr (
			_coco_x, _coco_y + SPRITE_ADJUST,
			COCOS_PATTERN, COCOS_PALETTE, 236 + (coco_it << 2)
		);

		IF_COLLIDE_IN ((_coco_x + 4), (_coco_y + 4), prx, pry) {
			if (kids_mode) {
				pstate = EST_PARALYZED;
				pctstate = 100;
			} else phit = 1;
			__asm__ ("ldx %v", coco_it);
			__asm__ ("jmp %g", cocos_do_destroy);
		}

		__asm__ ("ldx %v", coco_it);

		// Destroy coco?
		// Check 
		// X < COCOS_V ~ COCOS_V - 1 >= X
		__asm__ ("lda #%b", (COCOS_V - 1));
		__asm__ ("cmp %v", _coco_x);
		__asm__ ("bcs %g", cocos_do_destroy);

		// || X > 248 - COCOS_V ~ X >= 249 - COCOS_V
		__asm__ ("lda %v", _coco_x);
		__asm__ ("cmp #%w", (249 - COCOS_V));
		__asm__ ("bcs %g", cocos_do_destroy);

		// || Y < COCOS_V ~ COCOS_V - 1 >= Y
		__asm__ ("lda #%b", (COCOS_V - 1));
		__asm__ ("cmp %v", _coco_y);
		__asm__ ("bcs %g", cocos_do_destroy);

		// || Y > 184 - COCOS_V ~ Y >= 185 - COCOS_V
		__asm__ ("lda %v", _coco_y);
		__asm__ ("cmp #%w", (185 - COCOS_V));
		__asm__ ("bcs %g", cocos_do_destroy);
		
		// Collision with bg
		__asm__ ("lda %v", _coco_x);
		__asm__ ("lsr a");
		__asm__ ("lsr a");
		__asm__ ("lsr a");
		__asm__ ("lsr a");
		__asm__ ("sta %v", cx1);

		__asm__ ("lda %v", _coco_y);
		__asm__ ("and #$f0");
		__asm__ ("ora %v", cx1);

		__asm__ ("tay");
		__asm__ ("lda %v, y", map_attr);
		__asm__ ("and #8");
		__asm__ ("bne %g", cocos_do_destroy);

		__asm__ ("jmp %g", cocos_do_continue);

	cocos_do_nococo:
		oam_spr (
			0, 240,
			0xfa, 1, 236 + (coco_it << 2)
		);

		__asm__ ("ldx %v", coco_it);
		__asm__ ("jmp %g", cocos_do_continue);

	cocos_do_destroy:
		__asm__ ("lda #$ff");
		__asm__ ("sta %v, x", coco_y);
		__asm__ ("ldy %v", coco_slots_i);
		__asm__ ("txa");
		__asm__ ("sta %v, y", coco_slots);
		__asm__ ("inc %v", coco_slots_i);

	cocos_do_continue:
		__asm__ ("inx");
		__asm__ ("cpx #%b", COCOS_MAX);
		__asm__ ("bne %g", cocos_do_loop);
}
