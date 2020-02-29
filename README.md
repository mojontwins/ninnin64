# AGNES 0.1 R

Ahoy! There's nothing more confusing that the nomenclature and versioning of the libraries we have released, and what I'm about to introduce is one more entry in the list!

**MK1 NES / AGNES 0.1 R** is a "Revamp" or "Remake" of the original first version of our [MK1_NES / AGNES engine](https://github.com/mojontwins/MK1_NES). Yup, I know this sounds silly. The framework began in late 2013 rather shyly, and evolved slowly until it was discontinued in early 2016 and rebuilt, recoded, and heavily improved in 2018 and 2019. So what's this?

I was busy recovering old versions of abandoned games 'cause I like to keep stuff in good, not-quite-releaseable-but-maybe-showable state. And I came accross of the unfinished first version of Cheril Perils for NES, in fact I came accross several iterations of the game. I was cleaning them up and storing them nicely and easily buildable, with updated tools and converters, and I suddenly found myself correcting a couple of bugs, substituting some stuff... and things escalated quickly.

I basicly rewrote v0.1 of AGNES. Yup, that's it: I remade it so it was able to build the game using the same assets and configuration, but rewrote most of it from scratch - even rewrote some parts in pure assembly.

The results works nicely, is tiny, and quite fast.

And then I found myself making a new game with it, a version of Ninjajar! for my 5 years old son.

And then I was adding the missing stuff I needed to produce a Ninjajar! game to the engine. So there's extra stuff not present in the original 0.1, - and extra stuff which is wildly different or even doesn't exist in the latest version of AGNES.

Most of the time stuff which differs from the latest AGNES iteration is different because 0.1 is rather simple, and is built for tinyness, and stuff is a bit hacky. Many components and routines expect to be fed with "good data", for example.

Anyways, this is just a tiny doc so I don't forget how special stuff has to be done.

## Enemy types

The `enengine.h` module is quite limited, as it comes from the first version of AGNES, so most fancy enemy types are simply not there. This is what 0.1 had:

- Linear enemies. 
- Top-down pursuers with respawn (think Sgt. Helmet Training Day).

0.1 R also has:

### Sideview pursuers

Those just move horizontally to match the player's `prx` coodinate.

### Linear 8 way fanties

Those are pretty dumb fanties which move in the 8 integer directions with fixed speed which collide with the background. Note that, as most of AGNES 0.1 R, they are handled by quite simple (i.e. incomplete) code to save space. 

To place them in `ponedor.exe` use type 6, and speed 0, 1, 3, 7... for 1 pixel per frame, 1/2, 1/4, 1/8, etc.

## Enemy subtypes

Sideview pursuers are not very interesting unless they can shoot. AGNES 0.1 didn't implement simple cocos - 0.1 R implements *silly* cocos - this is, simple projectiles which can be fired in any of the 4 cardinal directions. No diagonals. No homing projectiles.

So now more stuff is stored in the pair of "type" and "attribute" values you can fiddle with with `ponedor.exe`: they are broken up in two nibbles each:

- Type byte: `AAAABBBB`, with `BBBB` = enemy type and `AAAA` = enemy subtype (shooting behaviour as of this version of the engine)
- Attr byte: `CCCCDDDD`, with `CCCC` = extra attribute and `DDDD` = primary attribute.

Note that using `AAAA` and `CCCC` = 0 the behaviour is 100% legacy and doesn't work old data. Enemies don't shoot and stuff. So let's check this stuff:

### `AAAA`, enemy subtype.

- 00: Normal, backwards compatible. Does nothing.
- 01: The enemies shoots horizontally if it 'sees' the player: if the player is in vertical range (+-16 pixels) and the enemy is facing the player.
- 02: Same, but vertically.
- 03, 04, 05, 06: Fixed, timed shooters, which shoot up, right, down and left, respectively.

The frequency of the shoots depends on the extra attribute:

### `CCCC`, extra attribute.

For shooters, defines frequency:

- 0000: the frequency is that defined at `config.h` in `ENEM_COCOS_FREQ`.
- Any non-zero value: frequency, in units of 16 frames.

### Learn by example.

Imagine you have your normal, all-0.1, plain linear enemy which wanders left and right, using type 2 and speed 1 (t = 0x02, a = 0x01). You can make it shoot the player. Imagine that you have set up your room so the player may jump over the enemy to cover a gap in a broken bridge. You want the enemy to shoot the player on sight from below! That's enemy subtype 2. So if you change t from 0x02 to 0x22, now your linear also shoots on sight. The extra attribute is still 0, so it uses the `ENEMS_COCO_FREQ` general frequency, but you still can change this! Make a fast shooter, for example changing a = 0x01 to 0x21, so a new coco is fired every 2*16=32 frames.

## Hooks

AGNES 0.1 implemented the msc engine so you could use scripting in your games (but was a bit buggy - that's why I ended up rewriting the WHOLE thing for AGNES 1.0, including the compiler). This allows you to do quite advanced stuff, but most of the time it is overkill as a couple of lines in C (or even assembly) can do the trick in much less space.

Scripting was quite useful in the original Speccy MK* engines, as you had little space for code, but a big bunch of RAM for data, so you could have an interpreter and all the compiled scripts as data. But it isn't quite as useful when you can have tons of readily available ROM space for both code and data.

Currently ANGES features a bunch of "code injection points", where you can add your code to define and/or modify how the engine behaves. For AGNES 0.1 R I have devised something quite simmiliar, but which works like the scripting system.

In `my/hooks.h` you have a collection of functions you can populate with your C or assembly code, which are called in the same spots as msc scripting sections are:

```c 
	void entering_game (void) {
		// Run at the beginning of the game
	}

	void entering_level (void) {
		// Run at the beginning of the level
	}

	void exiting_screen (void) {
		// Run right after the fade out, screen off.
	}

	void entering_screen (void) {
		// Run right before the screen on, fade in.
	}

	void press_fire_at_screen (void) {
		// Run when player presses B *or* enters a fire_zone.
		// Also run when player kills an enemy #ifdef FIRE_ON_KILL
	}

	void on_tile_break (void) {
		// Run when player breaks a tile
	}

	void on_tile_get (void) {
		// Run when player gets a "tile get" tile
	}

	void on_obj_got (void) {
		// Run when player gets a type 1 hotspot
	}
```

## Mapped graphics

AGNES features tile-mapped metatiles: each metatile is defined by 5 bytes: 4 pattern values and 1 palette value. In AGNES 0.1 metatile N was rendered using patterns `64+N*4` and the three following. Quite limited, but also quite compact, as you don't need to store metatile maps, just the palette value. So your metatileset just takes 48 bytes of ROM space.

The palette values for each metatile are stored in `assets/palettes.h` and are written there by `tileset_import_generator.exe`, which gets stuff from the `fiddle/` folder and generates data and `tileset_import.png`, the files needed by good ol' AGNES 0.1 - just check `prepare_fiddle.bat`.

Sprites are quite simmilar. You have just 64 16x16 sprite faces, but again, only 64 bytes of ROM space are spent to store palette data.

So mapped what? Well- you have 48 metatiles and 64 sprite cells, but levels only allow for 16 metatiles and 8 sprite cells (for enemies). The mapping is just selecting which 16 metatiles and which 8 sprite cells are in each level. Define your arrays at `my/tsmap.h` and `my/enems_ssmap.h` and populate the `l_tsmap` and `l_enems_ssmap` arrays at `my/levelset.h`.

## Stuff about map data

### `MAP_IN_CHR_ROM`

If enabled, maps are to be stored in CHR-ROM. This means some AGNES-1.0 type stuff must be performed to make the map binaries into CHR-ROM rather than PRG-ROM, and that which CHR-ROM page and the offset of each map should be provided, for each level.

First of all, `rle44mapchrrom.exe` should be used instead of `rle44mapMK1.exe`. A list of maps should be provided, usually `/map/maplist.txt`. 

### Silly springs

Throw a beh value of 48 to a cell `map_attr` and if the player touches that cell with his or her feet a tile will spawn in such cell - use it with a killing tile (spikes) for a great trap. 

### Quicksands

They work great.

### Slippery floor

