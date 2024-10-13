#include "doomdef.h"

// These are used to track a global animation state for MF_RINGMOBJ
VINT ringmobjstates[NUMMOBJTYPES];
VINT ringmobjtics[NUMMOBJTYPES];

const char * const sprnames[NUMSPRITES] = {
"PLAY",
"ARMA",
"BBLS",
"BOM1",
"BOM2",
"BOM3",
"BUBL",
"BUS1",
"BUS2",
"DRWN",
"DUST",
"EGGM",
"EGLZ",
"ELEM",
"FISH",
"FL01",
"FL02",
"FL03",
"FL12",
"FORC",
"FRET",
"FWR1",
"FWR2",
"FWR3",
"GFZC",
"IVSP",
"JETF",
"LASF",
"LASR",
"MAGN",
"MISL",
"MSTV",
"POSS",
"RING",
"ROIA",
"RSPR",
"SCOR",
"SIGN",
"SPLH",
"SPRK",
"SPRR",
"SPRY",
"SSWY",
"SSWR",
"STPT",
"TOKE",
"TV1U",
"TVAR",
"TVAT",
"TVEL",
"TVFO",
"TVIV",
"TVRI",
"TVSS",
"TVWW",
"WIND",
"YSPR",
};

void A_SpawnState();
void A_Chase();
void A_FaceTarget();
void A_SkullAttack();
void A_Fall();
void A_Look();
void A_Pain();
void A_BossScream();
void A_BossDeath();
void A_FishJump();
void A_MonitorPop();
void A_AwardBox();
void A_FlickyCheck();
void A_FlickyFly();
void A_BubbleRise();
void A_FocusTarget();
void A_Boss1Chase();
void A_Boss1Laser();
void A_PrepareRepeat();
void A_Repeat();
void A_ChangeHeight();
void A_UnidusBall();
void A_BubbleSpawn();
void A_SignSpin();

#define STATE(sprite,frame,tics,action,nextstate) {sprite,frame,tics,0,0,nextstate,action}
#define STATE2(sprite,frame,tics,action,var1,var2,nextstate) {sprite,frame,tics,var1,var2,nextstate,action}

const state_t	states[NUMSTATES] = {
STATE(SPR_PLAY,0,-1,NULL,S_NULL),	// S_NULL
STATE(SPR_PLAY,0,-1,NULL,S_NULL), // S_INVISIBLE
STATE(SPR_PLAY,0,-1,A_SpawnState,S_NULL), // S_SPAWNSTATE
STATE(SPR_PLAY,0,105,NULL,S_PLAY_TAP1),	// S_PLAY_STND
STATE(SPR_PLAY,1,8,NULL,S_PLAY_TAP2), // S_PLAY_TAP1
STATE(SPR_PLAY,2,8,NULL,S_PLAY_TAP1), // S_PLAY_TAP2
STATE(SPR_PLAY,3,4,NULL,S_PLAY_RUN2),	// S_PLAY_RUN1
STATE(SPR_PLAY,4,4,NULL,S_PLAY_RUN3),	// S_PLAY_RUN2
STATE(SPR_PLAY,5,4,NULL,S_PLAY_RUN4),	// S_PLAY_RUN3
STATE(SPR_PLAY,6,4,NULL,S_PLAY_RUN5),	// S_PLAY_RUN4
STATE(SPR_PLAY,7,4,NULL,S_PLAY_RUN6),	// S_PLAY_RUN5
STATE(SPR_PLAY,8,4,NULL,S_PLAY_RUN7),	// S_PLAY_RUN6
STATE(SPR_PLAY,9,4,NULL,S_PLAY_RUN8),	// S_PLAY_RUN7
STATE(SPR_PLAY,10,4,NULL,S_PLAY_RUN1),	// S_PLAY_RUN8
STATE(SPR_PLAY,11,1,NULL,S_PLAY_SPD2), // S_PLAY_SPD1
STATE(SPR_PLAY,12,1,NULL,S_PLAY_SPD3), // S_PLAY_SPD2
STATE(SPR_PLAY,13,1,NULL,S_PLAY_SPD4), // S_PLAY_SPD3
STATE(SPR_PLAY,14,1,NULL,S_PLAY_SPD1), // S_PLAY_SPD4
STATE(SPR_PLAY,15,127,NULL,S_PLAY_FALL1),	// S_PLAY_PAIN
STATE(SPR_PLAY,16,-1,A_Fall,S_NULL),	// S_PLAY_DIE
STATE(SPR_PLAY,17,-1,A_Fall,S_NULL),	// S_PLAY_DROWN
STATE(SPR_PLAY,18,1,NULL,S_PLAY_ATK2), // S_PLAY_ATK1
STATE(SPR_PLAY,19,1,NULL,S_PLAY_ATK3), // S_PLAY_ATK2
STATE(SPR_PLAY,20,1,NULL,S_PLAY_ATK4), // S_PLAY_ATK3
STATE(SPR_PLAY,21,1,NULL,S_PLAY_ATK5), // S_PLAY_ATK4
STATE(SPR_PLAY,22,1,NULL,S_PLAY_ATK1), // S_PLAY_ATK5
STATE(SPR_PLAY,23,1,NULL,S_PLAY_DASH2), // S_PLAY_DASH1
STATE(SPR_PLAY,24,1,NULL,S_PLAY_DASH3), // S_PLAY_DASH2
STATE(SPR_PLAY,25,1,NULL,S_PLAY_DASH4), // S_PLAY_DASH3
STATE(SPR_PLAY,26,1,NULL,S_PLAY_DASH1), // S_PLAY_DASH4
STATE(SPR_PLAY,27,14,NULL,S_PLAY_RUN1), // S_PLAY_GASP
STATE(SPR_PLAY,28,-1,NULL,S_PLAY_FALL1), // S_PLAY_SPRING
STATE(SPR_PLAY,29,2,NULL,S_PLAY_FALL2), // S_PLAY_FALL1
STATE(SPR_PLAY,31,2,NULL,S_PLAY_FALL1), // S_PLAY_FALL2
STATE(SPR_PLAY,32,12,NULL,S_PLAY_TEETER2), // S_PLAY_TEETER1
STATE(SPR_PLAY,33,12,NULL,S_PLAY_TEETER1), // S_PLAY_TEETER2
STATE(SPR_PLAY,34,-1,NULL,S_NULL), // S_PLAY_HANG
STATE(SPR_POSS,0,TICRATE/2,A_Look,S_POSS_STND2),	/* S_POSS_STND */
STATE(SPR_POSS,1,TICRATE/2,A_Look,S_POSS_STND),	/* S_POSS_STND2 */
STATE(SPR_POSS,0,4,A_Chase,S_POSS_RUN2),	/* S_POSS_RUN1 */
STATE(SPR_POSS,0,4,A_Chase,S_POSS_RUN3),	/* S_POSS_RUN2 */
STATE(SPR_POSS,1,4,A_Chase,S_POSS_RUN4),	/* S_POSS_RUN3 */
STATE(SPR_POSS,1,4,A_Chase,S_POSS_RUN5),	/* S_POSS_RUN4 */
STATE(SPR_POSS,2,4,A_Chase,S_POSS_RUN6),	/* S_POSS_RUN5 */
STATE(SPR_POSS,2,4,A_Chase,S_POSS_RUN7),	/* S_POSS_RUN6 */
STATE(SPR_POSS,3,4,A_Chase,S_POSS_RUN8),	/* S_POSS_RUN7 */
STATE(SPR_POSS,3,4,A_Chase,S_POSS_RUN1),	/* S_POSS_RUN8 */
STATE(SPR_FISH,1,1,NULL,S_FISH2), // S_FISH1
STATE(SPR_FISH,1,2,A_FishJump,S_FISH1), // S_FISH2
STATE(SPR_FISH,0,1,NULL,S_FISH4), // S_FISH3
STATE(SPR_FISH,0,2,A_FishJump,S_FISH3), // S_FISH4
STATE(SPR_RING,0,2,NULL,S_RING2), // S_RING1
STATE(SPR_RING,1,2,NULL,S_RING3), // S_RING2
STATE(SPR_RING,2,2,NULL,S_RING4), // S_RING3
STATE(SPR_RING,3,2,NULL,S_RING5), // S_RING4
STATE(SPR_RING,4,2,NULL,S_RING6), // S_RING5
STATE(SPR_RING,5,2,NULL,S_RING7), // S_RING6
STATE(SPR_RING,6,2,NULL,S_RING8), // S_RING7
STATE(SPR_RING,5|FF_FLIPPED,2,NULL,S_RING9), // S_RING8
STATE(SPR_RING,4|FF_FLIPPED,2,NULL,S_RING10), // S_RING9
STATE(SPR_RING,3|FF_FLIPPED,2,NULL,S_RING11), // S_RING10
STATE(SPR_RING,2|FF_FLIPPED,2,NULL,S_RING12), // S_RING11
STATE(SPR_RING,1|FF_FLIPPED,2,NULL,S_RING1), // S_RING12
STATE(SPR_SPRK,0,2,NULL,S_SPARK2), // S_SPARK1
STATE(SPR_SPRK,1,2,NULL,S_SPARK3), // S_SPARK2
STATE(SPR_SPRK,2,2,NULL,S_SPARK4), // S_SPARK3
STATE(SPR_SPRK,3,2,NULL,S_SPARK5), // S_SPARK4
STATE(SPR_SPRK,4,2,NULL,S_SPARK6), // S_SPARK5
STATE(SPR_SPRK,5,2,NULL,S_SPARK7), // S_SPARK6
STATE(SPR_SPRK,6,2,NULL,S_SPARK8), // S_SPARK7
STATE(SPR_SPRK,7,2,NULL,S_SPARK9), // S_SPARK8
STATE(SPR_SPRK,8,2,NULL,S_NULL), // S_SPARK9
STATE(SPR_FWR1,0,4,NULL,S_GFZFLOWERA2), // S_GFZFLOWERA1
STATE(SPR_FWR1,1,4,NULL,S_GFZFLOWERA3), // S_GFZFLOWERA2
STATE(SPR_FWR1,2,4,NULL,S_GFZFLOWERA4), // S_GFZFLOWERA3
STATE(SPR_FWR1,1,4,NULL,S_GFZFLOWERA5), // S_GFZFLOWERA4
STATE(SPR_FWR1,0,4,NULL,S_GFZFLOWERA6), // S_GFZFLOWERA5
STATE(SPR_FWR1,1|FF_FLIPPED,4,NULL,S_GFZFLOWERA7), // S_GFZFLOWERA6
STATE(SPR_FWR1,2|FF_FLIPPED,4,NULL,S_GFZFLOWERA8), // S_GFZFLOWERA7
STATE(SPR_FWR1,1|FF_FLIPPED,4,NULL,S_GFZFLOWERA1), // S_GFZFLOWERA8
STATE(SPR_FWR2,0,TICRATE,NULL,S_GFZFLOWERB2), // S_GFZFLOWERB1
STATE(SPR_FWR2,1,2,NULL,S_GFZFLOWERB3), // S_GFZFLOWERB2
STATE(SPR_FWR2,2,2,NULL,S_GFZFLOWERB4), // S_GFZFLOWERB3
STATE(SPR_FWR2,3,2,NULL,S_GFZFLOWERB5), // S_GFZFLOWERB4
STATE(SPR_FWR2,4,2,NULL,S_GFZFLOWERB6), // S_GFZFLOWERB5
STATE(SPR_FWR2,5,2,NULL,S_GFZFLOWERB7), // S_GFZFLOWERB6
STATE(SPR_FWR2,6,2,NULL,S_GFZFLOWERB8), // S_GFZFLOWERB7
STATE(SPR_FWR2,7,2,NULL,S_GFZFLOWERB9), // S_GFZFLOWERB8
STATE(SPR_FWR2,8,2,NULL,S_GFZFLOWERB10), // S_GFZFLOWERB9
STATE(SPR_FWR2,9,TICRATE,NULL,S_GFZFLOWERB11), // S_GFZFLOWERB10
STATE(SPR_FWR2,8,2,NULL,S_GFZFLOWERB12), // S_GFZFLOWERB11
STATE(SPR_FWR2,7,2,NULL,S_GFZFLOWERB13), // S_GFZFLOWERB12
STATE(SPR_FWR2,6,2,NULL,S_GFZFLOWERB14), // S_GFZFLOWERB13
STATE(SPR_FWR2,5,2,NULL,S_GFZFLOWERB15), // S_GFZFLOWERB14
STATE(SPR_FWR2,4,2,NULL,S_GFZFLOWERB16), // S_GFZFLOWERB15
STATE(SPR_FWR2,3,2,NULL,S_GFZFLOWERB17), // S_GFZFLOWERB16
STATE(SPR_FWR2,2,2,NULL,S_GFZFLOWERB18), // S_GFZFLOWERB17
STATE(SPR_FWR2,1,2,NULL,S_GFZFLOWERB1), // S_GFZFLOWERB18
STATE(SPR_FWR3,0,4,NULL,S_GFZFLOWERC2), // S_GFZFLOWERC1
STATE(SPR_FWR3,1,4,NULL,S_GFZFLOWERC3), // S_GFZFLOWERC2
STATE(SPR_FWR3,2,4,NULL,S_GFZFLOWERC4), // S_GFZFLOWERC3
STATE(SPR_FWR3,3,4,NULL,S_GFZFLOWERC5), // S_GFZFLOWERC4
STATE(SPR_FWR3,1,4,NULL,S_GFZFLOWERC6), // S_GFZFLOWERC5
STATE(SPR_FWR3,4,4,NULL,S_GFZFLOWERC7), // S_GFZFLOWERC6
STATE(SPR_FWR3,0|FF_FLIPPED,4,NULL,S_GFZFLOWERC8), // S_GFZFLOWERC7
STATE(SPR_FWR3,1|FF_FLIPPED,4,NULL,S_GFZFLOWERC9), // S_GFZFLOWERC8
STATE(SPR_FWR3,2|FF_FLIPPED,4,NULL,S_GFZFLOWERC10), // S_GFZFLOWERC9
STATE(SPR_FWR3,3|FF_FLIPPED,4,NULL,S_GFZFLOWERC11), // S_GFZFLOWERC10
STATE(SPR_FWR3,1|FF_FLIPPED,4,NULL,S_GFZFLOWERC12), // S_GFZFLOWERC11
STATE(SPR_FWR3,4|FF_FLIPPED,4,NULL,S_GFZFLOWERC1), // S_GFZFLOWERC12
STATE(SPR_BUS1,0,-1,NULL,S_NULL), // S_BERRYBUSH
STATE(SPR_BUS2,0,-1,NULL,S_NULL), // S_BUSH
STATE(SPR_SPRY,0,-1,NULL,S_NULL), // S_YELLOWSPRING
STATE(SPR_SPRR,0,-1,NULL,S_NULL), // S_REDSPRING
STATE(SPR_YSPR,0,-1,NULL,S_NULL), // S_YDIAG
STATE(SPR_RSPR,0,-1,NULL,S_NULL), // S_RDIAG
STATE(SPR_SSWY,0,-1,NULL,S_NULL), // S_YHORIZ
STATE(SPR_SSWR,0,-1,NULL,S_NULL), // S_RHORIZ
STATE(SPR_BOM1,0,0,NULL,S_XPLD1), // S_XPLD_FLICKY
STATE(SPR_BOM1,0,2,NULL,S_XPLD2), // S_XPLD1
STATE(SPR_BOM1,1,2,NULL,S_XPLD3), // S_XPLD2
STATE(SPR_BOM1,2,2,NULL,S_XPLD4), // S_XPLD3
STATE(SPR_BOM1,3,2,NULL,S_XPLD5), // S_XPLD4
STATE(SPR_BOM1,4,2,NULL,S_XPLD6), // S_XPLD5
STATE(SPR_BOM1,5,2,NULL,S_NULL), // S_XPLD6
STATE(SPR_SCOR,0,TICRATE,NULL,S_NULL), // S_SCORE100
STATE(SPR_SCOR,1,TICRATE,NULL,S_NULL), // S_SCORE200
STATE(SPR_SCOR,2,TICRATE,NULL,S_NULL), // S_SCORE500
STATE(SPR_SCOR,3,TICRATE,NULL,S_NULL), // S_SCORE1000
STATE(SPR_DUST,0,6,NULL,S_SPINDUST2), // S_SPINDUST1
STATE(SPR_DUST,1,4,NULL,S_SPINDUST3), // S_SPINDUST2
STATE(SPR_DUST,2,2,NULL,S_SPINDUST4), // S_SPINDUST3
STATE(SPR_DUST,3,2,NULL,S_NULL), // S_SPINDUST4
STATE(SPR_MSTV,0,2,NULL,S_SPAWNSTATE), // S_BOX_FLICKER
STATE(SPR_MSTV,0,4,A_MonitorPop,S_BOX_POP2), // S_BOX_POP1
STATE(SPR_MSTV,1,-1,NULL,S_NULL), // S_BOX_POP2
STATE(SPR_TVRI,0,4,NULL,S_BOX_FLICKER), // S_RING_BOX
STATE(SPR_TVAT,0,4,NULL,S_BOX_FLICKER), // S_ATTRACT_BOX
STATE(SPR_TVFO,0,4,NULL,S_BOX_FLICKER), // S_FORCE_BOX
STATE(SPR_TVAR,0,4,NULL,S_BOX_FLICKER), // S_ARMAGEDDON_BOX
STATE(SPR_TVWW,0,4,NULL,S_BOX_FLICKER), // S_WHIRLWIND_BOX
STATE(SPR_TVEL,0,4,NULL,S_BOX_FLICKER), // S_ELEMENTAL_BOX
STATE(SPR_TVSS,0,4,NULL,S_BOX_FLICKER), // S_SNEAKERS_BOX
STATE(SPR_TVIV,0,4,NULL,S_BOX_FLICKER), // S_INVULN_BOX
STATE(SPR_TV1U,0,4,NULL,S_BOX_FLICKER), // S_1UP_BOX

STATE(SPR_TVRI,2,16,NULL,S_RING_ICON2), // S_RING_ICON1
STATE(SPR_TVRI,2,16,A_AwardBox,S_NULL), // S_RING_ICON2
STATE(SPR_TVAT,2,16,NULL,S_ATTRACT_ICON2), // S_ATTRACT_ICON1
STATE(SPR_TVAT,2,16,A_AwardBox,S_NULL), // S_ATTRACT_ICON2
STATE(SPR_TVFO,2,16,NULL,S_FORCE_ICON2), // S_FORCE_ICON1
STATE(SPR_TVFO,2,16,A_AwardBox,S_NULL), // S_FORCE_ICON2
STATE(SPR_TVAR,2,16,NULL,S_ARMAGEDDON_ICON2), // S_ARMAGEDDON_ICON1
STATE(SPR_TVAR,2,16,A_AwardBox,S_NULL), // S_ARMAGEDDON_ICON2
STATE(SPR_TVWW,2,16,NULL,S_WHIRLWIND_ICON2), // S_WHIRLWIND_ICON1
STATE(SPR_TVWW,2,16,A_AwardBox,S_NULL), // S_WHIRLWIND_ICON2
STATE(SPR_TVEL,2,16,NULL,S_ELEMENTAL_ICON2), // S_ELEMENTAL_ICON1
STATE(SPR_TVEL,2,16,A_AwardBox,S_NULL), // S_ELEMENTAL_ICON2
STATE(SPR_TVSS,2,16,NULL,S_SNEAKERS_ICON2), // S_SNEAKERS_ICON1
STATE(SPR_TVSS,2,16,A_AwardBox,S_NULL), // S_SNEAKERS_ICON2
STATE(SPR_TVIV,2,16,NULL,S_INVULN_ICON2), // S_INVULN_ICON1
STATE(SPR_TVIV,2,16,A_AwardBox,S_NULL), // S_INVULN_ICON2
STATE(SPR_TV1U,2,16,NULL,S_1UP_ICON2), // S_1UP_ICON1
STATE(SPR_TV1U,2,16,A_AwardBox,S_NULL), // S_1UP_ICON2

// Armageddon shield
STATE(SPR_ARMA,0,1,NULL,S_ARMA2), // S_ARMA1
STATE(SPR_ARMA,1,1,NULL,S_ARMA3), // S_ARMA2
STATE(SPR_ARMA,2,1,NULL,S_ARMA4), // S_ARMA3
STATE(SPR_ARMA,3,1,NULL,S_ARMA5), // S_ARMA4
STATE(SPR_ARMA,4,1,NULL,S_ARMA6), // S_ARMA5
STATE(SPR_ARMA,5,1,NULL,S_ARMA7), // S_ARMA6
STATE(SPR_ARMA,6,1,NULL,S_ARMA8), // S_ARMA7
STATE(SPR_ARMA,7,1,NULL,S_ARMA1), // S_ARMA8

// Whirlwind shield
STATE(SPR_WIND,0,1,NULL,S_WIND2), // S_WIND1
STATE(SPR_WIND,1,1,NULL,S_WIND3), // S_WIND2
STATE(SPR_WIND,2,1,NULL,S_WIND4), // S_WIND3
STATE(SPR_WIND,3,1,NULL,S_WIND1), // S_WIND4

// Attraction shield
STATE(SPR_MAGN,0,1,NULL,S_MAGN2), // S_MAGN1
STATE(SPR_MAGN,1,1,NULL,S_MAGN3), // S_MAGN2
STATE(SPR_MAGN,2,1,NULL,S_MAGN4), // S_MAGN3
STATE(SPR_MAGN,3,1,NULL,S_MAGN5), // S_MAGN4
STATE(SPR_MAGN,4,1,NULL,S_MAGN6), // S_MAGN5
STATE(SPR_MAGN,5,1,NULL,S_MAGN7), // S_MAGN6
STATE(SPR_MAGN,6,1,NULL,S_MAGN1), // S_MAGN7

// Force shield
STATE(SPR_FORC,0,1,NULL,S_FORCA2), // S_FORCA1
STATE(SPR_FORC,1,1,NULL,S_FORCA3), // S_FORCA2,
STATE(SPR_FORC,2,1,NULL,S_FORCA4), // S_FORCA3,
STATE(SPR_FORC,3,1,NULL,S_FORCA1), // S_FORCA4,
STATE(SPR_FORC,4,1,NULL,S_FORCB2), // S_FORCB1,
STATE(SPR_FORC,5,1,NULL,S_FORCB3), // S_FORCB2,
STATE(SPR_FORC,6,1,NULL,S_FORCB4), // S_FORCB3,
STATE(SPR_FORC,7,1,NULL,S_FORCB1), // S_FORCB4,
STATE(SPR_FORC,8,TICRATE/2,NULL,S_NULL), // S_FORCESTOP,

// Elemental shield
STATE(SPR_ELEM,0,1,NULL,S_ELEM2), // S_ELEM1,
STATE(SPR_ELEM,1,1,NULL,S_ELEM3), // S_ELEM2,
STATE(SPR_ELEM,2,1,NULL,S_ELEM4), // S_ELEM3,
STATE(SPR_ELEM,3,1,NULL,S_ELEM1), // S_ELEM4,
STATE(SPR_ELEM,4,2,NULL,S_ELEM1), // S_ELEMDOWN,

// Invincibility sparkles
STATE(SPR_IVSP,0,1,NULL,S_IVSP2), // S_IVSP1
STATE(SPR_IVSP,1,1,NULL,S_IVSP3), // S_IVSP2
STATE(SPR_IVSP,2,1,NULL,S_IVSP4), // S_IVSP3
STATE(SPR_IVSP,3,1,NULL,S_IVSP5), // S_IVSP4
STATE(SPR_IVSP,4,1,NULL,S_IVSP6), // S_IVSP5
STATE(SPR_IVSP,5,1,NULL,S_IVSP7), // S_IVSP6
STATE(SPR_IVSP,6,1,NULL,S_IVSP8), // S_IVSP7
STATE(SPR_IVSP,7,1,NULL,S_IVSP9), // S_IVSP8
STATE(SPR_IVSP,8,1,NULL,S_IVSP10), // S_IVSP9
STATE(SPR_IVSP,9,1,NULL,S_IVSP11), // S_IVSP10
STATE(SPR_IVSP,10,1,NULL,S_IVSP12), // S_IVSP11
STATE(SPR_IVSP,11,1,NULL,S_IVSP13), // S_IVSP12
STATE(SPR_IVSP,12,1,NULL,S_IVSP14), // S_IVSP13
STATE(SPR_IVSP,13,1,NULL,S_IVSP15), // S_IVSP14
STATE(SPR_IVSP,14,1,NULL,S_IVSP16), // S_IVSP15
STATE(SPR_IVSP,15,1,NULL,S_IVSP17), // S_IVSP16
STATE(SPR_IVSP,16,1,NULL,S_IVSP18), // S_IVSP17
STATE(SPR_IVSP,17,1,NULL,S_IVSP19), // S_IVSP18
STATE(SPR_IVSP,18,1,NULL,S_IVSP20), // S_IVSP19
STATE(SPR_IVSP,19,1,NULL,S_IVSP21), // S_IVSP20
STATE(SPR_IVSP,20,1,NULL,S_IVSP22), // S_IVSP21
STATE(SPR_IVSP,21,1,NULL,S_IVSP23), // S_IVSP22
STATE(SPR_IVSP,22,1,NULL,S_IVSP24), // S_IVSP23
STATE(SPR_IVSP,23,1,NULL,S_IVSP25), // S_IVSP24
STATE(SPR_IVSP,24,1,NULL,S_IVSP26), // S_IVSP25
STATE(SPR_IVSP,25,1,NULL,S_IVSP27), // S_IVSP26
STATE(SPR_IVSP,26,1,NULL,S_IVSP28), // S_IVSP27
STATE(SPR_IVSP,27,1,NULL,S_IVSP29), // S_IVSP28
STATE(SPR_IVSP,28,1,NULL,S_IVSP30), // S_IVSP29
STATE(SPR_IVSP,29,1,NULL,S_IVSP31), // S_IVSP30
STATE(SPR_IVSP,30,1,NULL,S_IVSP32), // S_IVSP31
STATE(SPR_IVSP,31,1,NULL,S_NULL), // S_IVSP32

// Bluebird
STATE(SPR_FL01,0,2,A_FlickyCheck,S_FLICKY_01_OUT), // S_FLICKY_01_OUT
STATE(SPR_FL01,1,4,A_FlickyFly,S_FLICKY_01_FLAP2), // S_FLICKY_01_FLAP1
STATE(SPR_FL01,2,4,A_FlickyFly,S_FLICKY_01_FLAP3), // S_FLICKY_01_FLAP2
STATE(SPR_FL01,3,4,A_FlickyFly,S_FLICKY_01_FLAP1), // S_FLICKY_01_FLAP3

// Rabbit
STATE(SPR_FL02,0,4,A_FlickyCheck,S_FLICKY_02_OUT), // S_FLICKY_02_OUT
STATE(SPR_FL02,2,16,NULL,S_FLICKY_02_DOWN), // S_FLICKY_02_UP
STATE(SPR_FL02,3,4,A_FlickyFly,S_FLICKY_02_DOWN), // S_FLICKY_02_DOWN

// Chicken
STATE(SPR_FL03,0,4,A_FlickyCheck,S_FLICKY_03_OUT), // S_FLICKY_03_OUT
STATE(SPR_FL03,2,4,A_FlickyFly,S_FLICKY_03_FLAP2), // S_FLICKY_03_FLAP1
STATE(SPR_FL03,3,4,A_FlickyFly,S_FLICKY_03_FLAP1), // S_FLICKY_03_FLAP2

// Rat
STATE(SPR_FL12,0,4,A_FlickyCheck,S_FLICKY_12_OUT), // S_FLICKY_12_OUT
STATE(SPR_FL12,1,2,A_FlickyFly,S_FLICKY_12_RUN2), // S_FLICKY_12_RUN1
STATE(SPR_FL12,2,2,NULL,S_FLICKY_12_RUN3), // S_FLICKY_12_RUN2
STATE(SPR_FL12,3,2,NULL,S_FLICKY_12_RUN1), // S_FLICKY_12_RUN2

STATE(SPR_GFZC,0,1,NULL,S_GFZDEBRIS2), // S_GFZDEBRIS1
STATE(SPR_GFZC,1,1,NULL,S_GFZDEBRIS3), // S_GFZDEBRIS2
STATE(SPR_GFZC,2,1,NULL,S_GFZDEBRIS4), // S_GFZDEBRIS3
STATE(SPR_GFZC,3,1,NULL,S_GFZDEBRIS5), // S_GFZDEBRIS4
STATE(SPR_GFZC,4,1,NULL,S_GFZDEBRIS6), // S_GFZDEBRIS5
STATE(SPR_GFZC,5,1,NULL,S_GFZDEBRIS7), // S_GFZDEBRIS6
STATE(SPR_GFZC,6,1,NULL,S_GFZDEBRIS8), // S_GFZDEBRIS7
STATE(SPR_GFZC,7,1,NULL,S_GFZDEBRIS1), // S_GFZDEBRIS8

// Bubble patch
STATE(SPR_BBLS,0,7,A_BubbleSpawn,S_BUBBLES2), // S_BUBBLES1
STATE(SPR_BBLS,1,7,NULL,S_BUBBLES3), // S_BUBBLES2
STATE(SPR_BBLS,2,7,A_BubbleSpawn,S_BUBBLES4), // S_BUBBLES3
STATE(SPR_BBLS,3,7,NULL,S_BUBBLES1), // S_BUBBLES4

// Bubbles
STATE(SPR_BUBL,0,1,A_BubbleRise,S_SMALLBUBBLE), // S_SMALLBUBBLE
STATE(SPR_BUBL,1,1,A_BubbleRise,S_MEDIUMBUBBLE), // S_MEDIUMBUBBLE

// Extra Large Bubble (breathable)
STATE(SPR_BUBL,2,8,A_BubbleRise,S_LARGEBUBBLE2), // S_LARGEBUBBLE1
STATE(SPR_BUBL,3,8,A_BubbleRise,S_EXTRALARGEBUBBLE), // S_LARGEBUBBLE2
STATE(SPR_BUBL,4,16,A_BubbleRise,S_EXTRALARGEBUBBLE), // S_EXTRALARGEBUBBLE

// Extra Large Bubble goes POP!
STATE(SPR_BUBL,5,16,NULL,S_NULL), // S_POP1

// Water Splish
STATE(SPR_SPLH,0,2,NULL,S_SPLISH2), // S_SPLISH1
STATE(SPR_SPLH,1,2,NULL,S_SPLISH3), // S_SPLISH2
STATE(SPR_SPLH,2,2,NULL,S_SPLISH4), // S_SPLISH3
STATE(SPR_SPLH,3,2,NULL,S_SPLISH5), // S_SPLISH4
STATE(SPR_SPLH,4,2,NULL,S_SPLISH6), // S_SPLISH5
STATE(SPR_SPLH,5,2,NULL,S_SPLISH7), // S_SPLISH6
STATE(SPR_SPLH,6,2,NULL,S_SPLISH8), // S_SPLISH7
STATE(SPR_SPLH,7,2,NULL,S_SPLISH9), // S_SPLISH8
STATE(SPR_SPLH,8,2,NULL,S_NULL),    // S_SPLISH9

// S3&K Boss Explosion
STATE(SPR_BOM3,FF_FULLBRIGHT,1,NULL,S_SONIC3KBOSSEXPLOSION2), // S_SONIC3KBOSSEXPLOSION1
STATE(SPR_BOM3,FF_FULLBRIGHT|1,1,NULL,S_SONIC3KBOSSEXPLOSION3), // S_SONIC3KBOSSEXPLOSION2
STATE(SPR_BOM3,FF_FULLBRIGHT|2,2,NULL,S_SONIC3KBOSSEXPLOSION4), // S_SONIC3KBOSSEXPLOSION3
STATE(SPR_BOM3,FF_FULLBRIGHT|3,2,NULL,S_SONIC3KBOSSEXPLOSION5), // S_SONIC3KBOSSEXPLOSION4
STATE(SPR_BOM3,FF_FULLBRIGHT|4,3,NULL,S_SONIC3KBOSSEXPLOSION6), // S_SONIC3KBOSSEXPLOSION5
STATE(SPR_BOM3,FF_FULLBRIGHT|5,4,NULL,S_NULL), // S_SONIC3KBOSSEXPLOSION6

STATE(SPR_JETF,FF_FULLBRIGHT,-1,NULL,S_NULL), // S_JETFUME1
STATE(SPR_FRET,0,2,NULL,S_NULL), // S_FRET

// Boss 1
STATE(SPR_EGGM,0,1,A_Boss1Chase,S_EGGMOBILE_STND),   // S_EGGMOBILE_STND
STATE(SPR_EGGM,17,TICRATE,A_FaceTarget,S_EGGMOBILE_ROFL1),   // S_EGGMOBILE_ROFL
STATE(SPR_EGGM,18,2,A_FaceTarget,S_EGGMOBILE_ROFL2),   // S_EGGMOBILE_ROFL1
STATE(SPR_EGGM,17,2,A_FaceTarget,S_EGGMOBILE_STND),   // S_EGGMOBILE_ROFL2
STATE(SPR_EGGM,1,3,A_FaceTarget,S_EGGMOBILE_LATK2),  // S_EGGMOBILE_LATK1
STATE(SPR_EGGM,2,15,NULL,S_EGGMOBILE_LATK3),  // S_EGGMOBILE_LATK2
STATE(SPR_EGGM,3,2,NULL,S_EGGMOBILE_LATK4),  // S_EGGMOBILE_LATK3
STATE(SPR_EGGM,4,2,A_FaceTarget,S_EGGMOBILE_LATK5),  // S_EGGMOBILE_LATK4
STATE2(SPR_EGGM,6,0,A_PrepareRepeat,45,0,S_EGGMOBILE_LATK6),  // S_EGGMOBILE_LATK5
STATE2(SPR_EGGM,5,1,A_Boss1Laser,MT_LASER,0,S_EGGMOBILE_LATK7),  // S_EGGMOBILE_LATK6
STATE2(SPR_EGGM,6,1,A_Boss1Laser,MT_LASER,(1<<8),S_EGGMOBILE_LATK8),  // S_EGGMOBILE_LATK7
STATE2(SPR_EGGM,5,0,A_Repeat,45,S_EGGMOBILE_LATK6,S_EGGMOBILE_LATK9),  // S_EGGMOBILE_LATK8
STATE(SPR_EGGM,8,2,NULL,S_EGGMOBILE_ROFL), // S_EGGMOBILE_LATK9
STATE(SPR_EGGM,9,3,A_FaceTarget,S_EGGMOBILE_RATK2),  // S_EGGMOBILE_RATK1
STATE(SPR_EGGM,10,5,NULL,S_EGGMOBILE_RATK3),  // S_EGGMOBILE_RATK2
STATE(SPR_EGGM,11,2,NULL,S_EGGMOBILE_RATK4),  // S_EGGMOBILE_RATK3
STATE(SPR_EGGM,12,2,A_FaceTarget,S_EGGMOBILE_RATK5),  // S_EGGMOBILE_RATK4
STATE2(SPR_EGGM,14,0,A_PrepareRepeat,45,0,S_EGGMOBILE_RATK6),  // S_EGGMOBILE_RATK5
STATE2(SPR_EGGM,13,1,A_Boss1Laser,MT_LASER,1,S_EGGMOBILE_RATK7),  // S_EGGMOBILE_RATK6
STATE2(SPR_EGGM,14,1,A_Boss1Laser,MT_LASER,1|(1<<8),S_EGGMOBILE_RATK8),  // S_EGGMOBILE_RATK7
STATE2(SPR_EGGM,13,0,A_Repeat,45,S_EGGMOBILE_RATK6,S_EGGMOBILE_RATK9),  // S_EGGMOBILE_RATK8
STATE(SPR_EGGM,16,2,NULL,S_EGGMOBILE_ROFL), // S_EGGMOBILE_RATK9
STATE2(SPR_EGGM,0,0,A_PrepareRepeat,45,0,S_EGGMOBILE_PANIC2),  // S_EGGMOBILE_PANIC1
STATE(SPR_EGGM,1,15,A_FaceTarget,S_EGGMOBILE_PANIC2A),  // S_EGGMOBILE_PANIC2
STATE(SPR_EGGM,2,3,A_FaceTarget,S_EGGMOBILE_PANIC2B),  // S_EGGMOBILE_PANIC2A
STATE(SPR_EGGM,3,3,A_FaceTarget,S_EGGMOBILE_PANIC2C),  // S_EGGMOBILE_PANIC2B
STATE(SPR_EGGM,4,3,A_FaceTarget,S_EGGMOBILE_PANIC3),  // S_EGGMOBILE_PANIC2C
STATE2(SPR_EGGM,7,1,A_Boss1Laser,MT_LASER,3,S_EGGMOBILE_PANIC4),  // S_EGGMOBILE_PANIC3
STATE2(SPR_EGGM,6,1,A_Boss1Laser,MT_LASER,3|(1<<8),S_EGGMOBILE_PANIC5),  // S_EGGMOBILE_PANIC4
STATE2(SPR_EGGM,6,0,A_Repeat,45,S_EGGMOBILE_PANIC3,S_EGGMOBILE_PANIC6),  // S_EGGMOBILE_PANIC5
STATE2(SPR_EGGM,0,0,A_PrepareRepeat,45,0,S_EGGMOBILE_PANIC7),  // S_EGGMOBILE_PANIC6
STATE(SPR_EGGM,9,15,A_FaceTarget,S_EGGMOBILE_PANIC7A),  // S_EGGMOBILE_PANIC7
STATE(SPR_EGGM,10,3,A_FaceTarget,S_EGGMOBILE_PANIC7B),  // S_EGGMOBILE_PANIC7A
STATE(SPR_EGGM,11,3,A_FaceTarget,S_EGGMOBILE_PANIC7C),  // S_EGGMOBILE_PANIC7B
STATE(SPR_EGGM,12,3,A_FaceTarget,S_EGGMOBILE_PANIC8),  // S_EGGMOBILE_PANIC7C
STATE2(SPR_EGGM,15,1,A_Boss1Laser,MT_LASER,3,S_EGGMOBILE_PANIC9),  // S_EGGMOBILE_PANIC8
STATE2(SPR_EGGM,14,1,A_Boss1Laser,MT_LASER,3|(1<<8),S_EGGMOBILE_PANIC10),  // S_EGGMOBILE_PANIC9
STATE2(SPR_EGGM,14,0,A_Repeat,45,S_EGGMOBILE_PANIC8,S_EGGMOBILE_PANIC11), // S_EGGMOBILE_PANIC10
STATE2(SPR_EGGM,0,0,A_PrepareRepeat,45,0,S_EGGMOBILE_PANIC12), // S_EGGMOBILE_PANIC11
STATE(SPR_EGGM,1,15,A_FaceTarget,S_EGGMOBILE_PANIC12A), // S_EGGMOBILE_PANIC12
STATE(SPR_EGGM,2,3,A_FaceTarget,S_EGGMOBILE_PANIC12B), // S_EGGMOBILE_PANIC12A
STATE(SPR_EGGM,3,3,A_FaceTarget,S_EGGMOBILE_PANIC12C), // S_EGGMOBILE_PANIC12B
STATE(SPR_EGGM,4,3,A_FaceTarget,S_EGGMOBILE_PANIC13), // S_EGGMOBILE_PANIC12C
STATE2(SPR_EGGM,7,1,A_Boss1Laser,MT_LASER,3,S_EGGMOBILE_PANIC14), // S_EGGMOBILE_PANIC13
STATE2(SPR_EGGM,6,1,A_Boss1Laser,MT_LASER,3|(1<<8),S_EGGMOBILE_PANIC15), // S_EGGMOBILE_PANIC14
STATE2(SPR_EGGM,6,0,A_Repeat,45,S_EGGMOBILE_PANIC13,S_EGGMOBILE_ROFL), // S_EGGMOBILE_PANIC15
STATE(SPR_EGGM,19,24,A_Pain,S_EGGMOBILE_PAIN2),  // S_EGGMOBILE_PAIN
STATE2(SPR_EGGM,19,16,A_SkullAttack,3,1,S_EGGMOBILE_STND),   // S_EGGMOBILE_PAIN2
STATE2(SPR_EGGM,20,2,A_Fall,17,0,S_EGGMOBILE_DIE2),   // S_EGGMOBILE_DIE1
STATE(SPR_EGGM,20,2,A_BossScream,S_EGGMOBILE_DIE3),   // S_EGGMOBILE_DIE2
STATE2(SPR_EGGM,20,0,A_Repeat,17,S_EGGMOBILE_DIE2,S_EGGMOBILE_DIE4),   // S_EGGMOBILE_DIE3
STATE(SPR_EGGM,20,-1,A_BossDeath,S_NULL),             // S_EGGMOBILE_DIE4
STATE2(SPR_EGGM,21,2,A_BossScream,90,MT_EXPLODE,S_EGGMOBILE_FLEE2),  // S_EGGMOBILE_FLEE1
STATE(SPR_EGGM,21,2,A_BossScream,S_EGGMOBILE_FLEE1),  // S_EGGMOBILE_FLEE2

STATE(SPR_PLAY,0,1,A_FocusTarget,S_EGGMOBILE_TARGET), // S_EGGMOBILE_TARGET

STATE(SPR_EGLZ,0,TICRATE,NULL,S_NULL), // S_BOSSEGLZ1
STATE(SPR_EGLZ,1,TICRATE,NULL,S_NULL), // S_BOSSEGLZ2

STATE(SPR_LASR,FF_FULLBRIGHT|0,2,NULL,S_NULL), // S_LASER
STATE(SPR_LASR,FF_FULLBRIGHT|1,2,NULL,S_NULL), // S_LASER2

STATE(SPR_LASF,FF_FULLBRIGHT|0,2,NULL,S_LASERFLAME2), // S_LASERFLAME1
STATE2(SPR_LASF,FF_FULLBRIGHT|1,1,A_ChangeHeight,156,3,S_LASERFLAME3), // S_LASERFLAME2
STATE2(SPR_LASF,FF_FULLBRIGHT|2,0,A_ChangeHeight,32,3,S_LASERFLAME4), // S_LASERFLAME3
STATE(SPR_LASF,FF_FULLBRIGHT|2,4,NULL,S_LASERFLAME4A), // S_LASERFLAME4
STATE(SPR_LASF,FF_FULLBRIGHT|3,2,NULL,S_NULL), // S_LASERFLAME4A

STATE(SPR_DUST,0,4,NULL,S_DUST2), // S_DUST1
STATE(SPR_DUST,1,5,NULL,S_DUST3), // S_DUST2
STATE(SPR_DUST,2,3,NULL,S_DUST4), // S_DUST3
STATE(SPR_DUST,3,2,NULL,S_NULL),  // S_DUST4

// Starpost
STATE(SPR_STPT,0,-1,NULL,S_STARPOST_IDLE), // S_STARPOST_IDLE
STATE(SPR_STPT,7,1,NULL,S_STARPOST_FLASH2), // S_STARPOST_FLASH1
STATE(SPR_STPT,8,1,NULL,S_STARPOST_FLASH3), // S_STARPOST_FLASH2
STATE(SPR_STPT,9,1,NULL,S_STARPOST_FLASH4), // S_STARPOST_FLASH3
STATE(SPR_STPT,8,1,NULL,S_STARPOST_FLASH1), // S_STARPOST_FLASH4
STATE(SPR_STPT,1,1,NULL,S_STARPOST_SPIN2), // S_STARPOST_SPIN1
STATE(SPR_STPT,2,1,NULL,S_STARPOST_SPIN3), // S_STARPOST_SPIN2
STATE(SPR_STPT,3,1,NULL,S_STARPOST_SPIN4), // S_STARPOST_SPIN3
STATE(SPR_STPT,4,1,NULL,S_STARPOST_SPIN5), // S_STARPOST_SPIN4
STATE(SPR_STPT,5,1,NULL,S_STARPOST_SPIN6), // S_STARPOST_SPIN5
STATE(SPR_STPT,6,1,NULL,S_STARPOST_SPIN7), // S_STARPOST_SPIN6
STATE(SPR_STPT,FF_FLIPPED|5,1,NULL,S_STARPOST_SPIN8), // S_STARPOST_SPIN7
STATE(SPR_STPT,FF_FLIPPED|4,1,NULL,S_STARPOST_SPIN9), // S_STARPOST_SPIN8
STATE(SPR_STPT,FF_FLIPPED|3,1,NULL,S_STARPOST_SPIN10), // S_STARPOST_SPIN9
STATE(SPR_STPT,FF_FLIPPED|2,1,NULL,S_STARPOST_SPIN11), // S_STARPOST_SPIN10
STATE(SPR_STPT,FF_FLIPPED|1,1,NULL,S_STARPOST_SPIN12), // S_STARPOST_SPIN11
STATE(SPR_STPT,0,1,NULL,S_STARPOST_SPIN13), // S_STARPOST_SPIN12
STATE(SPR_STPT,1,1,NULL,S_STARPOST_SPIN14), // S_STARPOST_SPIN13
STATE(SPR_STPT,2,1,NULL,S_STARPOST_SPIN15), // S_STARPOST_SPIN14
STATE(SPR_STPT,3,1,NULL,S_STARPOST_SPIN16), // S_STARPOST_SPIN15
STATE(SPR_STPT,4,1,NULL,S_STARPOST_SPIN17), // S_STARPOST_SPIN16
STATE(SPR_STPT,5,1,NULL,S_STARPOST_SPIN18), // S_STARPOST_SPIN17
STATE(SPR_STPT,6,1,NULL,S_STARPOST_SPIN19), // S_STARPOST_SPIN18
STATE(SPR_STPT,FF_FLIPPED|5,1,NULL,S_STARPOST_SPIN20), // S_STARPOST_SPIN19
STATE(SPR_STPT,FF_FLIPPED|4,1,NULL,S_STARPOST_SPIN21), // S_STARPOST_SPIN20
STATE(SPR_STPT,FF_FLIPPED|3,1,NULL,S_STARPOST_SPIN22), // S_STARPOST_SPIN21
STATE(SPR_STPT,FF_FLIPPED|2,1,NULL,S_STARPOST_SPIN23), // S_STARPOST_SPIN22
STATE(SPR_STPT,FF_FLIPPED|1,1,NULL,S_STARPOST_FLASH1), // S_STARPOST_SPIN23

STATE(SPR_DRWN,0,40,NULL,S_NULL), // S_ZERO
STATE(SPR_DRWN,1,40,NULL,S_NULL), // S_ONE
STATE(SPR_DRWN,2,40,NULL,S_NULL), // S_TWO
STATE(SPR_DRWN,3,40,NULL,S_NULL), // S_THREE
STATE(SPR_DRWN,4,40,NULL,S_NULL), // S_FOUR
STATE(SPR_DRWN,5,40,NULL,S_NULL), // S_FIVE

STATE(SPR_SIGN,0,-1,NULL,S_NULL), // S_SIGN
STATE(SPR_SIGN,1,1,A_SignSpin,S_SIGN2), // S_SIGN1
STATE(SPR_SIGN,2,1,NULL,S_SIGN3), // S_SIGN2
STATE(SPR_SIGN,FF_FLIPPED|1,1,NULL,S_SIGN4), // S_SIGN3
STATE(SPR_SIGN,3,1,NULL,S_SIGN5), // S_SIGN4
STATE(SPR_SIGN,1,1,NULL,S_SIGN6), // S_SIGN5
STATE(SPR_SIGN,2,1,NULL,S_SIGN7), // S_SIGN6
STATE(SPR_SIGN,FF_FLIPPED|1,1,NULL,S_SIGN1), // S_SIGN7
STATE(SPR_SIGN,3,-1,NULL,S_NULL), // S_SIGNSTOP

};

#undef STATE
#undef STATE2

const mobjinfo_t mobjinfo[NUMMOBJTYPES] = {

{		/* MT_PLAYER */
-1,		/* doomednum */
S_PLAY_STND,		/* spawnstate */
1,		/* spawnhealth */
S_PLAY_RUN1,		/* seestate */
sfx_None,		/* seesound */
0,		/* reactiontime */
sfx_s3k_3b,		/* attacksound */
S_PLAY_PAIN,		/* painstate */
255,		/* painchance */
sfx_s3k_b9,		/* painsound */
S_NULL,		/* meleestate */
S_PLAY_ATK1,		/* missilestate */
S_PLAY_DIE,		/* deathstate */
S_PLAY_DROWN,		/* xdeathstate */
sfx_s3k_35,		/* deathsound */
0,		/* speed */
16*FRACUNIT,		/* radius */
48*FRACUNIT,		/* height */
100,		/* mass */
0,		/* damage */
sfx_None,		/* activesound */
MF_SOLID,		/* flags */
MF2_SHOOTABLE, // flags2
 },

 {           // MT_RING_BOX
		400,            // doomednum
		S_RING_BOX,     // spawnstate
		1,              // spawnhealth
		S_NULL,         // seestate
		sfx_None,       // seesound
		8,              // reactiontime
		sfx_None,       // attacksound
		S_RING_BOX,     // painstate
		0,              // painchance
		sfx_None,       // painsound
		S_NULL,         // meleestate
		S_NULL,         // missilestate
		S_BOX_POP1,     // deathstate
		S_NULL,         // xdeathstate
		sfx_s3k_3d,     // deathsound
		1,              // speed
		18*FRACUNIT,    // radius
		40*FRACUNIT,    // height
		100,            // mass
		MT_RING_ICON,   // damage
		sfx_None,       // activesound
		MF_SOLID|MF_STATIC, // flags
		MF2_SHOOTABLE, // flags2
	},
	{           // MT_ATTRACT_BOX
		402,            // doomednum
		S_ATTRACT_BOX,  // spawnstate
		1,              // spawnhealth
		S_NULL,         // seestate
		sfx_None,       // seesound
		8,              // reactiontime
		sfx_None,       // attacksound
		S_ATTRACT_BOX,  // painstate
		0,              // painchance
		sfx_None,       // painsound
		S_NULL,         // meleestate
		S_NULL,         // missilestate
		S_BOX_POP1,     // deathstate
		S_NULL,         // xdeathstate
		sfx_s3k_3d,        // deathsound
		1,              // speed
		18*FRACUNIT,    // radius
		40*FRACUNIT,    // height
		100,            // mass
		MT_ATTRACT_ICON,// damage
		sfx_None,       // activesound
		MF_SOLID|MF_STATIC, // flags
		MF2_SHOOTABLE, // flags2
	},

	{           // MT_FORCE_BOX
		403,            // doomednum
		S_FORCE_BOX,    // spawnstate
		1,              // spawnhealth
		S_NULL,         // seestate
		sfx_None,       // seesound
		8,              // reactiontime
		sfx_None,       // attacksound
		S_FORCE_BOX,    // painstate
		0,              // painchance
		sfx_None,       // painsound
		S_NULL,         // meleestate
		S_NULL,         // missilestate
		S_BOX_POP1,     // deathstate
		S_NULL,         // xdeathstate
		sfx_s3k_3d,        // deathsound
		1,              // speed
		18*FRACUNIT,    // radius
		40*FRACUNIT,    // height
		100,            // mass
		MT_FORCE_ICON,  // damage
		sfx_None,       // activesound
		MF_SOLID|MF_STATIC, // flags
		MF2_SHOOTABLE, // flags2
	},

	{           // MT_ARMAGEDDON_BOX
		404,            // doomednum
		S_ARMAGEDDON_BOX, // spawnstate
		1,              // spawnhealth
		S_NULL,         // seestate
		sfx_None,       // seesound
		8,              // reactiontime
		sfx_None,       // attacksound
		S_ARMAGEDDON_BOX, // painstate
		0,              // painchance
		sfx_None,       // painsound
		S_NULL,         // meleestate
		S_NULL,         // missilestate
		S_BOX_POP1,     // deathstate
		S_NULL,         // xdeathstate
		sfx_s3k_3d,        // deathsound
		1,              // speed
		18*FRACUNIT,    // radius
		40*FRACUNIT,    // height
		100,            // mass
		MT_ARMAGEDDON_ICON, // damage
		sfx_None,       // activesound
		MF_SOLID|MF_STATIC, // flags
		MF2_SHOOTABLE, // flags2
	},

	{           // MT_WHIRLWIND_BOX
		405,            // doomednum
		S_WHIRLWIND_BOX, // spawnstate
		1,              // spawnhealth
		S_NULL,         // seestate
		sfx_None,       // seesound
		8,              // reactiontime
		sfx_None,       // attacksound
		S_WHIRLWIND_BOX, // painstate
		0,              // painchance
		sfx_None,       // painsound
		S_NULL,         // meleestate
		S_NULL,         // missilestate
		S_BOX_POP1,     // deathstate
		S_NULL,         // xdeathstate
		sfx_s3k_3d,        // deathsound
		1,              // speed
		18*FRACUNIT,    // radius
		40*FRACUNIT,    // height
		100,            // mass
		MT_WHIRLWIND_ICON, // damage
		sfx_None,       // activesound
		MF_SOLID|MF_STATIC, // flags
		MF2_SHOOTABLE, // flags2
	},

	{           // MT_ELEMENTAL_BOX
		406,            // doomednum
		S_ELEMENTAL_BOX,     // spawnstate
		1,              // spawnhealth
		S_NULL,         // seestate
		sfx_None,       // seesound
		8,              // reactiontime
		sfx_None,       // attacksound
		S_ELEMENTAL_BOX,     // painstate
		0,              // painchance
		sfx_None,       // painsound
		S_NULL,         // meleestate
		S_NULL,         // missilestate
		S_BOX_POP1,     // deathstate
		S_NULL,         // xdeathstate
		sfx_s3k_3d,        // deathsound
		1,              // speed
		18*FRACUNIT,    // radius
		40*FRACUNIT,    // height
		100,            // mass
		MT_ELEMENTAL_ICON, // damage
		sfx_None,       // activesound
		MF_SOLID|MF_STATIC, // flags
		MF2_SHOOTABLE, // flags2
	},

	{           // MT_SNEAKERS_BOX
		407,            // doomednum
		S_SNEAKERS_BOX, // spawnstate
		1,              // spawnhealth
		S_NULL,         // seestate
		sfx_None,       // seesound
		8,              // reactiontime
		sfx_None,       // attacksound
		S_SNEAKERS_BOX, // painstate
		0,              // painchance
		sfx_None,       // painsound
		S_NULL,         // meleestate
		S_NULL,         // missilestate
		S_BOX_POP1,     // deathstate
		S_NULL,         // xdeathstate
		sfx_s3k_3d,        // deathsound
		1,              // speed
		18*FRACUNIT,    // radius
		40*FRACUNIT,    // height
		100,            // mass
		MT_SNEAKERS_ICON, // damage
		sfx_None,       // activesound
		MF_SOLID|MF_STATIC, // flags
		MF2_SHOOTABLE, // flags2
	},

	{           // MT_INVULN_BOX
		408,            // doomednum
		S_INVULN_BOX,   // spawnstate
		1,              // spawnhealth
		S_NULL,         // seestate
		sfx_None,       // seesound
		8,              // reactiontime
		sfx_None,       // attacksound
		S_INVULN_BOX,   // painstate
		0,              // painchance
		sfx_None,       // painsound
		S_NULL,         // meleestate
		S_NULL,         // missilestate
		S_BOX_POP1,     // deathstate
		S_NULL,         // xdeathstate
		sfx_s3k_3d,        // deathsound
		1,              // speed
		18*FRACUNIT,    // radius
		40*FRACUNIT,    // height
		100,            // mass
		MT_INVULN_ICON, // damage
		sfx_None,       // activesound
		MF_SOLID|MF_STATIC, // flags
		MF2_SHOOTABLE, // flags2
	},
	{           // MT_1UP_BOX
		409,            // doomednum
		S_1UP_BOX,      // spawnstate
		1,              // spawnhealth
		S_NULL,         // seestate
		sfx_None,       // seesound
		8,              // reactiontime
		sfx_None,       // attacksound
		S_1UP_BOX,      // painstate
		0,              // painchance
		sfx_None,       // painsound
		S_NULL,         // meleestate
		S_NULL,         // missilestate
		S_BOX_POP1,     // deathstate
		S_NULL,         // xdeathstate
		sfx_s3k_3d,        // deathsound
		1,              // speed
		18*FRACUNIT,    // radius
		40*FRACUNIT,    // height
		100,            // mass
		MT_1UP_ICON,    // damage
		sfx_None,       // activesound
		MF_SOLID|MF_STATIC, // flags
		MF2_SHOOTABLE, // flags2
	},
	{           // MT_RING_ICON
		-1,              // doomednum
		S_RING_ICON1,    // spawnstate
		1,               // spawnhealth
		S_NULL,          // seestate
		sfx_s3k_33,      // seesound
		10,              // reactiontime
		sfx_None,        // attacksound
		S_NULL,          // painstate
		0,               // painchance
		sfx_None,        // painsound
		S_NULL,          // meleestate
		S_NULL,          // missilestate
		S_NULL,          // deathstate
		S_NULL,          // xdeathstate
		sfx_None,        // deathsound
		4*FRACUNIT,      // speed
		8*FRACUNIT,      // radius
		14*FRACUNIT,     // height
		100,             // mass
		62,     // damage
		sfx_None,        // activesound
		MF_NOBLOCKMAP|MF_NOCLIP|MF_NOGRAVITY, // flags
		0, // flags2
	},
	{           // MT_ATTRACT_ICON
		-1,             // doomednum
		S_ATTRACT_ICON1, // spawnstate
		1,              // spawnhealth
		S_NULL,         // seestate
		sfx_attrsg,     // seesound
		8,              // reactiontime
		sfx_None,       // attacksound
		S_NULL,         // painstate
		MT_ATTRACT_ORB, // painchance
		sfx_None,       // painsound
		S_NULL,         // meleestate
		S_NULL,         // missilestate
		S_NULL,         // deathstate
		S_NULL,         // xdeathstate
		sfx_None,       // deathsound
		4*FRACUNIT,     // speed
		8*FRACUNIT,     // radius
		14*FRACUNIT,    // height
		100,            // mass
		62,    // damage
		sfx_None,       // activesound
		MF_NOBLOCKMAP|MF_NOCLIP|MF_NOGRAVITY, // flags
		0, // flags2
	},
	{           // MT_FORCE_ICON
		-1,             // doomednum
		S_FORCE_ICON1,  // spawnstate
		1,              // spawnhealth
		S_NULL,         // seestate
		sfx_forcsg,     // seesound
		8,              // reactiontime
		sfx_None,       // attacksound
		S_NULL,         // painstate
		MT_FORCE_ORB,   // painchance
		sfx_None,       // painsound
		S_NULL,         // meleestate
		S_NULL,         // missilestate
		S_NULL,         // deathstate
		S_NULL,         // xdeathstate
		sfx_None,       // deathsound
		4*FRACUNIT,     // speed
		8*FRACUNIT,     // radius
		14*FRACUNIT,    // height
		100,            // mass
		62,    // damage
		sfx_None,       // activesound
		MF_NOBLOCKMAP|MF_NOCLIP|MF_NOGRAVITY, // flags
		0, // flags2
	},
	{           // MT_ARMAGEDDON_ICON
		-1,             // doomednum
		S_ARMAGEDDON_ICON1, // spawnstate
		1,              // spawnhealth
		S_NULL,         // seestate
		sfx_armasg,     // seesound
		8,              // reactiontime
		sfx_None,       // attacksound
		S_NULL,         // painstate
		MT_ARMAGEDDON_ORB, // painchance
		sfx_None,       // painsound
		S_NULL,         // meleestate
		S_NULL,         // missilestate
		S_NULL,         // deathstate
		S_NULL,         // xdeathstate
		sfx_None,       // deathsound
		4*FRACUNIT,     // speed
		8*FRACUNIT,     // radius
		14*FRACUNIT,    // height
		100,            // mass
		62,    // damage
		sfx_None,       // activesound
		MF_NOBLOCKMAP|MF_NOCLIP|MF_NOGRAVITY, // flags
		0, // flags2
	},
	{           // MT_WHIRLWIND_ICON
		-1,             // doomednum
		S_WHIRLWIND_ICON1, // spawnstate
		1,              // spawnhealth
		S_NULL,         // seestate
		sfx_wirlsg,     // seesound
		8,              // reactiontime
		sfx_None,       // attacksound
		S_NULL,         // painstate
		MT_WHIRLWIND_ORB, // painchance
		sfx_None,       // painsound
		S_NULL,         // meleestate
		S_NULL,         // missilestate
		S_NULL,         // deathstate
		S_NULL,         // xdeathstate
		sfx_None,       // deathsound
		4*FRACUNIT,     // speed
		8*FRACUNIT,     // radius
		14*FRACUNIT,    // height
		100,            // mass
		62,    // damage
		sfx_None,       // activesound
		MF_NOBLOCKMAP|MF_NOCLIP|MF_NOGRAVITY, // flags
		0, // flags2
	},
	{           // MT_ELEMENTAL_ICON
		-1,             // doomednum
		S_ELEMENTAL_ICON1, // spawnstate
		1,              // spawnhealth
		S_NULL,         // seestate
		sfx_elemsg,      // seesound
		8,              // reactiontime
		sfx_None,       // attacksound
		S_NULL,         // painstate
		MT_ELEMENTAL_ORB, // painchance
		sfx_None,       // painsound
		S_NULL,         // meleestate
		S_NULL,         // missilestate
		S_NULL,         // deathstate
		S_NULL,         // xdeathstate
		sfx_None,       // deathsound
		4*FRACUNIT,     // speed
		8*FRACUNIT,     // radius
		14*FRACUNIT,    // height
		100,            // mass
		62,    // damage
		sfx_None,       // activesound
		MF_NOBLOCKMAP|MF_NOCLIP|MF_NOGRAVITY, // flags
		0, // flags2
	},
	{           // MT_SNEAKERS_ICON
		-1,             // doomednum
		S_SNEAKERS_ICON1, // spawnstate
		1,              // spawnhealth
		S_NULL,         // seestate
		sfx_None,       // seesound
		8,              // reactiontime
		sfx_None,       // attacksound
		S_NULL,         // painstate
		0,              // painchance
		sfx_None,       // painsound
		S_NULL,         // meleestate
		S_NULL,         // missilestate
		S_NULL,         // deathstate
		S_NULL,         // xdeathstate
		sfx_None,       // deathsound
		4*FRACUNIT,     // speed
		8*FRACUNIT,     // radius
		14*FRACUNIT,    // height
		100,            // mass
		62,    // damage
		sfx_None,       // activesound
		MF_NOBLOCKMAP|MF_NOCLIP|MF_NOGRAVITY, // flags
		0, // flags2
	},
	{           // MT_INVULN_ICON
		-1,             // doomednum
		S_INVULN_ICON1, // spawnstate
		1,              // spawnhealth
		S_NULL,         // seestate
		sfx_None,       // seesound
		8,              // reactiontime
		sfx_None,       // attacksound
		S_NULL,         // painstate
		0,              // painchance
		sfx_None,       // painsound
		S_NULL,         // meleestate
		S_NULL,         // missilestate
		S_NULL,         // deathstate
		S_NULL,         // xdeathstate
		sfx_None,       // deathsound
		4*FRACUNIT,     // speed
		8*FRACUNIT,     // radius
		14*FRACUNIT,    // height
		100,            // mass
		62,             // damage
		sfx_None,       // activesound
		MF_NOBLOCKMAP|MF_NOCLIP|MF_NOGRAVITY, // flags
		0, // flags2
	},
	{           // MT_1UP_ICON
		-1,             // doomednum
		S_1UP_ICON1,    // spawnstate
		1,              // spawnhealth
		S_NULL,         // seestate
		sfx_None,       // seesound
		8,              // reactiontime
		sfx_None,       // attacksound
		S_NULL,         // painstate
		0,              // painchance
		sfx_None,       // painsound
		S_NULL,         // meleestate
		S_NULL,         // missilestate
		S_NULL,         // deathstate
		S_NULL,         // xdeathstate
		sfx_None,       // deathsound
		4*FRACUNIT,     // speed
		8*FRACUNIT,     // radius
		14*FRACUNIT,    // height
		100,            // mass
		62,             // damage
		sfx_None,       // activesound
		MF_NOBLOCKMAP|MF_NOCLIP|MF_NOGRAVITY, // flags
		0, // flags2
	},
	{           // MT_EGGMOBILE
		200,               // doomednum
		S_EGGMOBILE_STND,  // spawnstate
		8,                 // spawnhealth
		S_EGGMOBILE_STND,  // seestate
		sfx_None,          // seesound
		45,                // reactiontime
		sfx_None,          // attacksound
		S_EGGMOBILE_PAIN,  // painstate
		0,                 // painchance
		sfx_s3k_6e,        // painsound
		S_EGGMOBILE_LATK1, // meleestate
		S_EGGMOBILE_RATK1, // missilestate
		S_EGGMOBILE_DIE1,  // deathstate
		S_EGGMOBILE_FLEE1, // xdeathstate
		sfx_s3k_b4,         // deathsound
		4,                 // speed
		36*FRACUNIT,       // radius
		84*FRACUNIT,       // height
		S_EGGMOBILE_PANIC1,// mass
		3,                 // damage
		sfx_telept,        // activesound
		MF_SPECIAL|MF_NOGRAVITY, // flags
		MF2_SHOOTABLE|MF2_FLOAT|MF2_ENEMY, // flags2
	},
	{           // MT_STARPOST
		502,            // doomednum
		S_STARPOST_IDLE, // spawnstate
		1,              // spawnhealth
		S_STARPOST_FLASH1, // seestate
		sfx_None,       // seesound
		8,              // reactiontime
		sfx_None,       // attacksound
		S_STARPOST_SPIN1, // painstate
		0,              // painchance
		sfx_s3k_63,     // painsound
		S_NULL,         // meleestate
		S_NULL,         // missilestate
		S_NULL,         // deathstate
		S_NULL,         // xdeathstate
		sfx_None,       // deathsound
		8,              // speed
		64*FRACUNIT,    // radius
		128*FRACUNIT,   // height
		4,              // mass
		0,              // damage
		sfx_None,       // activesound
		MF_SPECIAL|MF_STATIC,     // flags
		0, // flags2
	},
	{           // MT_DROWNNUMBERS
		-1,             // doomednum
		S_ZERO,         // spawnstate
		1000,           // spawnhealth
		S_NULL,         // seestate
		sfx_None,       // seesound
		8,              // reactiontime
		sfx_None,       // attacksound
		S_NULL,         // painstate
		0,              // painchance
		sfx_None,       // painsound
		S_NULL,         // meleestate
		S_NULL,         // missilestate
		S_NULL,         // deathstate
		S_NULL,         // xdeathstate
		sfx_None,       // deathsound
		8,              // speed
		8*FRACUNIT,     // radius
		8*FRACUNIT,     // height
		16,             // mass
		0,              // damage
		sfx_None,       // activesound
		MF_NOBLOCKMAP|MF_NOCLIP|MF_NOGRAVITY, // flags
		MF2_FORWARDOFFSET, // flags2
	},
{           // MT_SCORE
	-1,             // doomednum
	S_SCORE100,         // spawnstate
	1,              // spawnhealth
	S_NULL,         // seestate
	sfx_None,       // seesound
	8,              // reactiontime
	sfx_None,       // attacksound
	S_NULL,         // painstate
	0,              // painchance
	sfx_None,       // painsound
	S_NULL,         // meleestate
	S_NULL,         // missilestate
	S_NULL,         // deathstate
	S_NULL,         // xdeathstate
	sfx_None,       // deathsound
	4*FRACUNIT,     // speed
	8*FRACUNIT,     // radius
	8*FRACUNIT,     // height
	100,            // mass
	0,              // damage
	sfx_None,       // activesound
	MF_NOBLOCKMAP|MF_NOCLIP|MF_NOGRAVITY|MF_STATIC, // flags
	0, // flags2
},
	{           // MT_SIGN
		501,            // doomednum
		S_SIGN,         // spawnstate
		1000,           // spawnhealth
		S_SIGN1,        // seestate
		sfx_lvpass,     // seesound
		8,              // reactiontime
		sfx_None,       // attacksound
		S_NULL,   // painstate
		MT_SPARK,       // painchance
		sfx_None,      // painsound
		S_NULL,   // meleestate
		S_NULL,    // missilestate
		S_SIGNSTOP,     // deathstate
		S_NULL,         // xdeathstate
		sfx_s3k_64,      // deathsound
		8,              // speed
		36*FRACUNIT,    // radius
		32*FRACUNIT,    // height
		16,             // mass
		0,              // damage
		sfx_None,       // activesound
		MF_NOCLIP|MF_NOBLOCKMAP, // flags
		0, // flags2
	},

 {		/* MT_CAMERA */
-1,		/* doomednum */
S_NULL,		/* spawnstate */
20,		/* spawnhealth */
S_NULL,		/* seestate */
sfx_None,		/* seesound */
8,		/* reactiontime */
sfx_None,		/* attacksound */
S_NULL,		/* painstate */
200,		/* painchance */
sfx_None,		/* painsound */
0,		/* meleestate */
S_NULL,		/* missilestate */
S_NULL,		/* deathstate */
S_NULL,		/* xdeathstate */
sfx_None,		/* deathsound */
8,		/* speed */
20*FRACUNIT,		/* radius */
12*FRACUNIT,		/* height */
100,		/* mass */
0,		/* damage */
sfx_None,		/* activesound */
0,		/* flags */
0,	// flags2
 },

 {           // MT_EXPLODE
	-1,             // doomednum
	S_XPLD1,        // spawnstate
	1,              // spawnhealth
	S_NULL,         // seestate
	sfx_None,       // seesound
	32,             // reactiontime
	sfx_None,       // attacksound
	S_NULL,         // painstate
	200,            // painchance
	sfx_None,       // painsound
	S_NULL,         // meleestate
	S_NULL,         // missilestate
	S_NULL,         // deathstate
	S_NULL,         // xdeathstate
	sfx_None,       // deathsound
	1*FRACUNIT,     // speed
	8*FRACUNIT,     // radius
	16*FRACUNIT,    // height
	100,            // mass
	0,              // damage
	sfx_None,       // activesound
	MF_NOBLOCKMAP|MF_NOCLIP|MF_NOGRAVITY|MF_STATIC, // flags
	MF2_NARROWGFX, // flags2
},

{		/* MT_POSSESSED */
100,		/* doomednum */
S_POSS_STND,		/* spawnstate */
1,		/* spawnhealth */
S_POSS_RUN1,		/* seestate */
sfx_None,		/* seesound */
16,		/* reactiontime */
sfx_None,		/* attacksound */
S_NULL,		/* painstate */
200,		/* painchance */
sfx_None,		/* painsound */
0,		/* meleestate */
S_NULL,		/* missilestate */
S_XPLD_FLICKY,		/* deathstate */
S_NULL,		/* xdeathstate */
sfx_s3k_3d,		/* deathsound */
2,		/* speed */
24*FRACUNIT,		/* radius */
32*FRACUNIT,		/* height */
100,		/* mass */
0,		/* damage */
sfx_None,		/* activesound */
0,		/* flags */
MF2_SHOOTABLE|MF2_ENEMY,	// flags2
 },

 	{           // MT_GFZFISH
		102,            // doomednum
		S_FISH2,        // spawnstate
		1,              // spawnhealth
		S_FISH1,        // seestate
		sfx_None,       // seesound
		8,              // reactiontime
		sfx_None,       // attacksound
		S_NULL,         // painstate
		0,              // painchance
		sfx_None,       // painsound
		S_FISH3,        // meleestate
		S_NULL,         // missilestate
		S_XPLD_FLICKY,  // deathstate
		S_FISH4,        // xdeathstate
		sfx_s3k_3d,     // deathsound
		0,              // speed
		8*FRACUNIT,     // radius
		28*FRACUNIT,    // height
		100,            // mass
		1,              // damage
		sfx_None,       // activesound
		0, // flags
		MF2_ENEMY|MF2_SHOOTABLE, // flags2
	},

{           // MT_RING
	300,            // doomednum
	S_RING1,         // spawnstate
	1000,           // spawnhealth
	S_NULL,         // seestate
	sfx_None,       // seesound
	0,//MT_FLINGRING,   // reactiontime
	sfx_None,       // attacksound
	S_NULL,         // painstate
	0,              // painchance
	sfx_None,       // painsound
	S_NULL,         // meleestate
	S_NULL,         // missilestate
	S_NULL,//S_SPRK1,        // deathstate
	S_NULL,         // xdeathstate
	sfx_s3k_33,     // deathsound
	38,    // speed
	16*FRACUNIT,    // radius
	24*FRACUNIT,    // height
	100,            // mass
	0,              // damage
	sfx_None,       // activesound
	MF_SPECIAL|MF_NOGRAVITY|MF_STATIC|MF_RINGMOBJ, // flags
	0, // flags2
},
{           // MT_SPARK
	-1,            // doomednum
	S_SPARK1,         // spawnstate
	1000,           // spawnhealth
	S_NULL,         // seestate
	sfx_None,       // seesound
	0,//MT_FLINGRING,   // reactiontime
	sfx_None,       // attacksound
	S_NULL,         // painstate
	0,              // painchance
	sfx_None,       // painsound
	S_NULL,         // meleestate
	S_NULL,         // missilestate
	S_NULL,//S_SPRK1,        // deathstate
	S_NULL,         // xdeathstate
	sfx_None,     // deathsound
	38,    // speed
	16*FRACUNIT,    // radius
	24*FRACUNIT,    // height
	100,            // mass
	0,              // damage
	sfx_None,       // activesound
	MF_NOGRAVITY|MF_STATIC|MF_NOBLOCKMAP, // flags
	0, // flags2
},
{           // MT_FLINGRING
	-1,            // doomednum
	S_RING1,         // spawnstate
	4*TICRATE,           // spawnhealth
	S_NULL,         // seestate
	sfx_None,       // seesound
	0,//MT_FLINGRING,   // reactiontime
	sfx_None,       // attacksound
	S_NULL,         // painstate
	0,              // painchance
	sfx_None,       // painsound
	S_NULL,         // meleestate
	S_NULL,         // missilestate
	S_NULL,//S_SPRK1,        // deathstate
	S_NULL,         // xdeathstate
	sfx_s3k_33,     // deathsound
	38,    // speed
	16*FRACUNIT,    // radius
	24*FRACUNIT,    // height
	100,            // mass
	0,              // damage
	sfx_None,       // activesound
	MF_SPECIAL, // flags
	MF2_FLOAT, // flags2
},
{           // MT_ATTRACTRING
	-1,            // doomednum
	S_RING1,         // spawnstate
	1,      		// spawnhealth
	S_NULL,         // seestate
	sfx_None,       // seesound
	0,//MT_FLINGRING,   // reactiontime
	sfx_None,       // attacksound
	S_NULL,         // painstate
	0,              // painchance
	sfx_None,       // painsound
	S_NULL,         // meleestate
	S_NULL,         // missilestate
	S_NULL,//S_SPRK1,        // deathstate
	S_NULL,         // xdeathstate
	sfx_s3k_33,     // deathsound
	10,    // speed
	16*FRACUNIT,    // radius
	24*FRACUNIT,    // height
	100,            // mass
	0,              // damage
	sfx_None,       // activesound
	MF_SPECIAL, // flags
	MF2_FLOAT, // flags2
},
{           // MT_GFZFLOWERA
	800,            // doomednum
	S_GFZFLOWERA1,         // spawnstate
	1000,           // spawnhealth
	S_NULL,         // seestate
	sfx_None,       // seesound
	0,//MT_FLINGRING,   // reactiontime
	sfx_None,       // attacksound
	S_NULL,         // painstate
	0,              // painchance
	sfx_None,       // painsound
	S_NULL,         // meleestate
	S_NULL,         // missilestate
	S_NULL,//S_SPRK1,        // deathstate
	S_NULL,         // xdeathstate
	sfx_None,     // deathsound
	38,    // speed
	16*FRACUNIT,    // radius
	40*FRACUNIT,    // height
	100,            // mass
	0,              // damage
	sfx_None,       // activesound
	MF_NOGRAVITY|MF_STATIC|MF_NOBLOCKMAP|MF_RINGMOBJ, // flags
	0, // flags2
},
{           // MT_GFZFLOWERB
	801,            // doomednum
	S_GFZFLOWERB1,         // spawnstate
	1000,           // spawnhealth
	S_NULL,         // seestate
	sfx_None,       // seesound
	0,//MT_FLINGRING,   // reactiontime
	sfx_None,       // attacksound
	S_NULL,         // painstate
	0,              // painchance
	sfx_None,       // painsound
	S_NULL,         // meleestate
	S_NULL,         // missilestate
	S_NULL,//S_SPRK1,        // deathstate
	S_NULL,         // xdeathstate
	sfx_None,     // deathsound
	38,    // speed
	16*FRACUNIT,    // radius
	96*FRACUNIT,    // height
	100,            // mass
	0,              // damage
	sfx_None,       // activesound
	MF_NOGRAVITY|MF_STATIC|MF_NOBLOCKMAP|MF_RINGMOBJ, // flags
	0, // flags2
},
{           // MT_GFZFLOWERC
	802,            // doomednum
	S_GFZFLOWERC1,         // spawnstate
	1000,           // spawnhealth
	S_NULL,         // seestate
	sfx_None,       // seesound
	0,//MT_FLINGRING,   // reactiontime
	sfx_None,       // attacksound
	S_NULL,         // painstate
	0,              // painchance
	sfx_None,       // painsound
	S_NULL,         // meleestate
	S_NULL,         // missilestate
	S_NULL,//S_SPRK1,        // deathstate
	S_NULL,         // xdeathstate
	sfx_None,     // deathsound
	38,    // speed
	8*FRACUNIT,    // radius
	32*FRACUNIT,    // height
	100,            // mass
	0,              // damage
	sfx_None,       // activesound
	MF_NOGRAVITY|MF_STATIC|MF_NOBLOCKMAP|MF_RINGMOBJ, // flags
	0, // flags2
},
{           // MT_BERRYBUSH
	804,            // doomednum
	S_BERRYBUSH,         // spawnstate
	1000,           // spawnhealth
	S_NULL,         // seestate
	sfx_None,       // seesound
	0,//MT_FLINGRING,   // reactiontime
	sfx_None,       // attacksound
	S_NULL,         // painstate
	0,              // painchance
	sfx_None,       // painsound
	S_NULL,         // meleestate
	S_NULL,         // missilestate
	S_NULL,//S_SPRK1,        // deathstate
	S_NULL,         // xdeathstate
	sfx_None,     // deathsound
	38,    // speed
	16*FRACUNIT,    // radius
	16*FRACUNIT,    // height
	100,            // mass
	0,              // damage
	sfx_None,       // activesound
	MF_NOGRAVITY|MF_STATIC|MF_NOBLOCKMAP|MF_RINGMOBJ, // flags
	0, // flags2
},
{           // MT_BUSH
	805,            // doomednum
	S_BUSH,         // spawnstate
	1000,           // spawnhealth
	S_NULL,         // seestate
	sfx_None,       // seesound
	0,//MT_FLINGRING,   // reactiontime
	sfx_None,       // attacksound
	S_NULL,         // painstate
	0,              // painchance
	sfx_None,       // painsound
	S_NULL,         // meleestate
	S_NULL,         // missilestate
	S_NULL,//S_SPRK1,        // deathstate
	S_NULL,         // xdeathstate
	sfx_None,     // deathsound
	38,    // speed
	16*FRACUNIT,    // radius
	16*FRACUNIT,    // height
	100,            // mass
	0,              // damage
	sfx_None,       // activesound
	MF_NOGRAVITY|MF_STATIC|MF_NOBLOCKMAP|MF_RINGMOBJ, // flags
	0, // flags2
},
{           // MT_YELLOWSPRING
	550,            // doomednum
	S_YELLOWSPRING, // spawnstate
	1,           // spawnhealth
	S_NULL,         // seestate
	sfx_None,       // seesound
	0,              // reactiontime
	sfx_None,       // attacksound
	S_NULL,         // painstate
	0,              // painchance
	sfx_s3k_b1,     // painsound
	S_NULL,         // meleestate
	S_NULL,         // missilestate
	S_NULL,         // deathstate
	S_NULL,         // xdeathstate
	sfx_None,       // deathsound
	0,              // speed
	20*FRACUNIT,    // radius
	16*FRACUNIT,    // height
	20,    // mass
	0,              // damage
	sfx_None,       // activesound
	MF_SPECIAL|MF_STATIC, // flags
	0, // flags2
},

{           // MT_REDSPRING
	551,            // doomednum
	S_REDSPRING,    // spawnstate
	1,           // spawnhealth
	S_NULL,         // seestate
	sfx_None,       // seesound
	0,              // reactiontime
	sfx_None,       // attacksound
	S_NULL,         // painstate
	0,              // painchance
	sfx_s3k_b1,     // painsound
	S_NULL,         // meleestate
	S_NULL,         // missilestate
	S_NULL,         // deathstate
	S_NULL,         // xdeathstate
	sfx_None,       // deathsound
	0,              // speed
	20*FRACUNIT,    // radius
	16*FRACUNIT,    // height
	32,    // mass
	0,              // damage
	sfx_None,       // activesound
	MF_SPECIAL|MF_STATIC, // flags
	0, // flags2
},
{           // MT_YELLOWDIAG
	555,            // doomednum
	S_YDIAG,       // spawnstate
	1,              // spawnhealth
	S_NULL,         // seestate
	sfx_None,       // seesound
	0,              // reactiontime
	sfx_None,       // attacksound
	S_NULL,         // painstate
	0,              // painchance
	sfx_s3k_b1,     // painsound
	S_NULL,         // meleestate
	S_NULL,         // missilestate
	S_NULL,         // deathstate
	S_NULL,         // xdeathstate
	sfx_None,       // deathsound
	0,              // speed
	16*FRACUNIT,    // radius
	16*FRACUNIT,    // height
	20,    // mass
	20,    // damage
	sfx_None,       // activesound
	MF_SPECIAL|MF_STATIC, // flags
	0, // flags2
},

{           // MT_REDDIAG
	556,            // doomednum
	S_RDIAG,       // spawnstate
	1,              // spawnhealth
	S_NULL,         // seestate
	sfx_None,       // seesound
	0,              // reactiontime
	sfx_None,       // attacksound
	S_NULL,         // painstate
	0,              // painchance
	sfx_s3k_b1,     // painsound
	S_NULL,         // meleestate
	S_NULL,         // missilestate
	S_NULL,         // deathstate
	S_NULL,         // xdeathstate
	sfx_None,       // deathsound
	0,              // speed
	16*FRACUNIT,    // radius
	16*FRACUNIT,    // height
	32,    // mass
	32,    // damage
	sfx_None,       // activesound
	MF_SPECIAL|MF_STATIC, // flags
	0, // flags2
},
{           // MT_YELLOWHORIZ
	558,            // doomednum
	S_YHORIZ,      // spawnstate
	1,              // spawnhealth
	S_NULL,         // seestate
	sfx_None,       // seesound
	0,              // reactiontime
	sfx_None,       // attacksound
	S_NULL,         // painstate
	0,              // painchance
	sfx_s3k_b1,     // painsound
	S_NULL,         // meleestate
	S_NULL,         // missilestate
	S_NULL,         // deathstate
	S_NULL,         // xdeathstate
	sfx_None,       // deathsound
	0,              // speed
	16*FRACUNIT,    // radius
	32*FRACUNIT,    // height
	0,              // mass
	40,    // damage
	sfx_None,       // activesound
	MF_SPECIAL|MF_STATIC, // flags
	0, // flags2
},

{           // MT_REDHORIZ
	559,            // doomednum
	S_RHORIZ,      // spawnstate
	1,              // spawnhealth
	S_NULL,         // seestate
	sfx_None,       // seesound
	0,              // reactiontime
	sfx_None,       // attacksound
	S_NULL,         // painstate
	0,              // painchance
	sfx_s3k_b1,     // painsound
	S_NULL,         // meleestate
	S_NULL,         // missilestate
	S_NULL,         // deathstate
	S_NULL,         // xdeathstate
	sfx_None,       // deathsound
	0,              // speed
	16*FRACUNIT,    // radius
	32*FRACUNIT,    // height
	0,              // mass
	80,    // damage
	sfx_None,       // activesound
	MF_SPECIAL|MF_STATIC, // flags
	0, // flags2
},
	{           // MT_GHOST
		-1,             // doomednum
		S_SPINDUST1,         // spawnstate
		1,              // spawnhealth
		S_NULL,         // seestate
		sfx_None,       // seesound
		0,              // reactiontime
		sfx_None,       // attacksound
		S_NULL,         // painstate
		0,              // painchance
		sfx_None,       // painsound
		S_NULL,         // meleestate
		S_NULL,         // missilestate
		S_NULL,         // deathstate
		S_NULL,         // xdeathstate
		sfx_None,       // deathsound
		0,              // speed
		16*FRACUNIT,    // radius
		48*FRACUNIT,    // height
		1000,           // mass
		8,              // damage
		sfx_None,       // activesound
		MF_NOBLOCKMAP|MF_NOGRAVITY|MF_NOCLIP, // flags
		0, // flags2
	},

	{           // MT_ELEMENTAL_ORB
		-1,             // doomednum
		S_ELEM1,        // spawnstate
		1000,           // spawnhealth
		S_NULL,         // seestate
		sfx_None,       // seesound
		0,              // reactiontime
		sfx_None,       // attacksound
		S_NULL,         // painstate
		SH_ELEMENTAL,   // painchance
		sfx_None,       // painsound
		S_NULL,         // meleestate
		S_NULL,         // missilestate
		S_NULL,         // deathstate
		S_NULL,         // xdeathstate
		sfx_None,       // deathsound
		0,              // speed
		4*FRACUNIT,     // radius
		4*FRACUNIT,     // height
		16,             // mass
		0,              // damage
		sfx_None,       // activesound
		MF_NOBLOCKMAP|MF_NOCLIP|MF_NOGRAVITY, // flags
		MF2_NARROWGFX, // flags2
	},

	{           // MT_ATTRACT_ORB
		-1,             // doomednum
		S_MAGN1,        // spawnstate
		1000,           // spawnhealth
		S_NULL,         // seestate
		sfx_None,       // seesound
		0,              // reactiontime
		sfx_None,       // attacksound
		S_NULL,         // painstate
		SH_ATTRACT,   // painchance
		sfx_None,       // painsound
		S_NULL,         // meleestate
		S_NULL,         // missilestate
		S_NULL,         // deathstate
		S_NULL,         // xdeathstate
		sfx_None,       // deathsound
		0,              // speed
		4*FRACUNIT,     // radius
		4*FRACUNIT,     // height
		16,             // mass
		0,              // damage
		sfx_None,       // activesound
		MF_NOBLOCKMAP|MF_NOCLIP|MF_NOGRAVITY, // flags
		MF2_NARROWGFX, // flags2
	},

	{           // MT_FORCE_ORB
		-1,             // doomednum
		S_FORCA1,       // spawnstate
		1000,           // spawnhealth
		S_NULL,         // seestate
		sfx_None,       // seesound
		0,              // reactiontime
		sfx_None,       // attacksound
		S_FORCB1,       // painstate
		SH_FORCE2,      // painchance
		sfx_None,       // painsound
		S_NULL,         // meleestate
		S_NULL,         // missilestate
		S_NULL,         // deathstate
		S_NULL,         // xdeathstate
		sfx_None,       // deathsound
		0,              // speed
		4*FRACUNIT,     // radius
		4*FRACUNIT,     // height
		16,             // mass
		SH_FORCE1,      // damage
		sfx_None,       // activesound
		MF_NOBLOCKMAP|MF_NOCLIP|MF_NOGRAVITY, // flags
		MF2_NARROWGFX, // flags2
	},

	{           // MT_ARMAGEDDON_ORB
		-1,             // doomednum
		S_ARMA1,        // spawnstate
		1000,           // spawnhealth
		S_NULL,         // seestate
		sfx_None,       // seesound
		0,              // reactiontime
		sfx_None,       // attacksound
		S_NULL,         // painstate
		SH_ARMAGEDDON,  // painchance
		sfx_None,       // painsound
		S_NULL,         // meleestate
		S_NULL,         // missilestate
		S_NULL,         // deathstate
		S_NULL,         // xdeathstate
		sfx_None,       // deathsound
		0,              // speed
		4*FRACUNIT,     // radius
		4*FRACUNIT,     // height
		16,             // mass
		0,              // damage
		sfx_None,       // activesound
		MF_NOBLOCKMAP|MF_NOCLIP|MF_NOGRAVITY, // flags
		MF2_NARROWGFX, // flags2
	},

	{           // MT_WHIRLWIND_ORB
		-1,             // doomednum
		S_WIND1,        // spawnstate
		1000,           // spawnhealth
		S_NULL,         // seestate
		sfx_None,       // seesound
		0,              // reactiontime
		sfx_None,       // attacksound
		S_NULL,         // painstate
		SH_WHIRLWIND,   // painchance
		sfx_None,       // painsound
		S_NULL,         // meleestate
		S_NULL,         // missilestate
		S_NULL,         // deathstate
		S_NULL,         // xdeathstate
		sfx_None,       // deathsound
		0,              // speed
		4*FRACUNIT,     // radius
		4*FRACUNIT,     // height
		16,             // mass
		0,              // damage
		sfx_None,       // activesound
		MF_NOBLOCKMAP|MF_NOCLIP|MF_NOGRAVITY, // flags
		MF2_NARROWGFX, // flags2
	},

	{           // MT_IVSP
		-1,             // doomednum
		S_IVSP1,         // spawnstate
		1000,           // spawnhealth
		S_NULL,         // seestate
		sfx_None,       // seesound
		8,              // reactiontime
		sfx_None,       // attacksound
		S_NULL,         // painstate
		0,              // painchance
		sfx_None,       // painsound
		S_NULL,         // meleestate
		S_NULL,         // missilestate
		S_NULL,         // deathstate
		S_NULL,         // xdeathstate
		sfx_None,       // deathsound
		8,              // speed
		4*FRACUNIT,     // radius
		8*FRACUNIT,     // height
		16,             // mass
		0,              // damage
		sfx_None,       // activesound
		MF_NOBLOCKMAP|MF_NOCLIP|MF_NOGRAVITY|MF_STATIC, // flags
		MF2_FORWARDOFFSET,              // flags2
	},

	// Bluebird
	{           // MT_FLICKY_01
		-1,             // doomednum
		S_FLICKY_01_OUT, // spawnstate
		1000,           // spawnhealth
		S_FLICKY_01_FLAP1, // seestate
		sfx_None,       // seesound
		8,              // reactiontime
		sfx_None,       // attacksound
		S_NULL,         // painstate
		0,              // painchance
		sfx_None,       // painsound
		S_NULL,         // meleestate
		S_NULL,         // missilestate
		S_NULL,         // deathstate
		S_NULL,         // xdeathstate
		sfx_None,       // deathsound
		2*FRACUNIT,     // speed
		8*FRACUNIT,     // radius
		20*FRACUNIT,    // height
		4,              // mass
		0,              // damage
		sfx_None,       // activesound
		0, // flags
		MF2_FLOAT|MF2_NARROWGFX, // flags2
	},

	{           // MT_FLICKY_02
		-1,             // doomednum
		S_FLICKY_02_OUT, // spawnstate
		1000,           // spawnhealth
		S_FLICKY_02_DOWN, // seestate
		sfx_None,       // seesound
		8,              // reactiontime
		sfx_None,       // attacksound
		S_NULL,         // painstate
		1,              // painchance
		sfx_None,       // painsound
		S_FLICKY_02_UP, // meleestate
		S_NULL,         // missilestate
		S_NULL,         // deathstate
		S_NULL,         // xdeathstate
		sfx_None,       // deathsound
		2*FRACUNIT,     // speed
		8*FRACUNIT,     // radius
		20*FRACUNIT,    // height
		4,             // mass
		0,              // damage
		sfx_None,       // activesound
		0, // flags
		MF2_FLOAT|MF2_NARROWGFX, // flags2
	},

	{           // MT_FLICKY_03
		-1,             // doomednum
		S_FLICKY_03_OUT, // spawnstate
		1000,           // spawnhealth
		S_FLICKY_03_FLAP1, // seestate
		sfx_None,       // seesound
		8,              // reactiontime
		sfx_None,       // attacksound
		S_NULL,         // painstate
		0,              // painchance
		sfx_None,       // painsound
		S_NULL,         // meleestate
		S_NULL,         // missilestate
		S_NULL,         // deathstate
		S_NULL,         // xdeathstate
		sfx_None,       // deathsound
		1*FRACUNIT + (FRACUNIT/2),     // speed
		8*FRACUNIT,     // radius
		20*FRACUNIT,    // height
		3,              // mass
		0,              // damage
		sfx_None,       // activesound
		0, // flags
		MF2_FLOAT|MF2_NARROWGFX, // flags2
	},

	{           // MT_FLICKY_12
		-1,             // doomednum
		S_FLICKY_12_OUT, // spawnstate
		1000,           // spawnhealth
		S_FLICKY_12_RUN1, // seestate
		sfx_None,       // seesound
		8,              // reactiontime
		sfx_None,       // attacksound
		S_NULL,         // painstate
		0,              // painchance
		sfx_None,       // painsound
		S_NULL,         // meleestate
		S_NULL,         // missilestate
		S_NULL,         // deathstate
		S_NULL,         // xdeathstate
		sfx_None,       // deathsound
		3*FRACUNIT,     // speed
		8*FRACUNIT,     // radius
		20*FRACUNIT,    // height
		2,              // mass
		0,              // damage
		sfx_None,       // activesound
		0, // flags
		MF2_FLOAT|MF2_NARROWGFX, // flags2
	},

	{           // MT_GFZDEBRIS
		-1,             // doomednum
		S_GFZDEBRIS1,    // spawnstate
		1,              // spawnhealth
		S_NULL,         // seestate
		sfx_None,       // seesound
		0,              // reactiontime
		sfx_None,       // attacksound
		S_NULL,         // painstate
		0,              // painchance
		sfx_None,       // painsound
		S_NULL,         // meleestate
		S_NULL,         // missilestate
		S_NULL,         // deathstate
		S_NULL,         // xdeathstate
		sfx_None,       // deathsound
		0,              // speed
		32*FRACUNIT,    // radius
		64*FRACUNIT,    // height
		100,            // mass
		0,              // damage
		sfx_None,       // activesound
		MF_NOBLOCKMAP,  // flags
		MF2_FLOAT|MF2_NARROWGFX, // flags2
	},

	{           // MT_BUBBLES
		500,            // doomednum
		S_BUBBLES1,     // spawnstate
		1000,           // spawnhealth
		S_NULL,         // seestate
		sfx_None,       // seesound
		8,              // reactiontime
		sfx_None,       // attacksound
		S_NULL,         // painstate
		0,              // painchance
		sfx_None,       // painsound
		S_NULL,         // meleestate
		S_NULL,         // missilestate
		S_NULL,         // deathstate
		S_NULL,         // xdeathstate
		sfx_None,       // deathsound
		0,              // speed
		8*FRACUNIT,     // radius
		8*FRACUNIT,     // height
		100,            // mass
		0,              // damage
		sfx_None,       // activesound
		MF_NOBLOCKMAP|MF_NOCLIP, // flags
		0          // flags2
	},

	{           // MT_SMALLBUBBLE
		-1,             // doomednum
		S_SMALLBUBBLE,  // spawnstate
		1000,           // spawnhealth
		S_NULL,         // seestate
		sfx_None,       // seesound
		8,              // reactiontime
		sfx_None,       // attacksound
		S_NULL,         // painstate
		0,              // painchance
		sfx_None,       // painsound
		S_NULL,         // meleestate
		S_NULL,         // missilestate
		S_NULL,         // deathstate
		S_NULL,         // xdeathstate
		sfx_None,       // deathsound
		8,              // speed
		4*FRACUNIT,     // radius
		4*FRACUNIT,     // height
		16,             // mass
		0,              // damage
		sfx_None,       // activesound
		MF_NOBLOCKMAP|MF_NOCLIP|MF_NOGRAVITY, // flags
		MF2_FLOAT, // flags2
	},

	{           // MT_MEDIUMBUBBLE
		-1,             // doomednum
		S_MEDIUMBUBBLE, // spawnstate
		1000,           // spawnhealth
		S_NULL,         // seestate
		sfx_None,       // seesound
		8,              // reactiontime
		sfx_None,       // attacksound
		S_NULL,         // painstate
		0,              // painchance
		sfx_None,       // painsound
		S_NULL,         // meleestate
		S_NULL,         // missilestate
		S_NULL,         // deathstate
		S_NULL,         // xdeathstate
		sfx_None,       // deathsound
		8,              // speed
		8*FRACUNIT,     // radius
		8*FRACUNIT,     // height
		16,             // mass
		0,              // damage
		sfx_None,       // activesound
		MF_NOBLOCKMAP|MF_NOCLIP|MF_NOGRAVITY, // flags
		MF2_FLOAT, // flags2
	},

	{           // MT_EXTRALARGEBUBBLE
		-1,             // doomednum
		S_LARGEBUBBLE1, // spawnstate
		1000,           // spawnhealth
		S_NULL,         // seestate
		sfx_None,       // seesound
		8,              // reactiontime
		sfx_None,       // attacksound
		S_NULL,         // painstate
		0,              // painchance
		sfx_None,       // painsound
		S_NULL,         // meleestate
		S_NULL,         // missilestate
		S_POP1,         // deathstate
		S_NULL,         // xdeathstate
		sfx_s3k_38,     // deathsound
		8,              // speed
		23*FRACUNIT,    // radius
		43*FRACUNIT,    // height
		16,             // mass
		0,              // damage
		sfx_None,       // activesound
		MF_SPECIAL|MF_NOGRAVITY, // flags
		MF2_FLOAT, // flags2
	},

	{           // MT_SPLISH
		-1,             // doomednum
		S_SPLISH1,      // spawnstate
		1000,           // spawnhealth
		S_NULL,         // seestate
		sfx_None,       // seesound
		8,              // reactiontime
		sfx_None,       // attacksound
		S_NULL,         // painstate
		0,              // painchance
		sfx_None,       // painsound
		S_NULL,         // meleestate
		S_NULL,         // missilestate
		S_NULL,         // deathstate
		S_NULL,         // xdeathstate
		sfx_None,       // deathsound
		0,              // speed
		6*FRACUNIT,     // radius
		1*FRACUNIT,     // height
		100,            // mass
		1,              // damage
		sfx_None,       // activesound
		MF_NOBLOCKMAP|MF_NOCLIP|MF_NOGRAVITY|MF_STATIC, // flags
		0, // flags2
	},

	{           // MT_SONIC3KBOSSEXPLODE
		-1,                      // doomednum
		S_SONIC3KBOSSEXPLOSION1, // spawnstate
		1000,                    // spawnhealth
		S_NULL,                  // seestate
		sfx_None,                // seesound
		8,                       // reactiontime
		sfx_None,                // attacksound
		S_NULL,                  // painstate
		0,                       // painchance
		sfx_None,                // painsound
		S_NULL,                  // meleestate
		S_NULL,                  // missilestate
		S_NULL,                  // deathstate
		S_NULL,                  // xdeathstate
		sfx_None,                // deathsound
		1,                       // speed
		8*FRACUNIT,              // radius
		16*FRACUNIT,             // height
		4,                       // mass
		0,                       // damage
		sfx_None,                // activesound
		MF_NOBLOCKMAP|MF_NOCLIP|MF_NOGRAVITY|MF_STATIC, // flags
		MF2_NARROWGFX, // flags2
	},

	{           // MT_BOSSFLYPOINT
		290,            // doomednum
		S_INVISIBLE,    // spawnstate
		1000,           // spawnhealth
		S_NULL,         // seestate
		sfx_None,       // seesound
		8,              // reactiontime
		sfx_None,       // attacksound
		S_NULL,         // painstate
		0,              // painchance
		sfx_None,       // painsound
		S_NULL,         // meleestate
		S_NULL,         // missilestate
		S_NULL,         // deathstate
		S_NULL,         // xdeathstate
		sfx_None,       // deathsound
		1,              // speed
		2*FRACUNIT,     // radius
		4*FRACUNIT,     // height
		4,              // mass
		0,              // damage
		sfx_None,       // activesound
		MF_NOBLOCKMAP|MF_NOSECTOR|MF_NOGRAVITY, // flags
		0, // flags2
	},

	{           // MT_EGGTRAP
		291,            // doomednum
		S_INVISIBLE,    // spawnstate
		1000,           // spawnhealth
		S_NULL,         // seestate
		sfx_None,       // seesound
		8,              // reactiontime
		sfx_None,       // attacksound
		S_NULL,         // painstate
		0,              // painchance
		sfx_s3k_3d,        // painsound
		S_NULL,         // meleestate
		S_NULL,         // missilestate
		S_INVISIBLE,    // deathstate
		S_NULL,         // xdeathstate
		sfx_s3k_3d,        // deathsound
		1,              // speed
		8*FRACUNIT,     // radius
		16*FRACUNIT,    // height
		4,              // mass
		0,              // damage
		sfx_None,       // activesound
		MF_NOBLOCKMAP|MF_NOSECTOR, // flags
		0, // flags2
	},

	{           // MT_BOSSJUNK
		-1,             // doomednum
		S_BOSSEGLZ1,    // spawnstate
		1,              // spawnhealth
		S_NULL,         // seestate
		sfx_None,       // seesound
		8,              // reactiontime
		sfx_None,       // attacksound
		S_NULL,         // painstate
		0,              // painchance
		sfx_None,       // painsound
		S_NULL,         // meleestate
		S_NULL,         // missilestate
		S_NULL,         // deathstate
		S_NULL,         // xdeathstate
		sfx_None,       // deathsound
		0,              // speed
		8*FRACUNIT,     // radius
		64*FRACUNIT,    // height
		100,            // mass
		1,              // damage
		sfx_None,       // activesound
		MF_NOBLOCKMAP|MF_NOCLIP, // flags
		0, // flags2
	},

	{           // MT_JETFUME1
		-1,             // doomednum
		S_JETFUME1,     // spawnstate
		1000,           // spawnhealth
		S_NULL,         // seestate
		sfx_None,       // seesound
		8,              // reactiontime
		sfx_None,       // attacksound
		S_NULL,         // painstate
		0,              // painchance
		sfx_None,       // painsound
		S_NULL,         // meleestate
		S_NULL,         // missilestate
		S_NULL,         // deathstate
		S_NULL,         // xdeathstate
		sfx_None,       // deathsound
		1,              // speed
		8*FRACUNIT,     // radius
		16*FRACUNIT,    // height
		4,              // mass
		0,              // damage
		sfx_None,       // activesound
		MF_NOBLOCKMAP|MF_NOGRAVITY|MF_NOCLIP, // flags
		MF2_NARROWGFX, // flags2
	},

	{           // MT_EGGMOBILE_TARGET
		-1,             // doomednum
		S_EGGMOBILE_TARGET, // spawnstate
		1,              // spawnhealth
		S_NULL,         // seestate
		sfx_None,       // seesound
		0,              // reactiontime
		sfx_None,       // attacksound
		S_NULL,         // painstate
		0,              // painchance
		sfx_None,       // painsound
		0,              // meleestate
		S_NULL,         // missilestate
		S_NULL,         // deathstate
		S_NULL,         // xdeathstate
		sfx_None,       // deathsound
		2*FRACUNIT,     // speed
		0,              // radius
		0,              // height
		0,              // mass
		0,              // damage
		sfx_None,       // activesound
		MF_NOCLIP|MF_NOGRAVITY|MF_NOBLOCKMAP|MF_NOSECTOR, // flags
		0, // flags2
	},

	{           // MT_EGGMOBILE_FIRE
		-1,             // doomednum
		S_LASERFLAME1,  // spawnstate
		1,              // spawnhealth
		S_NULL,         // seestate
		sfx_None,       // seesound
		8,              // reactiontime
		sfx_None,       // attacksound
		S_NULL,         // painstate
		0,              // painchance
		sfx_None,       // painsound
		S_NULL,         // meleestate
		S_NULL,         // missilestate
		S_NULL,         // deathstate
		S_NULL,         // xdeathstate
		sfx_None,       // deathsound
		0,              // speed
		24*FRACUNIT,    // radius
		84*FRACUNIT,    // height
		0,       // mass
		1,              // damage
		sfx_None,       // activesound
		MF_NOGRAVITY, // flags
		MF2_MISSILE|MF2_FLOAT|MF2_NARROWGFX, // flags2
	},

	{           // MT_LASER
		-1,             // doomednum
		S_LASER,        // spawnstate
		1000,           // spawnhealth
		S_NULL,         // seestate
		sfx_rlaunc,     // seesound
		8,              // reactiontime
		sfx_None,       // attacksound
		S_NULL,         // painstate
		0,              // painchance
		sfx_None,       // painsound
		S_NULL,         // meleestate
		S_LASER2,       // missilestate
		S_NULL,         // deathstate
		S_NULL,         // xdeathstate
		sfx_None,       // deathsound
		20*FRACUNIT,    // speed
		11*FRACUNIT,    // radius
		8*FRACUNIT,     // height
		0,              // mass
		20,             // damage
		sfx_None,       // activesound
		MF_NOGRAVITY, // flags
		MF2_MISSILE|MF2_FLOAT|MF2_NARROWGFX, // flags2
	},

	{           // MT_DUST
		-1,             // doomednum
		S_DUST1,     // spawnstate
		1000,           // spawnhealth
		S_NULL,         // seestate
		sfx_None,       // seesound
		8,              // reactiontime
		sfx_None,       // attacksound
		S_NULL,         // painstate
		0,              // painchance
		sfx_None,       // painsound
		S_NULL,         // meleestate
		S_NULL,         // missilestate
		S_NULL,         // deathstate
		S_NULL,         // xdeathstate
		sfx_None,       // deathsound
		3*FRACUNIT,     // speed
		FRACUNIT,       // radius
		FRACUNIT,       // height
		4,              // mass
		0,              // damage
		sfx_None,       // activesound
		MF_NOBLOCKMAP|MF_NOGRAVITY|MF_NOCLIP|MF_STATIC, // flags
		MF2_NARROWGFX, // flags2
	},

};

