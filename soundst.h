/* newsfx.h */

typedef struct
{
    int     samples;
    int     loop_start;
    int     loop_end;
    int     info;
    int     unity;
	int		pitch_correction;
	int		decay_step;
    unsigned char data[1];
} sfx_t;

typedef struct sfxinfo_s
{
	char singularity;	/* Sfx singularity (only one at a time) */
	unsigned char priority;		/* Sfx priority */
#ifdef MARS
	short lump;
#else
	int	pitch;		/* pitch if a link */
	int	volume;		/* volume if a link */
	struct sfxinfo_s* link;	/* referenced sound if a link */
	sfx_t* md_data;	/* machine-dependent sound data */
#endif
} sfxinfo_t;

typedef struct
{
#ifdef MARS
	short lump;
#else
  void *md_data;	/* machine-dependent music data */
#endif
} musicinfo_t;

/*============================================================================ */

#define	INTERNALQUADS	256			/* 4k / 16 bytes per quad (64 bits) */
#define	EXTERNALQUADS	512			/* 16k  / 32 bytes per quad (16 bits+music) */
#define	SFXCHANNELS		8

typedef void (*getsoundpos_t)(mobj_t *, fixed_t *);

typedef struct
{
#ifdef MARS
	uint8_t		*data;
	int			position;
	int			increment;
	int			length;
	int			loop_length;
	int			prev_pos;			/* for adpcm decoding */
	uint8_t		volume;
	uint8_t		pan;
#else
	unsigned* source;				/* work in groups of 4 8 bit samples */
	int			startquad;
	int			stopquad;
	int			volume;				/* range from 0-32k */
#endif
	uint16_t	width;
	uint16_t	block_size; 		/* size of data block in bytes */
	int			remaining_bytes; 	/* WAV chunk */

	sfxinfo_t	*sfx;
	mobj_t		*mobj;
	getsoundpos_t getpos;
} sfxchannel_t;

enum
{
	mustype_none,
	mustype_fm,
	mustype_cd,
};

#ifdef MARS
extern	sfxchannel_t	*sfxchannels/*[SFXCHANNELS]*/;
#else
extern	sfxchannel_t	sfxchannels[SFXCHANNELS];
#endif

extern	int		finalquad;			/* the last quad mixed by update. */
									
extern	VINT	sfxvolume;			/* range 0 - 255 */
extern	VINT 	musicvolume;		/* range 0 - 255 */
extern	VINT	oldsfxvolume;		/* to detect transition to sound off */

extern	int		soundtics;			/* time spent mixing sounds */
extern	int		soundstarttics;		/* time S_Update started */

extern	int		sfxsample;			/* the sample about to be output */
									/* by S_WriteOutSamples */
									
/* external buffer for sfx and music */
extern	int		soundbuffer[EXTERNALQUADS*16]; 

extern	int		samplecount;		/* 22khz sample counter in DSP memory */

extern	VINT	musictype;

/*============================================================================ */

void S_Init(void);
void S_Clear (void);
void S_StartSound(mobj_t *mobj, int sound_id);
void S_StartPositionedSound(mobj_t* mobj, int sound_id, getsoundpos_t getpos);
void S_UpdateSounds(void);
int S_CDAvailable(void);
void S_SetMusicType(int t);

