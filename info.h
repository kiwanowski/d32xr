#ifndef __INFO_H__
#define __INFO_H__

typedef enum {
SPR_PLAY,
SPR_ARMA,
SPR_BBLS,
SPR_BMNB,
SPR_BMNE,
SPR_BOM1,
SPR_BOM2,
SPR_BOM3,
SPR_BUBL,
SPR_BUS1,
SPR_BUS2,
SPR_BUZZ,
SPR_CORL,
SPR_DRWN,
SPR_DUST,
SPR_EGGM,
SPR_EGGN,
SPR_EGLZ,
SPR_ELEM,
SPR_FISH,
SPR_FL01,
SPR_FL02,
SPR_FL03,
SPR_FL12,
SPR_FORC,
SPR_FRET,
SPR_FWR1,
SPR_FWR2,
SPR_FWR3,
SPR_FWR5,
SPR_FWR6,
SPR_GFZC,
SPR_GOOP,
SPR_IVSP,
SPR_JETF,
SPR_JJAW,
SPR_JPLA,
SPR_KELP,
SPR_LASF,
SPR_LASR,
SPR_MAGN,
SPR_MINE,
SPR_MISL,
SPR_MSTV,
SPR_POSS,
SPR_RBUZ,
SPR_RING,
SPR_ROIA,
SPR_RSPR,
SPR_SCOR,
SPR_SEWE,
SPR_SIGN,
SPR_SKIM,
SPR_SPHR,
SPR_SPLH,
SPR_SPOS,
SPR_SPRK,
SPR_SPRR,
SPR_SPRY,
SPR_SSHL,
SPR_SSWY,
SPR_SSWR,
SPR_STLG,
SPR_STPT,
SPR_TANK,
SPR_TFLO,
SPR_THZP,
SPR_THZT,
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
SPR_WIND,
SPR_YSPR,
NUMSPRITES
} spritenum_t;

typedef enum {
S_NULL,
S_INVISIBLE,
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
S_POSS_RUN1,
S_POSS_RUN2,
S_POSS_RUN3,
S_SPOS_STND,
S_SPOS_RUN1,
S_SPOS_RUN2,
S_SPOS_RUN3,
S_FISH1,
S_FISH2,
S_FISH3,
S_FISH4,
S_SSHELL_STND,
S_SSHELL_RUN1,
S_SSHELL_RUN2,
S_BUZZLOOK1,
S_BUZZLOOK2,
S_BUZZFLY1,
S_BUZZFLY2,
S_RBUZZLOOK1,
S_RBUZZLOOK2,
S_RBUZZFLY1,
S_RBUZZFLY2,
S_JETJAW_ROAM1,
S_JETJAW_ROAM2,
S_JETJAW_CHOMP1,
S_JETJAW_CHOMP2,
S_SKIM_LOOK,
S_SKIM_ACTIVE,
S_SKIM_DROP1,
S_SKIM_DROP2,
S_BIGMINE_IDLE,
S_BIGMINE_ALERT,
S_BIGMINE_SET1,
S_BIGMINE_SET2,
S_BIGMINE_SET3,
S_BIGMINE_BLAST1,
S_BIGMINE_BLAST2,
S_BIGMINE_BLAST3,
S_BIGMINE_BLAST4,

S_TOKEN1,
S_TOKEN2,
S_TOKEN3,
S_TOKEN4,
S_TOKEN5,
S_TOKEN6,
S_TOKEN7,
S_TOKEN8,
S_TOKEN9,
S_TOKEN10,
S_BLUESPHERE,
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
S_THZFLOWERA1, // old flower
S_THZFLOWERA2,
S_THZFLOWERA3,
S_THZFLOWERA4,
S_THZFLOWERA5,
S_THZFLOWERA6,
S_THZFLOWERA7,
S_THZFLOWERA8,
S_THZFLOWERB1, // red
S_THZFLOWERB2,
S_THZFLOWERB3,
S_THZFLOWERB4,
S_THZFLOWERB5,
S_THZFLOWERB6,
S_THZFLOWERB7,
S_THZFLOWERB8,
S_THZFLOWERB9,
S_THZFLOWERB10,
S_THZFLOWERC1, // yellow
S_THZFLOWERC2,
S_THZFLOWERC3,
S_THZFLOWERC4,
S_THZFLOWERC5,
S_THZFLOWERC6,
S_THZFLOWERC7,
S_THZFLOWERC8,
S_THZFLOWERC9,
S_THZFLOWERC10,
S_THZTREE,
S_SEAWEED,
S_CORAL1,
S_CORAL2,
S_CORAL3,
S_CORAL4,
S_CORAL5,
S_KELP,
S_STLG0,
S_STLG1,
S_STLG2,
S_STLG3,
S_STLG4,
S_STLG5,
S_STLG6,
S_STLG7,
S_BIGFERN,
S_JUNGLEPALM,
S_TORCHFLOWER1,
S_TORCHFLOWER2,

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

// Armageddon shield
S_ARMA1,
S_ARMA2,
S_ARMA3,
S_ARMA4,
S_ARMA5,
S_ARMA6,
S_ARMA7,
S_ARMA8,

// Whirlwind shield
S_WIND1,
S_WIND2,
S_WIND3,
S_WIND4,

// Attraction shield
S_MAGN1,
S_MAGN2,
S_MAGN3,
S_MAGN4,
S_MAGN5,
S_MAGN6,
S_MAGN7,

// Force shield
S_FORCA1,
S_FORCA2,
S_FORCA3,
S_FORCA4,
S_FORCB1,
S_FORCB2,
S_FORCB3,
S_FORCB4,
S_FORCESTOP,

// Elemental shield
S_ELEM1,
S_ELEM2,
S_ELEM3,
S_ELEM4,
S_ELEMDOWN,

S_IVSP1,
S_IVSP2,
S_IVSP3,
S_IVSP4,
S_IVSP5,
S_IVSP6,
S_IVSP7,
S_IVSP8,
S_IVSP9,
S_IVSP10,
S_IVSP11,
S_IVSP12,
S_IVSP13,
S_IVSP14,
S_IVSP15,
S_IVSP16,
S_IVSP17,
S_IVSP18,
S_IVSP19,
S_IVSP20,
S_IVSP21,
S_IVSP22,
S_IVSP23,
S_IVSP24,
S_IVSP25,
S_IVSP26,
S_IVSP27,
S_IVSP28,
S_IVSP29,
S_IVSP30,
S_IVSP31,
S_IVSP32,

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

S_GFZDEBRIS1,
S_GFZDEBRIS2,
S_GFZDEBRIS3,
S_GFZDEBRIS4,
S_GFZDEBRIS5,
S_GFZDEBRIS6,
S_GFZDEBRIS7,
S_GFZDEBRIS8,

// Bubble patch
S_BUBBLES1,
S_BUBBLES2,
S_BUBBLES3,
S_BUBBLES4,

// Bubbles
S_SMALLBUBBLE,
S_MEDIUMBUBBLE,
S_LARGEBUBBLE1,
S_LARGEBUBBLE2,
S_EXTRALARGEBUBBLE, // breathable

S_POP1, // Extra Large bubble goes POP!

// Water Splish
S_SPLISH1,
S_SPLISH2,
S_SPLISH3,
S_SPLISH4,
S_SPLISH5,
S_SPLISH6,
S_SPLISH7,
S_SPLISH8,
S_SPLISH9,

S_BOSSEXPLODE1,
S_BOSSEXPLODE2,
S_BOSSEXPLODE3,
S_BOSSEXPLODE4,
S_BOSSEXPLODE5,
S_BOSSEXPLODE6,

// S3&K Boss Explosion
S_SONIC3KBOSSEXPLOSION1,
S_SONIC3KBOSSEXPLOSION2,
S_SONIC3KBOSSEXPLOSION3,
S_SONIC3KBOSSEXPLOSION4,
S_SONIC3KBOSSEXPLOSION5,
S_SONIC3KBOSSEXPLOSION6,

S_JETFUME1,
S_FRET,

// Boss 1
S_EGGMOBILE_STND,
S_EGGMOBILE_ROFL,
S_EGGMOBILE_ROFL1,
S_EGGMOBILE_ROFL2,
S_EGGMOBILE_LATK1,
S_EGGMOBILE_LATK2,
S_EGGMOBILE_LATK3,
S_EGGMOBILE_LATK4,
S_EGGMOBILE_LATK5,
S_EGGMOBILE_LATK6,
S_EGGMOBILE_LATK7,
S_EGGMOBILE_LATK8,
S_EGGMOBILE_LATK9,
S_EGGMOBILE_RATK1,
S_EGGMOBILE_RATK2,
S_EGGMOBILE_RATK3,
S_EGGMOBILE_RATK4,
S_EGGMOBILE_RATK5,
S_EGGMOBILE_RATK6,
S_EGGMOBILE_RATK7,
S_EGGMOBILE_RATK8,
S_EGGMOBILE_RATK9,
S_EGGMOBILE_PANIC1,
S_EGGMOBILE_PANIC2,
S_EGGMOBILE_PANIC2A,
S_EGGMOBILE_PANIC2B,
S_EGGMOBILE_PANIC2C,
S_EGGMOBILE_PANIC3,
S_EGGMOBILE_PANIC4,
S_EGGMOBILE_PANIC5,
S_EGGMOBILE_PANIC6,
S_EGGMOBILE_PANIC7,
S_EGGMOBILE_PANIC7A,
S_EGGMOBILE_PANIC7B,
S_EGGMOBILE_PANIC7C,
S_EGGMOBILE_PANIC8,
S_EGGMOBILE_PANIC9,
S_EGGMOBILE_PANIC10,
S_EGGMOBILE_PANIC11,
S_EGGMOBILE_PANIC12,
S_EGGMOBILE_PANIC12A,
S_EGGMOBILE_PANIC12B,
S_EGGMOBILE_PANIC12C,
S_EGGMOBILE_PANIC13,
S_EGGMOBILE_PANIC14,
S_EGGMOBILE_PANIC15,
S_EGGMOBILE_PAIN,
S_EGGMOBILE_PAIN2,
S_EGGMOBILE_DIE1,
S_EGGMOBILE_DIE2,
S_EGGMOBILE_DIE3,
S_EGGMOBILE_DIE4,
S_EGGMOBILE_FLEE1,
S_EGGMOBILE_FLEE2,

S_EGGMOBILE_TARGET,

S_BOSSEGLZ1,
S_BOSSEGLZ2,

S_LASER,
S_LASER2,

S_LASERFLAME1,
S_LASERFLAME2,
S_LASERFLAME3,
S_LASERFLAME4,
S_LASERFLAME4A,

S_EGGMOBILE2_STND,
S_EGGMOBILE2_POGO1,
S_EGGMOBILE2_POGO2,
S_EGGMOBILE2_POGO3,
S_EGGMOBILE2_POGO4,
S_EGGMOBILE2_POGO5,
S_EGGMOBILE2_POGO6,
S_EGGMOBILE2_POGO7,
S_EGGMOBILE2_PAIN,
S_EGGMOBILE2_PAIN2,
S_EGGMOBILE2_DIE1,
S_EGGMOBILE2_DIE2,
S_EGGMOBILE2_DIE3,
S_EGGMOBILE2_DIE4,
S_EGGMOBILE2_FLEE1,
S_EGGMOBILE2_FLEE2,

S_BOSSTANK1,
S_BOSSSPIGOT,
S_GOOP1,
S_GOOP2,
S_GOOP3,

S_DUST1,
S_DUST2,
S_DUST3,
S_DUST4,

S_MINE,

// Starpost
S_STARPOST_IDLE,
S_STARPOST_FLASH1,
S_STARPOST_FLASH2,
S_STARPOST_FLASH3,
S_STARPOST_FLASH4,
S_STARPOST_SPIN1,
S_STARPOST_SPIN2,
S_STARPOST_SPIN3,
S_STARPOST_SPIN4,
S_STARPOST_SPIN5,
S_STARPOST_SPIN6,
S_STARPOST_SPIN7,
S_STARPOST_SPIN8,
S_STARPOST_SPIN9,
S_STARPOST_SPIN10,
S_STARPOST_SPIN11,
S_STARPOST_SPIN12,
S_STARPOST_SPIN13,
S_STARPOST_SPIN14,
S_STARPOST_SPIN15,
S_STARPOST_SPIN16,
S_STARPOST_SPIN17,
S_STARPOST_SPIN18,
S_STARPOST_SPIN19,
S_STARPOST_SPIN20,
S_STARPOST_SPIN21,
S_STARPOST_SPIN22,
S_STARPOST_SPIN23,

S_ZERO,
S_ONE,
S_TWO,
S_THREE,
S_FOUR,
S_FIVE,

S_SIGN,
S_SIGN1,
S_SIGN2,
S_SIGN3,
S_SIGN4,
S_SIGN5,
S_SIGN6,
S_SIGN7,
S_SIGNSTOP,

NUMSTATES
} statenum_t;

typedef struct
{
	 uint16_t	sprite;
	 uint16_t	frame;
	 int16_t	tics;
	 int16_t   var1;
	 int16_t   var2;
	 uint16_t	nextstate;
	 void		(*action)();
} state_t;

extern const state_t	states[NUMSTATES];
extern const char * const sprnames[NUMSPRITES];


typedef enum {
// START HIGH DETAIL SPRITES
MT_PLAYER,
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
MT_EGGMOBILE,
MT_EGGMOBILE2,
MT_STARPOST,
MT_DROWNNUMBERS,
MT_SCORE,
MT_SIGN,
// END HIGH DETAIL SPRITES (MT_SIGN MUST ALWAYS BE LAST ENTRY)

MT_CAMERA,
MT_EXPLODE,
MT_POSSESSED,
MT_REDCRAWLA,
MT_GFZFISH,
MT_SPRINGSHELL,
MT_GOLDBUZZ,
MT_REDBUZZ,
MT_SKIM,
MT_JETJAW,
MT_BIGMINE,

MT_TOKEN,
MT_BLUESPHERE,
MT_RING,
MT_SPARK,
MT_FLINGRING,
MT_ATTRACTRING,
MT_GFZFLOWERA,
MT_GFZFLOWERB,
MT_GFZFLOWERC,
MT_BERRYBUSH,
MT_BUSH,
MT_THZFLOWER1,
MT_THZFLOWER2,
MT_THZFLOWER3,
MT_THZTREE,
MT_SEAWEED,
MT_CORAL1,
MT_CORAL2,
MT_CORAL3,
MT_CORAL4,
MT_CORAL5,
MT_KELP,
MT_STALAGMITE0,
MT_STALAGMITE1,
MT_STALAGMITE2,
MT_STALAGMITE3,
MT_STALAGMITE4,
MT_STALAGMITE5,
MT_STALAGMITE6,
MT_STALAGMITE7,
MT_BIGFERN,
MT_JUNGLEPALM,
MT_TORCHFLOWER,

MT_YELLOWSPRING,
MT_REDSPRING,
MT_YELLOWDIAG,
MT_REDDIAG,
MT_YELLOWHORIZ,
MT_REDHORIZ,
MT_GHOST,

MT_ELEMENTAL_ORB,
MT_ATTRACT_ORB,
MT_FORCE_ORB, // FORCE_STOP can just be MT_GHOST
MT_ARMAGEDDON_ORB,
MT_WHIRLWIND_ORB,
MT_IVSP,

// Flickies
MT_FLICKY_01, // Bluebird
MT_FLICKY_02, // Rabbit
MT_FLICKY_03, // Chicken
MT_FLICKY_12, // Rat

MT_GFZDEBRIS,

MT_BUBBLES,
MT_SMALLBUBBLE, // small bubble
MT_MEDIUMBUBBLE, // medium bubble
MT_EXTRALARGEBUBBLE, // extra large bubble
MT_SPLISH,

// Generic Boss Items
MT_BOSSEXPLODE,
MT_SONIC3KBOSSEXPLODE,
MT_BOSSFLYPOINT,
MT_EGGTRAP,
MT_BOSSJUNK,

// Boss 1 stuff
MT_JETFUME1,
MT_EGGMOBILE_TARGET,
MT_EGGMOBILE_FIRE,

// Boss 2 stuff
MT_GOOP,
MT_AXIS,

MT_LASER,
MT_DUST,
MT_MINE,

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
	VINT		flags;
	VINT		flags2;
} mobjinfo_t;

extern const mobjinfo_t mobjinfo[NUMMOBJTYPES];

#endif