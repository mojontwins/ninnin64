// MTE MK1_NES / AGNES 0.1 R
// Copyleft Mojon Twins 2013, 2019

// general.h
// General functions, vars & buffers

void pad_read (void) {
	pad_this_frame = pad;
	pad = pad_poll (0);
	pad_this_frame = (pad_this_frame ^ pad) & pad;
}

unsigned char attr (signed char x, signed char y) {
	_x = x; _y = y;
	if (_x > 15 || _y > 11) return 0;
	return map_attr [COORDS(_x,_y)];
}

unsigned char qtile (signed char x, signed char y) {
	return map_buff [COORDS(x,y)];
}

void map_set (void) {
	rdd = COORDS(_x,_y);
	map_buff [rdd] = _t;
	map_attr [rdd] = behs [_t];
	_x = _x << 1; _y = TOP_ADJUST + (_y << 1);
	update_list_tile ();
}

unsigned char collide_in (void) {
	//return (_x1 >= _x2 && _x1 <= _x2 + 15 && _y1 >= _y2 && _y1 <= _y2 + 15);	

	// _x1 >= _x2
		__asm__ ("lda %v", _x1);
		__asm__ ("cmp %v", _x2);
		__asm__ ("bcc %g", collide_in_no);

	// _x1 <= _x2 + 15 -> _x2 + 15 >= _x1
		__asm__ ("lda %v", _x2);
		__asm__ ("clc");
		__asm__ ("adc #15");
		__asm__ ("cmp %v", _x1);
		__asm__ ("bcc %g", collide_in_no);

	// _y1 >= _y2
		__asm__ ("lda %v", _y1);
		__asm__ ("cmp %v", _y2);
		__asm__ ("bcc %g", collide_in_no);

	// _y1 <= _y2 + 15 -> _y2 + 15 >= _y1
		__asm__ ("lda %v", _y2);
		__asm__ ("clc");
		__asm__ ("adc #15");
		__asm__ ("cmp %v", _y1);
		__asm__ ("bcc %g", collide_in_no);

		return 1;

	collide_in_no:
	
		return 0;
}

unsigned char collide (void) {
	//return (_x2 <= _x1 + 7 && _x2 + 7 >= _x1 && _y2 <= _y1 + 7 && _y2 + 7 >= _y1 );
	//return (_x1 + 7 >= _x2 && _x2 + 7 >= _x1 && _y1 + 7 >= _y2 && _y2 + 7 >= _y1);
	
		__asm__ ("ldx #0");

	// _x2 <= _x1 + 7 -> _x1 + 7 >= _x2		
		__asm__ ("lda %v", _x1);
		__asm__ ("clc");
		__asm__ ("adc #7");
		__asm__ ("cmp %v", _x2);
		__asm__ ("bcc %g", collide_no);

	// _x2 + 7 >= _x1
		__asm__ ("lda %v", _x2);
		__asm__ ("clc");
		__asm__ ("adc #7");
		__asm__ ("cmp %v", _x1);
		__asm__ ("bcc %g", collide_no);

	// _y2 <= _y1 + 7 -> _y1 + 7 >= _y2		
		__asm__ ("lda %v", _y1);
		__asm__ ("clc");
		__asm__ ("adc #7");
		__asm__ ("cmp %v", _y2);
		__asm__ ("bcc %g", collide_no);

	// _y2 + 7 >= _y1
		__asm__ ("lda %v", _y2);
		__asm__ ("clc");
		__asm__ ("adc #7");
		__asm__ ("cmp %v", _y1);
		__asm__ ("bcc %g", collide_no);

		return 1;

	collide_no:
		return 0;	
}

#if FIXBITS == 6
signed int add_sign (signed int sign, signed int value) 
#else
signed char add_sign (signed char sign, signed char value)
#endif
{
	return sign == 0 ? 0 : sign < 0 ? -value : value;
}

#ifdef ACTIVATE_SCRIPTING
	void run_fire_script (void) {
		script = (unsigned char *) f_scripts [MAP_W * MAP_H];
		run_script ();
		script = (unsigned char *) f_scripts [n_pant];
		run_script ();
	}
#endif

void cm_two_points (void) {
	if (cy1 > 12 || cy2 > 12) cy2 = 0; 
	else if (cy1 > 11 || cy2 > 11) { at1 = at2 = 0; return; }
	/*
	rda = COORDS(cx1,cy1); t1 = map_buff [rda]; at1 = map_attr [rda];
	rda = COORDS(cx2,cy2); t2 = map_buff [rda]; at2 = map_attr [rda];
	*/
		__asm__ ("lda %v", cy1);
		__asm__ ("asl a");
		__asm__ ("asl a");
		__asm__ ("asl a");
		__asm__ ("asl a");
		__asm__ ("ora %v", cx1);
		__asm__ ("tax");
		__asm__ ("lda %v, x", map_buff);
		__asm__ ("sta %v", t1);
		__asm__ ("lda %v, x", map_attr);
		__asm__ ("sta %v", at1);

		__asm__ ("lda %v", cy2);
		__asm__ ("asl a");
		__asm__ ("asl a");
		__asm__ ("asl a");
		__asm__ ("asl a");
		__asm__ ("ora %v", cx2);
		__asm__ ("tax");
		__asm__ ("lda %v, x", map_buff);
		__asm__ ("sta %v", t2);
		__asm__ ("lda %v, x", map_attr);
		__asm__ ("sta %v", at2);		
}

void close_update_cycle (void) {
	//update_list [update_index] = 0xff;
		__asm__ ("ldx %v", update_index);
		__asm__ ("lda #$ff");
		__asm__ ("sta %v, x", update_list);
	ppu_waitnmi ();
}

void attr_list_mod (const unsigned char *l) {
	gp_map = l;

		__asm__ ("ldy #0");
	attr_list_mod_loop:
		__asm__ ("lda (%v), y", gp_map);
		__asm__ ("cmp #$ff");
		__asm__ ("beq %g", attr_list_mod_done);
		__asm__ ("tax");
		__asm__ ("iny");
		__asm__ ("lda (%v), y", gp_map);
		__asm__ ("sta %v, x", map_attr);
		__asm__ ("iny");
		__asm__ ("jmp %g", attr_list_mod_loop);
	attr_list_mod_done:
		;
}
