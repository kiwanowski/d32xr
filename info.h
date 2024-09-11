#ifndef __INFO_H__
#define __INFO_H__

typedef enum {
SPR_PLAY,
SPR_BBLS,
SPR_BOM1,
SPR_BOM2,
SPR_BOM3,
SPR_BUBL,
SPR_BUS1,
SPR_BUS2,
SPR_DRWN,
SPR_DUST,
SPR_EGGM,
SPR_FISH,
SPR_FL01,
SPR_FL02,
SPR_FL03,
SPR_FL12,
SPR_FWR1,
SPR_FWR2,
SPR_FWR3,
SPR_GFZC,
SPR_JETF,
SPR_MISL,
SPR_MSTV,
SPR_POSS,
SPR_RING,
SPR_ROIA,
SPR_RSPR,
SPR_SCOR,
SPR_SIGN,
SPR_SPLH,
SPR_SPRK,
SPR_SPRR,
SPR_SPRY,
SPR_SSWY,
SPR_SSWR,
SPR_STPT,
SPR_TOKE,
SPR_TV1U,
SPR_TVAR,
SPR_TVAT,
SPR_TVEL,
SPR_TVFO,
SPR_TVIV,
SPR_TVRI,
SPR_TVSS,
SPR_TVWW,
SPR_YSPR,
SPR_GFZDOORP,
SPR_GFZFENCE,
SPR_GFZGRASS,
SPR_GFZQFENCE,
SPR_GFZRAIL,
SPR_GFZWINDP,
NUMSPRITES
} spritenum_t;

typedef enum {
S_NULL,
S_SPAWNSTATE,
S_PLAY_STND,
S_PLAY_TAP1,
S_PLAY_TAP2,
S_PLAY_RUN1,
S_PLAY_RUN2,
S_PLAY_RUN3,
S_PLAY_RUN4,
S_PLAY_RUN5,
S_PLAY_RUN6,
S_PLAY_RUN7,
S_PLAY_RUN8,
S_PLAY_SPD1,
S_PLAY_SPD2,
S_PLAY_SPD3,
S_PLAY_SPD4,
S_PLAY_PAIN,
S_PLAY_DIE,
S_PLAY_DROWN,
S_PLAY_ATK1,
S_PLAY_ATK2,
S_PLAY_ATK3,
S_PLAY_ATK4,
S_PLAY_ATK5,
S_PLAY_DASH1,
S_PLAY_DASH2,
S_PLAY_DASH3,
S_PLAY_DASH4,
S_PLAY_GASP,
S_PLAY_SPRING,
S_PLAY_FALL1,
S_PLAY_FALL2,
S_PLAY_TEETER1,
S_PLAY_TEETER2,
S_PLAY_HANG,
S_POSS_STND,
S_POSS_STND2,
S_POSS_RUN1,
S_POSS_RUN2,
S_POSS_RUN3,
S_POSS_RUN4,
S_POSS_RUN5,
S_POSS_RUN6,
S_POSS_RUN7,
S_POSS_RUN8,
S_FISH1,
S_FISH2,
S_FISH3,
S_FISH4,
S_RING1,
S_RING2,
S_RING3,
S_RING4,
S_RING5,
S_RING6,
S_RING7,
S_RING8,
S_RING9,
S_RING10,
S_RING11,
S_RING12,
S_SPARK1,
S_SPARK2,
S_SPARK3,
S_SPARK4,
S_SPARK5,
S_SPARK6,
S_SPARK7,
S_SPARK8,
S_SPARK9,
S_GFZFLOWERA1,
S_GFZFLOWERA2,
S_GFZFLOWERA3,
S_GFZFLOWERA4,
S_GFZFLOWERA5,
S_GFZFLOWERA6,
S_GFZFLOWERA7,
S_GFZFLOWERA8,
S_GFZFLOWERB1,
S_GFZFLOWERB2,
S_GFZFLOWERB3,
S_GFZFLOWERB4,
S_GFZFLOWERB5,
S_GFZFLOWERB6,
S_GFZFLOWERB7,
S_GFZFLOWERB8,
S_GFZFLOWERB9,
S_GFZFLOWERB10,
S_GFZFLOWERB11,
S_GFZFLOWERB12,
S_GFZFLOWERB13,
S_GFZFLOWERB14,
S_GFZFLOWERB15,
S_GFZFLOWERB16,
S_GFZFLOWERB17,
S_GFZFLOWERB18,
S_GFZFLOWERC1,
S_GFZFLOWERC2,
S_GFZFLOWERC3,
S_GFZFLOWERC4,
S_GFZFLOWERC5,
S_GFZFLOWERC6,
S_GFZFLOWERC7,
S_GFZFLOWERC8,
S_GFZFLOWERC9,
S_GFZFLOWERC10,
S_GFZFLOWERC11,
S_GFZFLOWERC12,
S_BERRYBUSH,
S_BUSH,
S_YELLOWSPRING,
S_REDSPRING,
S_YDIAG,
S_RDIAG,
S_YHORIZ,
S_RHORIZ,
S_XPLD_FLICKY,
S_XPLD1,
S_XPLD2,
S_XPLD3,
S_XPLD4,
S_XPLD5,
S_XPLD6,
S_SCORE100,
S_SCORE200,
S_SCORE500,
S_SCORE1000,
S_SPINDUST1,
S_SPINDUST2,
S_SPINDUST3,
S_SPINDUST4,
S_BOX_FLICKER,
S_BOX_POP1,
S_BOX_POP2,
S_RING_BOX,
S_ATTRACT_BOX,
S_FORCE_BOX,
S_ARMAGEDDON_BOX,
S_WHIRLWIND_BOX,
S_ELEMENTAL_BOX,
S_SNEAKERS_BOX,
S_INVULN_BOX,
S_1UP_BOX,
S_RING_ICON1,
S_RING_ICON2,
S_ATTRACT_ICON1,
S_ATTRACT_ICON2,
S_FORCE_ICON1,
S_FORCE_ICON2,
S_ARMAGEDDON_ICON1,
S_ARMAGEDDON_ICON2,
S_WHIRLWIND_ICON1,
S_WHIRLWIND_ICON2,
S_ELEMENTAL_ICON1,
S_ELEMENTAL_ICON2,
S_SNEAKERS_ICON1,
S_SNEAKERS_ICON2,
S_INVULN_ICON1,
S_INVULN_ICON2,
S_1UP_ICON1,
S_1UP_ICON2,

// Bluebird
S_FLICKY_01_OUT,
S_FLICKY_01_FLAP1,
S_FLICKY_01_FLAP2,
S_FLICKY_01_FLAP3,

// Rabbit
S_FLICKY_02_OUT,
S_FLICKY_02_UP,
S_FLICKY_02_DOWN,

// Chicken
S_FLICKY_03_OUT,
S_FLICKY_03_FLAP1,
S_FLICKY_03_FLAP2,

// Rat
S_FLICKY_12_OUT,
S_FLICKY_12_RUN1,
S_FLICKY_12_RUN2,
S_FLICKY_12_RUN3,

NUMSTATES
} statenum_t;

typedef struct
{
	 uint16_t	sprite;
	 uint16_t	frame;
	 int16_t	tics;
	 uint16_t	nextstate;
	 void		(*action) ();
} state_t;

extern const state_t	states[NUMSTATES];
extern const char * const sprnames[NUMSPRITES];


typedef enum {
MT_PLAYER,
MT_CAMERA,
MT_EXPLODE,
MT_POSSESSED,
MT_GFZFISH,
MT_RING,
MT_SPARK,
MT_FLINGRING,
MT_GFZFLOWERA,
MT_GFZFLOWERB,
MT_GFZFLOWERC,
MT_BERRYBUSH,
MT_BUSH,
MT_YELLOWSPRING,
MT_REDSPRING,
MT_YELLOWDIAG,
MT_REDDIAG,
MT_YELLOWHORIZ,
MT_REDHORIZ,
MT_SCORE,
MT_GHOST,
MT_RING_BOX,
MT_ATTRACT_BOX,
MT_FORCE_BOX,
MT_ARMAGEDDON_BOX,
MT_WHIRLWIND_BOX,
MT_ELEMENTAL_BOX,
MT_SNEAKERS_BOX,
MT_INVULN_BOX,
MT_1UP_BOX,
MT_RING_ICON,
MT_ATTRACT_ICON,
MT_FORCE_ICON,
MT_ARMAGEDDON_ICON,
MT_WHIRLWIND_ICON,
MT_ELEMENTAL_ICON,
MT_SNEAKERS_ICON,
MT_INVULN_ICON,
MT_1UP_ICON,

// Flickies
MT_FLICKY_01, // Bluebird
MT_FLICKY_02, // Rabbit
MT_FLICKY_03, // Chicken
MT_FLICKY_12, // Rat

NUMMOBJTYPES
} mobjtype_t;

typedef struct {
	int16_t		doomednum;
	uint16_t	spawnstate;
	int16_t		spawnhealth;
	uint16_t	seestate;
	uint8_t		seesound;
	uint8_t		reactiontime;
	uint16_t	attacksound;
	uint16_t	painstate;
	uint16_t	painchance;
	uint16_t	painsound;
	uint16_t	meleestate;
	uint16_t	missilestate;
	uint16_t	deathstate;
	uint16_t	xdeathstate;
	uint16_t	deathsound;
	int		speed;
	int		radius;
	int		height;
	uint16_t	mass;
	uint16_t	damage;
	uint16_t	activesound;
	int		flags;
} mobjinfo_t;

extern const mobjinfo_t mobjinfo[NUMMOBJTYPES];

#endif