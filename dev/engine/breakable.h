// MTE MK1_NES / AGNES 0.1 R
// Copyleft Mojon Twins 2013, 2019

// breakable.h
// Manages breakable blocks and animations

#ifdef BREAKABLE_ANIM
	void breakable_do_anim (void) {
		__asm__ ("lda #0");
		__asm__ ("sta %v", do_process_breakable);

		__asm__ ("ldx #0");
	
	breakable_do_anim_loop:
		__asm__ ("stx %v", gpit);

		__asm__ ("lda %v, x", brkf);
		__asm__ ("beq %g", breakable_do_anim_continue);

		__asm__ ("inc %v, x", brkf);
		__asm__ ("lda %v, x", brkf);
		__asm__ ("cmp #%b", BREAKABLE_MAX_FRAMES);
		__asm__ ("bne %g", breakable_do_anim_set_flag);

		__asm__ ("lda #0");
		__asm__ ("sta %v, x", brkf);

		__asm__ ("lda %v, x", brkx);
		__asm__ ("sta %v", _x);

		__asm__ ("lda %v, x", brky);
		__asm__ ("sta %v", _y);

		//__asm__ ("lda #%b", BREAKABLE_ERASE);
		//__asm__ ("sta %v", _t);
		_t = BREAKABLE_ERASE;

		#ifdef ACTIVATE_HOOKS
			on_tile_break ();
		#endif

		map_set ();
		__asm__ ("jmp %g", breakable_do_anim_continue);

	breakable_do_anim_set_flag:
		__asm__ ("lda #1");
		__asm__ ("sta %v", do_process_breakable);

	breakable_do_anim_continue:
		__asm__ ("ldx %v", gpit);
		__asm__ ("inx");
		__asm__ ("cpx #%b", BREAKABLE_MAX);
		__asm__ ("bne %g", breakable_do_anim_loop);
		
	}
#endif

void break_wall (void) {
	rdm = _x | (_y << 4);
	#ifndef BREAKABLES_SOFT	
		if (brk_buff [rdm] < BREAKABLE_LIFE) {
			++ brk_buff [rdm];
		} else 
	#endif
	{		
		#ifdef BREAKABLE_ANIM
			// Unsafe but short & fast. Adjust MAX_BREAKABLE if this breaks your game

				__asm__ ("ldx #0");

			break_wall_anim_create_loop:
				__asm__ ("stx %v", gpit);

				__asm__ ("lda %v, x", brkf);
				__asm__ ("bne %g", break_wall_anim_create_continue);

				__asm__ ("lda #1");
				__asm__ ("sta %v", do_process_breakable);
				__asm__ ("sta %v, x", brkf);
				
				__asm__ ("lda %v", _x);
				__asm__ ("sta %v, x", brkx);
				__asm__ ("asl a");
				__asm__ ("sta %v", _x);

				__asm__ ("lda %v", _y);
				__asm__ ("sta %v, x", brky);
				__asm__ ("asl a");
				__asm__ ("clc");
				__asm__ ("adc #%b", TOP_ADJUST);
				__asm__ ("sta %v", _y);

				__asm__ ("lda #%b", BREAKABLE_BREAKING);
				__asm__ ("sta %v", _t);
				
				update_list_tile ();
				map_attr [rdm] = 8;

				sfx_play (SFX_BREAKB, 1);
				__asm__ ("jmp %g", break_wall_anim_create_done);

			break_wall_anim_create_continue:
				__asm__ ("ldx %v", gpit);
				__asm__ ("inx");
				__asm__ ("cpx #%b", BREAKABLE_MAX);
				__asm__ ("bne %g", break_wall_anim_create_loop);

			break_wall_anim_create_done:
				;
		#else
			sfx_play (SFX_BREAKH, 1);
			_t = BREAKABLE_ERASE;
			#ifdef ACTIVATE_HOOKS
				on_tile_break ();
			#endif
			map_set ();		
		#endif		
	}
}
