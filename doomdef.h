/* DoomDef.h */

#ifndef DOOMDEF_H__
#define DOOMDEF_H__

#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>

/* JAGUAR should be defined on the compiler command line for console builds */
/* if MARS isn't defined, assume jaguar version */

/*define	MARS */

#define LZSS_BUF_SIZE	0x1000

/* if rangecheck is undefined, most parameter validation debugging code */
/* will not be compiled */
#define	RANGECHECK

/* if SIMULATOR is defined, compile in extra code for things like console */
/* debugging aids */
#ifndef JAGUAR
#ifndef MARS
#define SIMULATOR
#endif
#endif

typedef unsigned short pixel_t;

#ifdef MARS
typedef unsigned char inpixel_t;
#else
typedef unsigned short inpixel_t;
#endif

/* the structure sizes are frozen at ints for jaguar asm code, but shrunk */
/* down for mars memory cramming */
#ifdef MARS
#define	VINT	short
#else
#define	VINT	int
#endif

#ifdef MARS
#define ATTR_DATA_OPTIMIZE_NONE __attribute__((section(".sdata"), aligned(16), optimize("O1")))
#define ATTR_DATA_CACHE_ALIGN __attribute__((section(".sdata"), aligned(16), optimize("Os")))
#define ATTR_OPTIMIZE_SIZE __attribute__((optimize("Os")))
#define ATTR_OPTIMIZE_EXTREME __attribute__((optimize("O3", "no-align-loops", "no-align-functions", "no-align-jumps", "no-align-labels")))
#else
#define ATTR_DATA_OPTIMIZE_NONE
#define ATTR_DATA_CACHE_ALIGN
#define ATTR_OPTIMIZE_SIZE
#define ATTR_OPTIMIZE_EXTREME
#endif

/*============================================================================= */

/* all external data is defined here */
#include "doomdata.h"

/* header generated by multigen utility */
#include "info.h"

#define D_MAXCHAR  ((char)0x7f)
#define D_MAXSHORT ((short)0x7fff)
#define D_MAXINT   ((int)0x7fffffff)  // max pos 32-bit int
#define D_MAXLONG  ((long)0x7fffffff)

#define D_MINCHAR  ((char)0x80)
#define D_MINSHORT ((short)0x8000)
#define D_MININT   ((int)0x80000000) // max negative 32-bit integer
#define D_MINLONG  ((long)0x80000000)

#ifndef NULL
#define	NULL	0
#endif

int D_vsnprintf(char *str, size_t nmax, const char *format, va_list ap) __attribute__((nonnull));
int D_snprintf(char *buf, size_t nsize, const char *fmt, ...) __attribute__((nonnull));
void D_printf (char *str, ...) __attribute__((nonnull));

void D_isort(int* a, int len) __attribute__((nonnull)) ATTR_DATA_CACHE_ALIGN;

/*
===============================================================================

						GLOBAL TYPES

===============================================================================
*/

#define MAXPLAYERS	2

#define TICRATE		15				/* number of tics / second */
#define TICVBLS		(60/TICRATE)	/* vblanks per tic */
									/* change this to 'ticrate' if you want */
									/* to use a different rate on PAL */

#define	FRACBITS		16
#define	FRACUNIT		(1<<FRACBITS)
typedef int fixed_t;

#ifdef MARS
#define THINKERS_30HZ
#endif

#ifdef THINKERS_30HZ
#define THINKERS_TICS 1
#else
#define THINKERS_TICS 2
#endif

#define	ANG45	0x20000000
#define	ANG90	0x40000000
#define	ANG180	0x80000000
#define	ANG270	0xc0000000
typedef unsigned angle_t;

#define	FINEANGLES			8192
#define	FINEMASK			(FINEANGLES-1)
#define	ANGLETOFINESHIFT	19	/* 0x100000000 to 0x2000 */

#ifdef MARS

fixed_t finesine(angle_t angle) ATTR_DATA_CACHE_ALIGN;
#define finecosine(angle) finesine((angle) + FINEANGLES / 4)

#else

extern	const fixed_t		finesine_[5*FINEANGLES/4];
extern	const fixed_t		*finecosine_;

#define finesine(x) finesine_[x]
#define finecosine(x) finecosine_[x]

#endif

typedef enum
{
	sk_baby,
	sk_easy,
	sk_medium,
	sk_hard,
	sk_nightmare
} skill_t;


typedef enum 
{
	ga_nothing, 
	ga_died,
	ga_completed,
	ga_secretexit,
	ga_warped,
	ga_exitdemo,
	ga_startnew
} gameaction_t;


/* */
/* library replacements */
/* */

#define D_abs(x) ((x < 0) ? -(x) : x)
void D_memset (void *dest, int val, size_t count) __attribute__((nonnull));
void D_memcpy (void *dest, const void *src, size_t count) __attribute__((nonnull));
void D_strncpy (char *dest, const char *src, int maxcount) __attribute__((nonnull));
int D_strncasecmp (const char *s1, const char *s2, int len) __attribute__((nonnull));
int D_strcasecmp (const char *s1, const char *s2) __attribute__((nonnull));
int mystrlen(const char *string) __attribute__((nonnull));
int D_atoi(const char* str) __attribute__((nonnull));
char* D_strchr(const char* str, char chr) __attribute__((nonnull));

/*
===============================================================================

							MAPOBJ DATA

===============================================================================
*/

struct mobj_s;

/* think_t is a function pointer to a routine to handle an actor */
typedef void (*think_t) ();

/* a latecall is a function that needs to be called after p_base is done */
typedef void (*latecall_t) (struct mobj_s *mo);

typedef struct thinker_s
{
	struct		thinker_s	*prev, *next;
	think_t		function;
} thinker_t;

struct player_s;

typedef struct
{
	unsigned short	x, y;
	unsigned short	angle;
	unsigned short	type;
} spawnthing_t;

typedef struct mobj_s
{
	fixed_t			x, y, z;
	struct	mobj_s* prev, * next;

	unsigned char		sprite;				/* used to find patch_t and flip value */
	unsigned char		player;		/* only valid if type == MT_PLAYER */

	VINT			health;
	VINT			tics;				/* state tic counter	 */
	VINT 			state;
	VINT			frame;				/* might be ord with FF_FULLBRIGHT */

	unsigned short		type;

/* info for drawing */
	struct	mobj_s	*snext, *sprev;		/* links in sector (if needed) */
	angle_t			angle;

/* interaction info */
	struct mobj_s	*bnext, *bprev;		/* links in blocks (if needed) */
	struct subsector_s	*subsector;
	fixed_t			floorz, ceilingz;	/* closest together of contacted secs */

	int			flags;
	fixed_t			radius, height;		/* for movement checking */

	/* STATIC OBJECTS END HERE */
	union {
		struct {
			unsigned char	movedir;		/* 0-7 */
			char			movecount;		/* when 0, select a new dir */
		};
		unsigned short		thingid;		/* thing id for respawning specials */
	};
	unsigned char		reactiontime;	/* if non 0, don't attack yet */
									/* used by player to freeze a bit after */
									/* teleporting */
	unsigned char		threshold;		/* if >0, the target will be chased */
									/* no matter what (even if shot) */

	fixed_t			momx, momy, momz;	/* momentums */

	unsigned 		speed;			/* mobjinfo[mobj->type].speed */
	struct mobj_s	*target;		/* thing being chased/attacked (or NULL) */
									/* also the originator for missiles */
	latecall_t		latecall;		/* set in p_base if more work needed */
	intptr_t		extradata;		/* for latecall functions */
} mobj_t
;

typedef struct degenmobj_s
{
	fixed_t			x, y, z;
	void 			*prev, *next;
} degenmobj_t
;

#define static_mobj_size (offsetof(mobj_t,momx))

/* */
/* frame flags */
/* */
#define	FF_FULLBRIGHT	0x8000		/* flag in thing->frame */
#define FF_FRAMEMASK	0x7fff

/* */
/* mobj flags */
/* */
#define	MF_SPECIAL		1			/* call P_SpecialThing when touched */
#define	MF_SOLID		2
#define	MF_SHOOTABLE	4
#define	MF_NOSECTOR		8			/* don't use the sector links */
									/* (invisible but touchable)  */
#define	MF_NOBLOCKMAP	16			/* don't use the blocklinks  */
									/* (inert but displayable) */
#define	MF_AMBUSH		32
#define	MF_JUSTHIT		64			/* try to attack right back */
#define	MF_JUSTATTACKED	128			/* take at least one step before attacking */
#define	MF_SPAWNCEILING	256			/* hang from ceiling instead of floor */
#define	MF_NOGRAVITY	512			/* don't apply gravity every tic */

/* movement flags */
#define	MF_DROPOFF		0x400		/* allow jumps from high places */
#define	MF_PICKUP		0x800		/* for players to pick up items */
#define	MF_NOCLIP		0x1000		/* player cheat */
#define	MF_SLIDE		0x2000		/* keep info about sliding along walls */
#define	MF_FLOAT		0x4000		/* allow moves to any height, no gravity */
#define	MF_TELEPORT		0x8000		/* don't cross lines or look at heights */
#define MF_MISSILE		0x10000		/* don't hit same species, explode on block */

#define	MF_DROPPED		0x20000		/* dropped by a demon, not level spawned */
#define	MF_SHADOW		0x40000		/* use fuzzy draw (shadow demons / invis) */
#define	MF_NOBLOOD		0x80000		/* don't bleed when shot (use puff) */
#define	MF_CORPSE		0x100000	/* don't stop moving halfway off a step */
#define	MF_INFLOAT		0x200000	/* floating to a height for a move, don't */
									/* auto float to target's height */

#define	MF_COUNTKILL	0x400000	/* count towards intermission kill total */
#define	MF_COUNTITEM	0x800000	/* count towards intermission item total */

#define	MF_SKULLFLY		0x1000000	/* skull in flight */
#define	MF_NOTDMATCH	0x2000000	/* don't spawn in death match (key cards) */

#define	MF_SEETARGET	0x4000000	/* is target visible? */

#define	MF_STATIC		0x8000000	/* can't move or think */

/*============================================================================= */
typedef enum
{
	PST_LIVE,			/* playing */
	PST_DEAD,			/* dead on the ground */
	PST_REBORN			/* ready to restart */
} playerstate_t;


/* psprites are scaled shapes directly on the view screen */
/* coordinates are given for a 320*200 view screen */
typedef enum
{
	ps_weapon,
	ps_flash,
	NUMPSPRITES
} psprnum_t;

typedef struct
{
	VINT	state;		/* a S_NULL state means not active */
	VINT	tics;
	fixed_t	sx, sy;
} pspdef_t;

typedef enum
{
	it_bluecard,
	it_yellowcard,
	it_redcard,
	it_blueskull,
	it_yellowskull,
	it_redskull,
	NUMCARDS
} card_t;

typedef enum
{
	wp_fist,
	wp_pistol,
	wp_shotgun,
	wp_chaingun,
	wp_missile,
	wp_plasma,
	wp_bfg,
	wp_chainsaw,
	NUMWEAPONS,
	wp_nochange
} weapontype_t;

typedef enum
{
	am_clip,		/* pistol / chaingun */
	am_shell,		/* shotgun */
	am_cell,		/* BFG */
	am_misl,		/* missile launcher */
	NUMAMMO,
	am_noammo		/* chainsaw / fist */
} ammotype_t;


typedef struct
{
	ammotype_t	ammo;
	int			upstate;
	int			downstate;
	int			readystate;
	int			atkstate;
	int			flashstate;
} weaponinfo_t;

extern	const weaponinfo_t	weaponinfo[NUMWEAPONS];

typedef enum
{
	pw_invulnerability,
	pw_strength,
	pw_ironfeet,
	pw_allmap,
	NUMPOWERS
} powertype_t;

#define	INVULNTICS		(30*15)
#define	INVISTICS		(60*15)
#define	INFRATICS		(120*15)
#define	IRONTICS		(60*15)

/*
================
=
= player_t
=
================
*/

typedef struct player_s
{
	mobj_t		*mo;
	playerstate_t	playerstate;
	
	fixed_t		forwardmove, sidemove;	/* built from ticbuttons */
	angle_t		angleturn;				/* built from ticbuttons */
	
	fixed_t		viewz;					/* focal origin above r.z */
	fixed_t		viewheight;				/* base height above floor for viewz */
	fixed_t		deltaviewheight;		/* squat speed */
	fixed_t		bob;					/* bounded/scaled total momentum */
	
	VINT		health;					/* only used between levels, mo->health */
										/* is used during levels	 */
	VINT		armorpoints, armortype;	/* armor type is 0-2 */
	
	VINT		powers[NUMPOWERS];		/* invinc and invis are tic counters	 */
	char		cards[NUMCARDS];
	char		backpack;
	VINT		frags;					/* kills of other player */
	VINT		readyweapon;
	VINT		pendingweapon;		/* wp_nochange if not changing */
	char		weaponowned[NUMWEAPONS];
	VINT		ammo[NUMAMMO];
	VINT		maxammo[NUMAMMO];
	VINT		attackdown, usedown;	/* true if button down last tic */
	VINT		cheats;					/* bit flags */
	
	VINT		refire;					/* refired shots are less accurate */
	
	VINT		killcount, itemcount, secretcount;		/* for intermission */
	char		*message;				/* hint messages */
	VINT		damagecount, bonuscount;/* for screen flashing */
	mobj_t		*attacker;				/* who did damage (NULL for floors) */
	VINT		extralight;				/* so gun flashes light up areas */
	VINT		colormap;				/* 0-3 for which color to draw player */
	pspdef_t	psprites[NUMPSPRITES];	/* view sprites (gun, etc) */
	boolean		didsecret;				/* true if secret level has been done */
	void		*lastsoundsector;		/* don't flood noise every time */
	
	int			automapx, automapy, automapscale, automapflags;
	int			turnheld;				/* for accelerative turning */
} player_t;

// stuff player keeps between respawns in single player
typedef struct
{
	VINT		health;
	VINT		armorpoints, armortype;
	VINT		ammo[NUMAMMO];
	VINT		maxammo[NUMAMMO];
	VINT		cheats;
	VINT		weapon;
	char		weaponowned[NUMWEAPONS];
	char		backpack;
} playerresp_t;

#define CF_NOCLIP		1
#define	CF_GODMODE		2

#define	AF_ACTIVE		1				/* automap active */
#define	AF_FOLLOW		2
#define	AF_ALLLINES		4
#define	AF_ALLMOBJ		8

#define	AF_OPTIONSACTIVE	128			/* options screen running */

/*
===============================================================================

					GLOBAL VARIABLES

===============================================================================
*/

/*================================== */

extern	int 	ticrate;	/* 4 for NTSC, 3 for PAL */
extern	int		ticsinframe;	/* how many tics since last drawer */
extern	int		ticon;
extern	int		frameon;
extern	int		ticbuttons[MAXPLAYERS];
extern	int		oldticbuttons[MAXPLAYERS];
extern	int		ticmousex[MAXPLAYERS], ticmousey[MAXPLAYERS];
extern	int		ticrealbuttons, oldticrealbuttons; /* buttons for the console player before reading the demo file */
extern	boolean		mousepresent;

int MiniLoop ( void (*start)(void),  void (*stop)(void)
		,  int (*ticker)(void), void (*drawer)(void)
		,  void (*update)(void) );

int	G_Ticker (void);
void G_Drawer (void);
void G_RunGame (void);
void G_LoadGame(int saveslot);

/*================================== */

#include "d_mapinfo.h"

extern	gameaction_t	gameaction;

#define	SBARHEIGHT	40			/* status bar height at bottom of screen */

typedef enum
{
	gt_single,
	gt_coop,
	gt_deathmatch	
} gametype_t;

extern	gametype_t	netgame;

extern	boolean		playeringame[MAXPLAYERS];
extern	int			consoleplayer;		/* player taking events and displaying */
extern	player_t	players[MAXPLAYERS];
extern	playerresp_t	playersresp[MAXPLAYERS];

extern	VINT		maxammo[NUMAMMO];


extern	skill_t		gameskill;
extern	int			totalkills, totalitems, totalsecret;	/* for intermission */
extern	int			gamemaplump;
extern	dmapinfo_t	gamemapinfo;
extern	dgameinfo_t	gameinfo;

extern 	VINT 		*gamemapnumbers;
extern 	VINT 		*gamemaplumps;
extern 	VINT 		gamemapcount;

extern 	int 		gametic;
extern 	int 		prevgametic;

#define MAXDMSTARTS		10
extern	mapthing_t	*deathmatchstarts, *deathmatch_p;
extern	mapthing_t	playerstarts[MAXPLAYERS];

#define	BODYQUESIZE		4
extern	int			bodyqueslot;

/*
===============================================================================

					GLOBAL FUNCTIONS

===============================================================================
*/


#ifdef MARS
#define FixedMul(a,b) (((int64_t)(a) * (b))>>16)
fixed_t	FixedDiv (fixed_t a, fixed_t b);
fixed_t IDiv (fixed_t a, fixed_t b);
#else
fixed_t	FixedMul (fixed_t a, fixed_t b);
fixed_t	FixedDiv (fixed_t a, fixed_t b);
#define IDiv(a,b) ((a) / (b))
#endif

#define	ACC_FIXEDMUL	4
#define	ACC_FIXEDDIV	8
#define	ACC_MULSI3		12
#define	ACC_UDIVSI3		16


#if defined(JAGUAR) || (defined(MARS)&&!defined(NeXT))
#ifndef __BIG_ENDIAN__
#define __BIG_ENDIAN__
#endif
#endif

short ShortSwap (short dat);
long LongSwap (long dat);

#ifdef __BIG_ENDIAN__

#define	BIGSHORT(x) (x)
#define	BIGLONG(x) (x)
/*define	LITTLESHORT(x) ShortSwap(x) */
#define	LITTLESHORT(x) (short)((((x)&255)<<8)+(((x)>>8)&255))
#define	LITTLELONG(x) LongSwap(x)

#else

#define	BIGSHORT(x) ShortSwap(x)
#define	BIGLONG(x) LongSwap(x)
#define	LITTLESHORT(x) (x)
#define	LITTLELONG(x) (x)

#endif



/*----------- */
/*MEMORY ZONE */
/*----------- */
/* tags < 100 are not overwritten until freed */
#define	PU_STATIC		1			/* static entire execution time */
#define	PU_SOUND		2			/* static while playing */
#define	PU_MUSIC		3			/* static while playing */
#define	PU_LEVEL		50			/* static until level exited */
#define	PU_LEVSPEC		51			/* a special thinker in a level */

#define	ZONEID	0x1d4a


typedef struct memblock_s
{
	int		size;           /* including the header and possibly tiny fragments */
	short   tag;            /* purgelevel */
	short   id;             /* should be ZONEID */
#ifndef MARS
	int		lockframe;		/* don't purge on the same frame */
#endif
	struct memblock_s   *next;
	struct memblock_s	*prev;
} memblock_t;

typedef struct
{
	int		size;				/* total bytes malloced, including header */
	memblock_t	*rover;
	memblock_t	blocklist;		/* start / end cap for linked list */
} memzone_t;

typedef void (*memblockcall_t) (void *, void*);

extern VINT framecount;

extern	memzone_t	*mainzone;
extern	memzone_t	*refzone;

void	Z_Init (void);
memzone_t *Z_InitZone (byte *base, int size);

void 	*Z_Malloc2 (memzone_t *mainzone, int size, int tag, boolean err);
void 	Z_Free2 (memzone_t *mainzone,void *ptr);

#define Z_Malloc(x,y) Z_Malloc2(mainzone,x,y,true)
#define Z_Free(x) Z_Free2(mainzone,x)

void 	Z_FreeTags (memzone_t *mainzone);
void	Z_CheckHeap (memzone_t *mainzone);
void	Z_ChangeTag (void *ptr, int tag);
int 	Z_FreeMemory (memzone_t *mainzone);
int 	Z_LargestFreeBlock(memzone_t *mainzone);
void 	Z_ForEachBlock(memzone_t *mainzone, memblockcall_t cb, void *userp);
int		Z_FreeBlocks(memzone_t* mainzone);

/*------- */
/*WADFILE */
/*------- */
typedef struct
{
	int			filepos;					/* also texture_t * for comp lumps */
	int			size;
	char		name[8];
} lumpinfo_t;

#define	MAXLUMPS	2048

extern	byte		*wadfileptr;

extern	lumpinfo_t	*lumpinfo;			/* points directly to rom image */
extern	int			numlumps;
extern	void		*lumpcache[MAXLUMPS];

void	W_Init (void);

int		W_CheckNumForName (const char *name);
int		W_GetNumForName (const char *name);
int		W_CheckNumForNameExt (const char *name, int start, int end);

int		W_LumpLength (int lump);
int 	W_ReadLump (int lump, void *dest);

void	*W_CacheLumpNum (int lump, int tag);
void	*W_CacheLumpName (const char *name, int tag);

const char *W_GetNameForNum (int lump);
void* W_GetLumpData(int lump) ATTR_DATA_CACHE_ALIGN;

#define W_POINTLUMPNUM(x) W_GetLumpData(x)


/*---------- */
/*BASE LEVEL */
/*---------- */
void D_DoomMain (void);
void D_DoomLoop (void);

extern	boolean	demoplayback, demorecording;
extern	unsigned *demo_p, *demobuffer;

extern	skill_t		startskill;
extern	int			startmap;
extern	gametype_t	starttype;
extern	int			startsave;
extern 	boolean 	startsplitscreen;

/*--------- */
/*SYSTEM IO */
/*--------- */
#define	SCREENWIDTH		320
#define	SCREENHEIGHT	180

void I_Init (void);
byte *I_WadBase (void);
byte *I_ZoneBase (int *size);
void* I_RemapLumpPtr (void* ptr) ATTR_DATA_CACHE_ALIGN;

/* return a pointer to a 64k or so temp work buffer for level setup uses */
/*(non-displayed frame buffer) */
/* Vic: changed this to always return buffer memset to 0 */
byte *I_TempBuffer (void);

/* temp work buffer which may contain garbage data */
byte *I_WorkBuffer (void);
void I_FreeWorkBuffer(void);
byte *I_AllocWorkBuffer(int size);

pixel_t *I_FrameBuffer (void);
pixel_t* I_OverwriteBuffer(void);

pixel_t *I_ViewportBuffer (void);
int I_ViewportYPos(void);
int I_FrameBufferHeight(void);
int I_IsPAL(void);

void I_ClearFrameBuffer (void);
void I_ClearWorkBuffer(void);
void I_ResetLineTable(void);

void I_SetPalette (const byte *palette);

int I_ReadControls(void);
int I_ReadControls2(void);
int I_ReadMouse(int *pmx, int *pmy);

void I_NetSetup (void);
unsigned I_NetTransfer (unsigned buttons);
void I_NetStop(void);

boolean	I_RefreshCompleted (void);
boolean	I_RefreshLatched (void);
int	I_GetTime (void);
int     I_GetFRTCounter (void);

void I_Update (void);

void I_Error (char *error, ...) __attribute__((noreturn));

void I_StoreScreenCopy(void);
void I_RestoreScreenCopy(void);
void I_SwapScreenCopy(void);

#ifdef MARS
//#define USE_C_DRAW

#ifdef USE_C_DRAW

#define I_DrawColumnLow I_DrawColumnLowC
#define I_DrawFuzzColumnLow I_DrawFuzzColumnLowC
#define I_DrawColumnNPo2Low I_DrawColumnNPo2LowC
#define I_DrawSpanLow I_DrawSpanLowC

#define I_DrawColumn I_DrawColumnC
#define I_DrawColumnNPo2 I_DrawColumnNPo2C
#define I_DrawFuzzColumn I_DrawFuzzColumnC
#define I_DrawSpan I_DrawSpanC

#else

#define I_DrawColumnLow I_DrawColumnLowA
#define I_DrawFuzzColumnLow I_DrawFuzzColumnLowA
#define I_DrawColumnNPo2Low I_DrawColumnNPo2LowA
#define I_DrawSpanLow I_DrawSpanLowA

#define I_DrawColumn I_DrawColumnA
#define I_DrawColumnNPo2 I_DrawColumnNPo2A
#define I_DrawFuzzColumn I_DrawFuzzColumnA
#define I_DrawSpan I_DrawSpanA

#endif

#endif

void I_DrawColumnLow(int dc_x, int dc_yl, int dc_yh, int light, fixed_t dc_iscale,
	fixed_t dc_texturemid, inpixel_t* dc_source, int dc_texheight, int *fuzzpos);

void I_DrawColumnNPo2Low(int dc_x, int dc_yl, int dc_yh, int light, fixed_t dc_iscale,
	fixed_t dc_texturemid, inpixel_t* dc_source, int dc_texheight, int *fuzzpos);

void I_DrawSpanLow(int ds_y, int ds_x1, int ds_x2, int light, fixed_t ds_xfrac,
	fixed_t ds_yfrac, fixed_t ds_xstep, fixed_t ds_ystep, inpixel_t* ds_source, int dc_texheight);

void I_DrawColumn(int dc_x, int dc_yl, int dc_yh, int light, fixed_t dc_iscale,
	fixed_t dc_texturemid, inpixel_t* dc_source, int dc_texheight, int* fuzzpos);

void I_DrawColumnNPo2(int dc_x, int dc_yl, int dc_yh, int light, fixed_t dc_iscale,
	fixed_t dc_texturemid, inpixel_t* dc_source, int dc_texheight, int* fuzzpos);

void I_DrawSpan(int ds_y, int ds_x1, int ds_x2, int light, fixed_t ds_xfrac,
	fixed_t ds_yfrac, fixed_t ds_xstep, fixed_t ds_ystep, inpixel_t* ds_source, int dc_texheight);

void I_DrawSpanPotato(int ds_y, int ds_x1, int ds_x2, int light, fixed_t ds_xfrac,
	fixed_t ds_yfrac, fixed_t ds_xstep, fixed_t ds_ystep, inpixel_t* ds_source, int dc_texheight)
	ATTR_DATA_CACHE_ALIGN;

void I_DrawSpanPotatoLow(int ds_y, int ds_x1, int ds_x2, int light, fixed_t ds_xfrac,
	fixed_t ds_yfrac, fixed_t ds_xstep, fixed_t ds_ystep, inpixel_t* ds_source, int dc_texheight)
	ATTR_DATA_CACHE_ALIGN;

void I_DrawFuzzColumn(int dc_x, int dc_yl, int dc_yh, int light, fixed_t frac_,
	fixed_t fracstep, inpixel_t* dc_source, int dc_texheight, int* fuzzpos);

void I_DrawFuzzColumnLow(int dc_x, int dc_yl, int dc_yh, int light, fixed_t frac_,
	fixed_t fracstep, inpixel_t* dc_source, int dc_texheight, int* fuzzpos);

void I_DrawColumnNoDraw(int dc_x, int dc_yl, int dc_yh, int light, fixed_t frac_,
	fixed_t fracstep, inpixel_t* dc_source, int dc_texheight, int* fuzzpos);

void I_DrawSpanNoDraw(int ds_y, int ds_x1, int ds_x2, int light, fixed_t ds_xfrac,
	fixed_t ds_yfrac, fixed_t ds_xstep, fixed_t ds_ystep, inpixel_t* ds_source, int dc_texheight);

void I_Print8 (int x, int y, const char *string);
int I_Print8Len(const char* string);

void I_DebugScreen (void);

/*---- */
/*GAME */
/*---- */

void G_DeathMatchSpawnPlayer (int playernum);
void G_Init(void);
void G_InitNew (skill_t skill, int map, gametype_t gametype, boolean splitscreen);
void G_ExitLevel (void);
void G_SecretExitLevel (void);
void G_WorldDone (void);

void G_RecordDemo (void);
int G_PlayDemoPtr (unsigned *demo);

int G_LumpNumForMapNum(int map);

/*----- */
/*PLAY */
/*----- */

void P_SetupLevel (int lumpnum, skill_t skill);
void P_Init (void);

void P_Start (void);
void P_Stop (void);
int P_Ticker (void);
void P_Drawer (void);
void P_Update (void);

void IN_Start (void);
void IN_Stop (void);
int IN_Ticker (void);
void IN_Drawer (void);

void M_Start (void);
void M_Start2(boolean startup);
void M_Stop (void);
int M_Ticker (void);
void M_Drawer (void);

void F_Start (void);
void F_Stop (void);
int F_Ticker (void);
void F_Drawer (void);

void AM_Control (player_t *player);
void AM_Drawer (void);
void AM_Start (void);

/*----- */
/*OPTIONS */
/*----- */

extern	VINT	o_musictype;

void O_Init (void);
void O_Control (player_t *player);
void O_Drawer (void);
void O_SetButtonsFromControltype(void);


/*----- */
/*STATUS */
/*----- */

void ST_Init (void);
void ST_Ticker (void);
void ST_Drawer (void);
void ST_ForceDraw(void);
void ST_InitEveryLevel(void);


/*------- */
/*REFRESH */
/*------- */
struct seg_s;

void R_RenderPlayerView (int displayplayer);
void R_Init (void);
int	R_FlatNumForName (const char *name);
int	R_TextureNumForName (const char *name);
int	R_CheckTextureNumForName (const char *name);
angle_t R_PointToAngle2 (fixed_t x1, fixed_t y1, fixed_t x2, fixed_t y2) ATTR_DATA_CACHE_ALIGN;
struct subsector_s *R_PointInSubsector (fixed_t x, fixed_t y) ATTR_DATA_CACHE_ALIGN;


/*---- */
/*MISC */
/*---- */
int M_Random (void) ATTR_DATA_CACHE_ALIGN;
int P_Random (void) ATTR_DATA_CACHE_ALIGN;
void M_ClearRandom (void);
void P_RandomSeed(int seed);
void M_ClearBox (fixed_t *box);
void M_AddToBox (fixed_t *box, fixed_t x, fixed_t y);


/*---- */
/*WIPE */
/*---- */

#define WIPEWIDTH   160

int wipe_InitMelt(short *y);
int wipe_ExitMelt(void);
int wipe_StartScreen(void);
int wipe_EndScreen(void);
int wipe_ScreenWipe(short *y, short *yy, int ticks, int step);


/* header generated by Dave's sound utility */
#include "sounds.h"

/*============================================================================ */
/* */
/* jag additions */
/* */
/*============================================================================ */

#ifndef MARS
extern	pixel_t	*workingscreen;
extern	int		junk, spincount;
#endif

#ifdef JAGUAR
extern	volatile int		ticcount, joybuttons;

#define BLITWAIT while ( ! ((junk=*(int *)0xf02238) & 1) )	;

#define	JP_NUM		1
#define	JP_9		2
#define	JP_6		4
#define	JP_3		8
#define	JP_0		0x10
#define	JP_8		0x20
#define	JP_5		0x40
#define	JP_2		0x80

#define	JP_OPTION	0x200
#define	JP_C		0x2000
#define JP_PWEAPN   0x4000 // CALICO: previous weapon input
#define JP_NWEAPN   0x8000 // CALICO: next weapon input
#define	JP_STAR		0x10000
#define	JP_7		0x20000
#define	JP_4		0x40000
#define	JP_1		0x80000
#define	JP_UP		0x100000
#define	JP_DOWN		0x200000
#define	JP_LEFT		0x400000
#define	JP_RIGHT	0x800000

#define	JP_B		0x2000000
#define	JP_PAUSE	0x10000000
#define	JP_A		0x20000000

#define	BT_A			JP_A
#define	BT_B			JP_B
#define	BT_C			JP_C
#define	BT_OPTION		JP_OPTION
#define	BT_PAUSE		JP_PAUSE
#define BT_STAR			JP_STAR
#define	BT_HASH			JP_NUM
#define	BT_1			JP_1
#define	BT_2			JP_2
#define	BT_3			JP_3
#define	BT_4			JP_4
#define	BT_5			JP_5
#define	BT_6			JP_6
#define	BT_7			JP_7
#define	BT_8			JP_8
#define	BT_9			JP_9
#define	BT_0			JP_0
#define	BT_PWEAPN		JP_PWEAPN
#define	BT_NWEAPN		JP_NWEAPN

#define	BT_LMBTN		0
#define	BT_RMBTN		0
#define	BT_MMBTN		0

#define BT_AUTOMAP		BT_9

#define BT_FASTTURN		0

#else

enum
{
	// hardware-agnostic game button actions
	// transmitted over network
	// should fit into a single word
	BT_RIGHT		= 0x1,
	BT_LEFT			= 0x2,
	BT_UP			= 0x4,
	BT_DOWN			= 0x8,

	BT_ATTACK		= 0x10,
	BT_USE			= 0x20,
	BT_STRAFE		= 0x40,
	BT_SPEED		= 0x80,

	BT_START		= 0x100,
	BT_AUTOMAP		= 0x200,
	BT_MODE			= 0x400,

	BT_PWEAPN		= 0x800,
	BT_NWEAPN		= 0x1000,

	BT_STRAFELEFT	= 0x2000,
	BT_STRAFERIGHT	= 0x4000,

	BT_A			= 0x8000,
	BT_B			= 0x10000,
	BT_C			= 0x20000,
	BT_LMBTN		= 0x40000,
	BT_RMBTN		= 0x80000,
	BT_MMBTN		= 0x100000,
	BT_PAUSE		= 0x200000,
	BT_OPTION		= 0x400000,
	BT_X			= 0x800000,
	BT_Y			= 0x1000000,
	BT_Z			= 0x2000000,
	BT_FASTTURN     = 0x4000000
};

#endif

#define MENU_BTNMASK (BT_A|BT_B|BT_C|BT_LMBTN|BT_RMBTN|BT_UP|BT_DOWN|BT_LEFT|BT_RIGHT|BT_MODE|BT_START|BT_OPTION)

typedef enum
{
	SFU,
	SUF,
	FSU,
	FUS,
	USF,
	UFS,
	NUMCONTROLOPTIONS
} control_t;

/* action buttons can be set to BT_A, BT_B, or BT_C */
/* strafe and use should be set to the same thing */
extern unsigned configuration[NUMCONTROLOPTIONS][3];
extern	VINT	controltype;				/* 0 to 5 */
extern	VINT	strafebtns;
extern	VINT	alwaysrun;
extern	boolean	splitscreen;

extern	VINT	sfxvolume, musicvolume;		/* range from 0 to 255 */
extern	VINT 	sfxdriver;

/* */
/* comnjag.c */
/*  */
extern	int		samplecount;


void C_Init (void);
void NumToStr (int num, char *str);
void PrintNumber (int x, int y, int num);


#define	BASEORGX	(7)
/*define	BASEORGY	(24) */
extern	unsigned BASEORGY;

/*================= */

typedef struct
{
	short	width;		/* in pixels */
	short	height;
	short	depth;		/* 1-5 */
	short	index;		/* location in palette of color 0 */
	short	pad1,pad2,pad3,pad4;	/* future expansion */
	byte	data[8];		/* as much as needed */
} jagobj_t;

void DoubleBufferSetup (void);
void EraseBlock (int x, int y, int width, int height);
void DrawJagobj (jagobj_t *jo, int x, int y);
void DrawJagobjLump(int lumpnum, int x, int y, int* ow, int* oh);
void DrawJagobj2(jagobj_t* jo, int x, int y, 
	int src_x, int src_y, int src_w, int src_h, pixel_t* fb);
void DrawFillRect(int x, int y, int w, int h, int c);
void UpdateBuffer (void);

#ifndef MARS
extern	byte	*bufferpage;		/* draw here */
extern	byte	*displaypage;		/* copied to here when finished */

extern	jagobj_t	*backgroundpic;

/*================= */

extern	int	gpucodestart, gpucodeend;		/* gpu funtion control */
extern int gpufinished;

extern		volatile int	dspcodestart, dspcodeend, dspfinished;

void ReloadWad (void);

int DSPFunction(void* start);

extern short *palette8;
extern int zero, ZERO, zero2;
int DSPRead(void volatile* adr);
#else

#define DSPRead(adr) (intptr_t)(*(adr))

#endif

extern	boolean		gamepaused;
extern	jagobj_t	*pausepic;

#ifndef MARS
extern	pixel_t	*screens[2];
extern	int		workpage;
#endif

void WriteEEProm (void);
void SaveGame(int slotnum);
void ReadGame(int slotnum);
void QuickSave(int nextmap);
int SaveCount(void);
int MaxSaveCount(void);
boolean GetSaveInfo(int slotnumber, VINT* mapnum, VINT* skill, VINT *mode);

void PrintHex (int x, int y, unsigned num);
void DrawPlaque (jagobj_t *pl);
void DrawTiledBackground2(int flat);
void DrawTiledBackground(void);
extern	int		maxlevel;			/* highest level selectable in menu (1-25) */

extern	int		gamevbls;			/* may not really be vbls in multiplayer */
extern	int		vblsinframe;			/* range from 4 to 8 */

#define MINTICSPERFRAME		2
#define MAXTICSPERFRAME		4
extern	VINT	ticsperframe;		/* 2 - 4 */

extern boolean	spr_rotations;

typedef enum
{
	DEBUGMODE_NONE,

	DEBUGMODE_FPSCOUNT,
	DEBUGMODE_ALLCOUNT,
	DEBUGMODE_NOTEXCACHE,
	DEBUGMODE_NODRAW,
	DEBUGMODE_TEXCACHE,

	DEBUGMODE_NUM_MODES
} debugmode_t;

extern VINT debugmode;
extern char clearscreen;
extern VINT colormapopt;
extern VINT initmathtables;

extern VINT COLOR_BLACK;
extern VINT COLOR_WHITE;

void I_InitMenuFire(jagobj_t* titlepic);
void I_StopMenuFire(void);
void I_DrawMenuFire(void);
void I_DrawSbar(void);
void S_StartSong(int musiclump, int looping, int cdtrack);
int S_SongForMapnum(int mapnum);
void S_StopSong(void);
void S_RestartSounds (void);
void S_SetSoundDriver (int newdrv);

/*================= */
/*TLS */
/*================= */

#define DOOMTLS_BANKPAGE 		0
#define DOOMTLS_SETBANKPAGEPTR 	4
#define DOOMTLS_VALIDCOUNT 		8

#ifdef MARS
#define STR_INDIR(x) #x
#define STR(x) STR_INDIR(x)

# define I_SetThreadLocalVar(offs,val) \
	__asm volatile(	"mov %0,r0\n\t" "mov.l r0,@(" STR(offs) ",gbr)\n\t" : : "r"(val) : "r0", "gbr")

# define I_GetThreadLocalVar(offs,val) \
	__asm volatile(	"mov.l @(" STR(offs) ",gbr),r0\n\t" "mov r0,%0\n\t" : "=r"(val) : : "r0", "gbr")
#else

void I_SetThreadLocalVar(uintptr_t offs, uint32_t val);
void I_GetThreadLocalVar(uintptr_t offs, uint32_t val);

#endif

#endif
