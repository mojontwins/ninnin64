// MTE MK1_NES / AGNES 0.1 R
// Copyleft Mojon Twins 2013, 2019

// ============================================================================
// I. General configuration
// ============================================================================

#define MAP_W                   5       // General map size WxH, if multi-level
#define MAP_H                   5       // W*H = # of rooms in yr bigger level
#define SCR_INI                 20      // (legacy) first screen
#define PLAYER_INI_X            2       // (legacy)
#define PLAYER_INI_Y            4       // (legacy) starting coordinates
//#define PLAYER_MAX_OBJECTS    1       // Ifdef, level ends collecting N objs
#define PLAYER_LIFE             10      // player life
#define PLAYER_DONT_LIMIT_LIFE          // Don't limit max life to PLAYER_LIFE
#define PLAYER_REFILL           1       // amount of life refill with hs type 3
#define HOTSPOTS_TILE_BASE      42      // tile # with gfx for the "obj" - 1
#define MAX_LEVELS              12      // (new) Max # of levels
#define ONLY_ONE_MAIN                   // Only one character main sprite.

// ============================================================================
// II. Engine type
// ============================================================================

// General directives:
// -------------------

//#define PLAYER_BOUNCES                // Collide w/enems make player bounce
#define FULL_BOUNCE                     // Bounce is vx = PLAYER_VX_MAX, or
//#define DOUBLE_BOUNCE                 // vx = PLAYER_VX_MAX*2, choose one.

#define DIE_AND_RESPAWN                 // Dying = respawn on latest safe.
#define PLAYER_FLICKERS                 // Player flickers after respawn

//#define DEACTIVATE_KEYS               // Exclude code for keys and locks
#define LOCK_TILE                       15
//#define DEACTIVATE_OBJECTS            // Exclude code for collectibles

// Special background stuff

//#define PLAYER_PUSH_BOXES             // If defined, tile #14 is pushable
//#define FIRE_TO_PUSH                  // Player has to press B to push
#define PUSHABLE_TILE                   14

#define ENABLE_CONVEYORS                // Tile beh &= 0x20
#define ENABLE_TILE_GET                 // Tile beh == 0x80
#define ENABLE_QUICKSANDS               // Tile beh == 0x02
#define ENABLE_SLIPPERY                 // Tile beh &= 0x40

// Stuff to configure enemies

#define COMPACTED_ENEMS                 // Compacted enems (eneexp3c).
//#define WALLS_STOP_ENEMIES            // Enems react to the scenary
#define ENEMS_LIFE_GAUGE        1       // Hits needed to kill enemies.
//#define PERSISTENT_ENEMIES
#define ENEMS_WITH_FACING               // Enems face left/right
#define ENEMS_USE_MAPPED_SPRITESET      // Spriteset in my/enems_ssmap.h

#define ENABLE_COCOS                    // Some enemies may fire cocos
#define COCOS_V                 4
#define COCOS_MAX               3
#define COCOS_PATTERN           0xfa
#define COCOS_PALETTE           3

#define ENABLE_HORZ_SIGHTSHOOTERS       // T = 0x1X, with X = 1..3
#define ENABLE_VERT_SIGHTSHOOTERS       // T = 0x2X, with X = 1..3
#define ENABLE_TIMED_SHOOTERS           // T = 0xYX, with Y = 3..6, X = 1..3
#define ENABLE_GYROSHOOTER              // T = 0x7X, with X = 1..3

#define ENEM_COCOS_FREQ         150     // Time between shoots

// Type 6s, in this engine, are simple 8-way linear fanties.

#define ENABLE_8WAY_FANTIES
#define FANTIES_CELL            3       // Use this sprite (1-4).
//#define FANTY_WITH_FACING             // Fanties face left/right
#define FANTIES_ALT_ANIMATION           // Use frame_counter for animation
#define FANTIES_LIFE            3       // Use for different life gauge.

// Type 7 enemies are quite simple in sideview. 

#define ENABLE_PURSUERS                 // T = 7
#define PURSUERS_CELL           3       // Use this sprite (1-4).

// This parameters only apply to top-view type 7s.

#define DEATH_COUNT_EXPRESSION  50+(rand8()&63)

// Type 8 enemies are arrows. They are shot from x1,y1 to x2,y2 

//#define ENABLE_ARROWS
#define ARROWS_BASE_SPRITE      99      // This value is absolute
#define KILLABLE_ARROW                  // Arrow can be killed

// Type 9 enemies are drops. They are shot from x1,y1 to x2,y2 

#define ENABLE_DROPS
#define DROPS_BASE_SPRITE       46      // This value is absolute
//#define KILLABLE_DROP                 // Drop can be killed

// What the player can do:

//#define NEED_BADDIES_COUNT            // You need the # of baddies.

// Shooting

//#define PLAYER_CAN_FIRE               // Enables shooting engine
#define PLAYER_BULLET_SPEED     4       // Pixels/frame
#define MAX_BULLETS             4       // Max # of bullets on screen
#define PLAYER_BULLET_Y_OFFSET  6       // v offset from player's origin
#define PLAYER_BULLET_X_OFFSET  0       // h offset from player's edge
#define FIRE_MIN_KILLABLE       3       // Ifdef, only enems >= N can be killed
#define MAX_AMMO                99      // Ifdef, ammo is not infinite!
#define AMMO_REFILL             50      // Amount of ammo in refills
//#define INITIAL_AMMO      0           // Ifdefm ammo = X when entering game

#define ENABLE_HITTER                   // Enables the hitter (punches)
#define HITTER_Y_OFFS           6       // v offset from player's origin
#define HITTER_ACTIVE_FRAME_A   2       // This and the following will define
#define HITTER_ACTIVE_FRAME_B   3       // a range in which the hitter hits.

#define ENABLE_BREAKABLE                 // Breakable walls
#define BREAKABLES_SOFT                 // WALLS_LIFE == 1, not buffered
//#define ENABLE_BREAKABLE_LIFE  2       // Needs a buffer, disabled if SOFT
#define BREAKABLE_ANIM                  // Show explosion when breaking
#define BREAKABLE_MAX_FRAMES    12      // Frames to show explosion
#define BREAKABLE_BREAKING      46
#define BREAKABLE_ERASE         (c_tsmap [0])   
#define BREAKABLE_MAX           4       // Max # of tiles breaking at once

#define ENABLE_SILLY_SPRINGS
#define SPRING_SPIKE_TILE       12

// Scripting / Hooks
// -----------------

// Select scripting, hooks or neither; never BOTH!
//#define ACTIVATE_SCRIPTING            // Activates msc scripting
#define ACTIVATE_HOOKS                  // Add code to hooks.h

// This applies to both scripting and hooks
//#define FIRE_ON_KILL                  // run fire script on enemy kill
#define ENABLE_FIRE_ZONE                // Allows to def a zone which auto-
                                        // triggers "FIRE"

// Only for scripting
//#define ENABLE_EXTERN_CODE            // Enables custom code to be run from
                                        // the script using EXTERN n

// Top view:
// ---------
//#define PLAYER_MOGGY_STYLE            // Enable top view.
#define TOP_OVER_SIDE                   // UP/DOWN has priority over LEFT/RIGHT

// Side view:
// ----------

// You can enable several engine types. 
// Control which is active with vertical_engine_type

#define PLAYER_HAS_JUMP                 // Player is able to jump.
//#define PLAYER_HAS_JETPAC             // Player can thrust using a jetpac.
#define PLAYER_HAS_SWIM                 // Player badum badum badero

//#define PLAYER_STEPS_ON_ENEMS         // Stepping on enemies kills them
//#define STEP_MIN_KILLABLE     3       // Ifdef, only enems >= N can be killed

// ============================================================================
// III. Screen configuration
// ============================================================================

// You can prevent any stat to show in the hud by commenting its X coordinate

#define LIFE_X                  4       //
#define LIFE_Y                  26      // Life gauge counter
//#define OBJECTS_X             5       //
#define OBJECTS_Y               26      // Objects counter
#define KEYS_X                  28      //
#define KEYS_Y                  26      // Keys counter
//#define KILLED_X              16      //
#define KILLED_Y                26      // Kills counter
//#define AMMO_X                8       // 
#define AMMO_Y                  29      // Ammo counter

// Text, used from scripting
//#define LINE_OF_TEXT          26      // Ifdef, scripts can show text @ Y = #
#define LINE_OF_TEXT_X          1       // X coordinate.
#define LINE_OF_TEXT_ATTR       71      // Attribute

// Map format

#define MAPPED_TILESETS                 // Ifdef tileset is mapped
#define MAP_FORMAT_RLE44                // Use RLE44, undef for good ol'PACKED
#define MAP_CUSTOM_RENDERER             // Add code to calculate _t yourself!
#define MAP_IN_CHR_ROM                  // Map will be read from CHR-ROM
#define DEFAULT_CHR_ROM         0       // Where default game graphics are

// ============================================================================
// IV. Player movement configuration
// ============================================================================

// IV.1. Vertical movement. Only for side-view.

#if FIXBITS == 6
    // 1 = 1/64 of a pixel

    #define PLAYER_VY_FALLING_MAX   256
    #define PLAYER_G                16
    #define PLAYER_G_JUMPING        4

    #define PLAYER_VY_JUMP_INITIAL  256
    #define PLAYER_VY_JUMP_A_STEPS  64
    #define PLAYER_VY_JUMP_RELEASE  48
    
    #define PLAYER_AY_JETPAC        32
    #define PLAYER_VY_JETPAC_MAX    256

    #define PLAYER_AY_SWIM          8
    #define PLAYER_VY_SWIM_MAX      64

    // IV.2. Horizontal (side view) or general (top view) movement.

    #define PLAYER_VX_MAX           128
    #define PLAYER_AX               8
    #define PLAYER_RX               8

    #define PLAYER_AX_SLIPPERY      4 
    #define PLAYER_RX_SLIPPERY      2
    #define PLAYER_VX_MAX_SLIPPERY  192

    #define PLAYER_AX_QUICKSANDS    4
    #define PLAYER_RX_QUICKSANDS    32
    #define PLAYER_VX_MAX_QUICKSANDS 32

    #define PLAYER_VX_CONVEYOR      64

    #define PLAYER_VY_SINKING       2
#else
    // 1 = 1/16 of a pixel

    #define PLAYER_VY_FALLING_MAX   64 
    #define PLAYER_G                4
    #define PLAYER_G_JUMPING        1

    #define PLAYER_VY_JUMP_INITIAL  52
    #define PLAYER_VY_JUMP_RELEASE  24
    #define PLAYER_VY_JUMP_A_STEPS  16

    #define PLAYER_AY_JETPAC        8 
    #define PLAYER_VY_JETPAC_MAX    64 

    #define PLAYER_AY_SWIM          2
    #define PLAYER_VY_SWIM_MAX      16

    // IV.2. Horizontal (side view) or general (top view) movement.

    #define PLAYER_VX_MAX           32 
    #define PLAYER_AX               3 
    #define PLAYER_RX               4 
    
    #define PLAYER_AX_SLIPPERY      1
    #define PLAYER_RX_SLIPPERY      half_life
    #define PLAYER_VX_MAX_SLIPPERY  48

    #define PLAYER_AX_QUICKSANDS    1
    #define PLAYER_RX_QUICKSANDS    8
    #define PLAYER_VX_MAX_QUICKSANDS 8      

    #define PLAYER_VX_CONVEYOR      16

    #define PLAYER_VY_SINKING       1
#endif

// ============================================================================
// V. Tile behaviour
// ============================================================================

// == 0 : Walkable (no action)
//  & 1 : kills.
// == 2 : quicksands
//  & 4 : Platform (only stops player if falling on it)
//  & 8 : Full obstacle (blocks player from all directions)
// == 10: special obstacle (pushing blocks OR locks!)
// == 24: Breakable (8 | 16) (#ifdef ENABLE_BREAKABLE)
//  & 32: Conveyor (+0 = left, +1 = right)
//  & 64: ICE
// == 48: If feet touches, throw a spring!
//   128: Tile get
// You can add the numbers to get *some* combined behaviours

const unsigned char behs [] = {
    0, 0, 0, 0, 8, 8, 4, 8, 8, 8, 8, 8, 1,24,24,10,
    8, 0,40,41, 2,72, 8, 8, 8, 8, 1, 8, 1,24, 4, 0,
    0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8,128
};

// Autodefs. Don't touch these:

#if defined(PLAYER_CAN_FIRE) || defined (PLAYER_STEPS_ON_ENEMS) || defined (ENABLE_HITTER)
    #define ENEMS_CAN_DIE
#endif

#if defined (ENABLE_SLIPPERY) || defined (ENABLE_QUICKSANDS)
    #define PLAYER_ALTERED_HORIZONTAL_MOVEMENT
    #define _PLAYER_AX      player_ax
    #define _PLAYER_RX      player_rx
    #define _PLAYER_VX_MAX  player_vx_max
#else
    #define _PLAYER_AX      PLAYER_AX
    #define _PLAYER_RX      PLAYER_RX
    #define _PLAYER_VX_MAX  PLAYER_VX_MAX
#endif

#if defined (ENABLE_HORZ_SIGHTSHOOTERS) || defined (ENABLE_VERT_SIGHTSHOOTERS)
    #define ENABLE_ENEM_SUBTYPES
#endif

#ifndef DOUBLE_BOUNCE
    #define FULL_BOUNCE
#endif

#if ENABLE_BREAKABLE_LIFE == 1
    #define BREAKABLES_SOFT
#endif
