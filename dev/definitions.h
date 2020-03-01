// MTE MK1_NES / AGNES 0.1 R
// Copyleft Mojon Twins 2013, 2019

// definitions.h
// Main definitions

#define FIXBITS                 4   // 4 or 6
#define SPRITE_ADJUST           7
#define TOP_ADJUST              0

#define MAX_CERROJOS            16  // make it as small as possible.

#define NENEMS                  3   // Don't change wildly
#define EST_NORMAL              0
#define EST_FLICKERING          2
#define EST_MUR                 4
#define EST_PARALYZED           8
#define sgni(n)                 (n < 0 ? -1 : 1)
#define saturate(n)             (n < 0 ? 0 : n)
#define WTOP                    1
#define WBOTTOM                 2
#define WLEFT                   3
#define WRIGHT                  4
#define FACING_RIGHT            0
#define FACING_LEFT             2
#define FACING_UP               4
#define FACING_DOWN             6
#define CLS_CHAR                251
#define CLS_ATTR                0x55
#define NAMETABLE_A             0x2000
#define EXPL_SPR                47

// Macros 

#define MSB(x)                  (((x)>>8))
#define LSB(x)                  (((x)&0xff))
#define COORDS(x,y)             ((x)|((y)<<4))
#define PR_STR(x,y,s)           {_x=(x);_y=(y);_s=(s);pr_str();}
#define P_T(x,y,t)              {_x=(x);_y=(y);_t=(t);p_t();}
#define ADD_SIGN2(a,b,v)        ((a)==(b))?(0):(((a)>(b))?(v):(-(v)))
#define SC(x,y)                 {_x=(x);_y=(y);}
#define IF_COLLIDE(x1,y1,x2,y2)     _x1=(x1);_y1=(y1);_x2=(x2);_y2=(y2);if (collide())
#define IF_COLLIDE_IN(x1,y1,x2,y2)  _x1=(x1);_y1=(y1);_x2=(x2);_y2=(y2);if (collide_in())
#define VRAM_PUT(n)				{__asm__ ("lda %v", n); __asm__ ("sta $2007");}
#define VRAM_PUTN(n)			{__asm__ ("lda #%b", n); __asm__ ("sta $2007");}
#define VRAM_READ				(*((unsigned char *) (0x2007)))

#define M_TITLE                 0
#define M_GOVER                 8
#define M_FIXED 				1
#define M_ENDING                9

// SFX

#define SFX_START               0
#define SFX_TILE                1
#define SFX_OBJECT              2
#define SFX_USE                 3
#define SFX_TRAMPOLINE          3
#define SFX_PHIT                4
#define SFX_DUMMY1              5
#define SFX_ENHIT               6
#define SFX_DUMMY2              7
#define SFX_JUMP                8
#define SFX_BULLET              9
#define SFX_COCO                10
#define SFX_SPRING              11
#define SFX_COUNT               12
#define SFX_BREAKH              13
#define SFX_HITTER              14
#define SFX_STEPON              15
#define SFX_FLOAT               16
#define SFX_BREAKB              17
#define SFX_RING                18
#define SFX_FANFARE             19

// Vertical engine types

#define ENGINE_TYPE_JUMP        1
#define ENGINE_TYPE_JETPAC      2
#define ENGINE_TYPE_SWIM        3
