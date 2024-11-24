/* P_main.c */

#include "doomdef.h"
#include "p_local.h"
#include "mars.h"
#include "p_camera.h"
#include "st_inter.h"

#define DEFAULT_GAME_ZONE_MARGIN (4*1024)

int			numvertexes;
vertex_t	*vertexes;

int			numsegs;
seg_t		*segs;

int			numsectors;
sector_t	*sectors;

int			numsubsectors;
subsector_t	*subsectors;

int			numnodes;
node_t		*nodes;

int			numlines;
line_t		*lines;

int			numsides;
side_t		*sides;

short		*blockmaplump;			/* offsets in blockmap are from here */
int			bmapwidth, bmapheight;	/* in mapblocks */
fixed_t		bmaporgx, bmaporgy;		/* origin of block map */
mobj_t		**blocklinks;			/* for thing chains */

byte		*rejectmatrix;			/* for fast sight rejection */

mapthing_t	*deathmatchstarts, *deathmatch_p;

VINT		*validcount;			/* increment every time a check is made */

mapthing_t	playerstarts[MAXPLAYERS];

int			numthings;
spawnthing_t* spawnthings;

/*
=================
=
= P_LoadVertexes
=
=================
*/

void P_LoadVertexes (int lump)
{
#ifdef MARS
	numvertexes = W_LumpLength (lump) / sizeof(vertex_t);
	vertexes = (vertex_t *)W_POINTLUMPNUM(lump);
#else
	byte		*data;
	int			i;
	mapvertex_t	*ml;
	vertex_t	*li;
	
	numvertexes = W_LumpLength (lump) / sizeof(mapvertex_t);
	vertexes = Z_Malloc (numvertexes*sizeof(vertex_t),PU_LEVEL);
	data = I_TempBuffer ();	
	W_ReadLump (lump,data);
	
	
	ml = (mapvertex_t *)data;
	li = vertexes;
	for (i=0 ; i<numvertexes ; i++, li++, ml++)
	{
		li->x = LITTLESHORT(ml->x)<<FRACBITS;
		li->y = LITTLESHORT(ml->y)<<FRACBITS;
	}
#endif
}


/*
=================
=
= P_LoadSegs
=
=================
*/

void P_LoadSegs (int lump)
{
#ifdef MARS
	numsegs = W_LumpLength (lump) / sizeof(seg_t);
	segs = (seg_t *)W_POINTLUMPNUM(lump);
#else
	byte		*data;
	int			i;
	mapseg_t	*ml;
	seg_t		*li;
	int			linedef, side;

	numsegs = W_LumpLength (lump) / sizeof(mapseg_t);
	segs = Z_Malloc (numsegs*sizeof(seg_t)+16,PU_LEVEL);
	segs = (void*)(((uintptr_t)segs + 15) & ~15); // aline on cacheline boundary
	D_memset (segs, 0, numsegs*sizeof(seg_t));
	data = I_TempBuffer ();
	W_ReadLump (lump,data);
	
	ml = (mapseg_t *)data;
	li = segs;
	for (i=0 ; i<numsegs ; i++, li++, ml++)
	{
		li->v1 = LITTLESHORT(ml->v1);
		li->v2 = LITTLESHORT(ml->v2);

		li->sideoffset = LITTLESHORT(ml->offset);
		li->linedef = LITTLESHORT(ml->linedef);

		side = LITTLESHORT(ml->side);
		side &= 1;

		li->sideoffset <<= 1;
		li->sideoffset |= side;
	}
#endif
}


/*
=================
=
= P_LoadSubsectors
=
=================
*/

void P_LoadSubsectors (int lump)
{
	byte			*data;
	int				i;
	mapsubsector_t	*ms;
	subsector_t		*ss;

	numsubsectors = W_LumpLength (lump) / sizeof(mapsubsector_t);
	subsectors = Z_Malloc (numsubsectors*sizeof(subsector_t),PU_LEVEL);
	data = I_TempBuffer ();
	W_ReadLump (lump,data);

	ms = (mapsubsector_t *)data;
	D_memset (subsectors,0, numsubsectors*sizeof(subsector_t));
	ss = subsectors;
	for (i=0 ; i<numsubsectors ; i++, ss++, ms++)
	{
		ss->numlines = LITTLESHORT(ms->numsegs);
		ss->firstline = LITTLESHORT(ms->firstseg);
	}
}


/*
=================
=
= P_LoadSectors
=
=================
*/

void P_LoadSectors (int lump)
{
	byte			*data;
	int				i;
	mapsector_t		*ms;
	sector_t		*ss;
			
	numsectors = W_LumpLength (lump) / sizeof(mapsector_t);
	sectors = Z_Malloc (numsectors*sizeof(sector_t) + 16,PU_LEVEL);
	sectors = (void*)(((uintptr_t)sectors + 15) & ~15); // aline on cacheline boundary
	D_memset (sectors, 0, numsectors*sizeof(sector_t));
	data = I_TempBuffer ();
	W_ReadLump (lump,data);
	
	ms = (mapsector_t *)data;
	ss = sectors;
	for (i=0 ; i<numsectors ; i++, ss++, ms++)
	{
		int lightlevel, special;

		ss->floorheight = LITTLESHORT(ms->floorheight)<<FRACBITS;
		ss->ceilingheight = LITTLESHORT(ms->ceilingheight)<<FRACBITS;
		ss->floorpic = R_FlatNumForName(ms->floorpic);
		if (!D_strncasecmp (ms->ceilingpic,"F_SKY1",6) )
			ss->ceilingpic = -1;
		else
		{
			ss->ceilingpic = R_FlatNumForName(ms->ceilingpic);
		}
		ss->thinglist = NULL;

		lightlevel = LITTLESHORT(ms->lightlevel);
		if (lightlevel < 0)
			lightlevel = 0;
		else if (lightlevel > 255)
			lightlevel = 255;
		ss->lightlevel = lightlevel;

		special = LITTLESHORT(ms->special);
		if (special < 0 || special > 255)
			special = 0;
		ss->special = special;

		ss->tag = (uint8_t)LITTLESHORT(ms->tag);
		ss->heightsec = -1; // sector used to get floor and ceiling height
		ss->fofsec = -1;
		ss->floor_xoffs = 0;
		ss->flags = 0;

		// killough 3/7/98:
//		ss->floor_xoffs = 0;
//		ss->floor_yoffs = 0;      // floor and ceiling flats offsets
	}
}


/*
=================
=
= P_LoadNodes
=
=================
*/

void P_LoadNodes (int lump)
{
#ifdef MARS	
	numnodes = W_LumpLength (lump) / sizeof(node_t);
	nodes = (node_t *)W_POINTLUMPNUM(lump);
#else
	byte		*data;
	int			i,j,k;
	mapnode_t	*mn;
	node_t		*no;
	
	numnodes = W_LumpLength (lump) / sizeof(mapnode_t);
	nodes = Z_Malloc (numnodes*sizeof(node_t),PU_LEVEL);
	data = I_TempBuffer ();
	W_ReadLump (lump,data);
	
	mn = (mapnode_t *)data;
	no = nodes;
	for (i=0 ; i<numnodes ; i++, no++, mn++)
	{
		no->x = LITTLESHORT(mn->x)<<FRACBITS;
		no->y = LITTLESHORT(mn->y)<<FRACBITS;
		no->dx = LITTLESHORT(mn->dx)<<FRACBITS;
		no->dy = LITTLESHORT(mn->dy)<<FRACBITS;
		for (j=0 ; j<2 ; j++)
		{
			no->children[j] = (unsigned short)LITTLESHORT(mn->children[j]);
			for (k=0 ; k<4 ; k++)
				no->bbox[j][k] = LITTLESHORT(mn->bbox[j][k])<<FRACBITS;
		}
	}
#endif
}



/*
=================
=
= P_LoadThings
=
=================
*/

void P_LoadThings (int lump)
{
	byte			*data;
	int				i;
	mapthing_t		*mt;
	spawnthing_t	*st;
	short			numthingsreal, numstaticthings, numringthings;

	data = I_TempBuffer ();
	W_ReadLump (lump,data);
	numthings = W_LumpLength (lump) / sizeof(mapthing_t);
	numthingsreal = 0;
	numstaticthings = 0;
	numringthings = 0;
	numscenerymobjs = 0;

	mt = (mapthing_t *)data;
	for (i=0 ; i<numthings ; i++, mt++)
	{
		mt->x = LITTLESHORT(mt->x);
		mt->y = LITTLESHORT(mt->y);
		mt->angle = LITTLESHORT(mt->angle);
		mt->type = LITTLESHORT(mt->type);
		mt->options = LITTLESHORT(mt->options);
	}

	if (netgame == gt_deathmatch)
	{
		spawnthings = Z_Malloc(numthings * sizeof(*spawnthings), PU_LEVEL);
		st = spawnthings;
		mt = (mapthing_t*)data;
		for (i = 0; i < numthings; i++, mt++)
		{
			st->x = mt->x, st->y = mt->y;
			st->angle = mt->angle;
			st->type = mt->type;
			st++;
		}
	}

	mt = (mapthing_t *)data;
	for (i=0 ; i<numthings ; i++, mt++)
	{
		switch (P_MapThingSpawnsMobj(mt))
		{
			case 0:
				break;
			case 1:
				numthingsreal++;
				break;
			case 2:
				numstaticthings++;
				break;
			case 3:
				numringthings++;
				break;
			case 4:
				numscenerymobjs++;
				break;
		}
	}

	// preallocate a few mobjs for puffs and projectiles
	numthingsreal += 48; // 32 rings, plus other items
	numstaticthings += 8;

	if (gamemapinfo.act == 3)
	{
		// Bosses spawn lots of stuff, so preallocate more.
		numthingsreal += 96;
		numstaticthings += 64;
	}
	P_PreSpawnMobjs(numthingsreal, numstaticthings, numringthings, numscenerymobjs);

	mt = (mapthing_t *)data;
	for (i=0 ; i<numthings ; i++, mt++)
		P_SpawnMapThing (mt, i);

	camBossMobj = P_FindFirstMobjOfType(MT_EGGMOBILE);
	if (!camBossMobj)
		camBossMobj = P_FindFirstMobjOfType(MT_EGGMOBILE2);

	if (players[0].starpostnum)
		P_SetStarPosts(players[0].starpostnum + 1);
}



/*
=================
=
= P_LoadLineDefs
=
= Also counts secret lines for intermissions
=================
*/

void P_LoadLineDefs (int lump)
{
	byte			*data;
	int				i;
	maplinedef_t	*mld;
	line_t			*ld;
	vertex_t		*v1, *v2;
	
	numlines = W_LumpLength (lump) / sizeof(maplinedef_t);
	lines = Z_Malloc (numlines*sizeof(line_t)+16,PU_LEVEL);
	lines = (void*)(((uintptr_t)lines + 15) & ~15); // aline on cacheline boundary
	D_memset (lines, 0, numlines*sizeof(line_t));
	data = I_TempBuffer ();
	W_ReadLump (lump,data);

	mld = (maplinedef_t *)data;
	ld = lines;
	for (i=0 ; i<numlines ; i++, mld++, ld++)
	{
		fixed_t dx,dy;
		ld->flags = LITTLESHORT(mld->flags);
		ld->special = (uint8_t)LITTLESHORT(mld->special);
		ld->tag = (uint8_t)LITTLESHORT(mld->tag);
		ld->v1 = LITTLESHORT(mld->v1);
		ld->v2 = LITTLESHORT(mld->v2);
		v1 = &vertexes[ld->v1];
		v2 = &vertexes[ld->v2];
		dx = v2->x - v1->x;
		dy = v2->y - v1->y;
		if (!dx)
			ld->flags |= ML_ST_VERTICAL;
		else if (!dy)
			ld->flags |= ML_ST_HORIZONTAL;
		else
		{
			if (FixedDiv (dy , dx) > 0)
				ld->flags |= ML_ST_POSITIVE;
			else
				ld->flags |= ML_ST_NEGATIVE;
		}

		ld->sidenum[0] = LITTLESHORT(mld->sidenum[0]);
		ld->sidenum[1] = LITTLESHORT(mld->sidenum[1]);
	}
}


/*
=================
=
= P_LoadSideDefs
=
=================
*/

void P_LoadSideDefs (int lump)
{
	byte			*data;
	int				i;
	mapsidedef_t	*msd;
	side_t			*sd;

#ifndef MARS
	for (i=0 ; i<numtextures ; i++)
		textures[i].usecount = 0;
#endif	
	numsides = W_LumpLength (lump) / sizeof(mapsidedef_t);
	sides = Z_Malloc (numsides*sizeof(side_t)+16,PU_LEVEL);
	sides = (void*)(((uintptr_t)sides + 15) & ~15); // aline on cacheline boundary
	D_memset (sides, 0, numsides*sizeof(side_t));
	data = I_TempBuffer ();
	W_ReadLump (lump,data);
	
	msd = (mapsidedef_t *)data;
	sd = sides;
	for (i=0 ; i<numsides ; i++, msd++, sd++)
	{
		int rowoffset, textureoffset;
		textureoffset = LITTLESHORT(msd->textureoffset);
		rowoffset = LITTLESHORT(msd->rowoffset);
		sd->toptexture = R_TextureNumForName(msd->toptexture);
		sd->bottomtexture = R_TextureNumForName(msd->bottomtexture);
		sd->midtexture = R_TextureNumForName(msd->midtexture);
		sd->sector = LITTLESHORT(msd->sector);
		sd->rowoffset = rowoffset & 0xff;
		sd->textureoffset = (textureoffset & 0xfff) | ((rowoffset & 0x0f00) << 4);
#ifndef MARS
		textures[sd->toptexture].usecount++;
		textures[sd->bottomtexture].usecount++;
		textures[sd->midtexture].usecount++;
#endif
	}
}



/*
=================
=
= P_LoadBlockMap
=
=================
*/

void P_LoadBlockMap (int lump)
{
	int		count;

#ifdef MARS
	blockmaplump = (short *)W_POINTLUMPNUM(lump);
#else
	int		i;
	
	blockmaplump = W_CacheLumpNum (lump,PU_LEVEL);
	blockmap = blockmaplump+4;
	count = W_LumpLength (lump)/2;
	for (i=0 ; i<count ; i++)
		blockmaplump[i] = LITTLESHORT(blockmaplump[i]);
#endif
		
	bmaporgx = blockmaplump[0]<<FRACBITS;
	bmaporgy = blockmaplump[1]<<FRACBITS;
	bmapwidth = blockmaplump[2];
	bmapheight = blockmaplump[3];
	
/* clear out mobj chains */
	count = sizeof(*blocklinks)* bmapwidth*bmapheight;
	blocklinks = Z_Malloc (count,PU_LEVEL);
	D_memset (blocklinks, 0, count);
}




/*
=================
=
= P_GroupLines
=
= Builds sector line lists and subsector sector numbers
= Finds block bounding boxes for sectors
=================
*/

void P_GroupLines (void)
{
	VINT		*linebuffer;
	int			i, total;
	sector_t	*sector;
	subsector_t	*ss;
	seg_t		*seg;
	line_t		*li;

/* look up sector number for each subsector */
	ss = subsectors;
	for (i=0 ; i<numsubsectors ; i++, ss++)
	{
		side_t *sidedef;
		line_t* linedef;

		seg = &segs[ss->firstline];
		linedef = &lines[seg->linedef];
		sidedef = &sides[linedef->sidenum[seg->sideoffset & 1]];
		ss->sector = &sectors[sidedef->sector];
	}

/* count number of lines in each sector */
	li = lines;
	total = 0;
	for (i=0 ; i<numlines ; i++, li++)
	{
		sector_t *front = LD_FRONTSECTOR(li);
		sector_t *back = LD_BACKSECTOR(li);

		front->linecount++;
		total++;
		if (back && back != front)
		{
			back->linecount++;
			total++;
		}
	}
	
/* build line tables for each sector	 */
	linebuffer = Z_Malloc (total*sizeof(*linebuffer), PU_LEVEL);
	sector = sectors;
	for (i=0 ; i<numsectors ; i++, sector++)
	{
		sector->lines = linebuffer;
		linebuffer += sector->linecount;
		sector->linecount = 0;
	}

	li = lines;
	for (i=0 ; i<numlines ; i++, li++)
	{
		sector_t *front = LD_FRONTSECTOR(li);
		sector_t *back = LD_BACKSECTOR(li);

		front->lines[front->linecount++] = i;
		if (back && back != front)
			back->lines[back->linecount++] = i;
	}
}

/*============================================================================= */

/*
=================
=
= P_SetupLevel
=
=================
*/

void P_SetupLevel (int lumpnum)
{
#ifndef MARS
	mobj_t	*mobj;
#endif
	extern	int	cy;
	int gamezonemargin;
	
	M_ClearRandom ();

	stagefailed = true;
	leveltime = 0;
	
D_printf ("P_SetupLevel(%i)\n",lumpnum);

	P_InitThinkers ();

	R_ResetTextures();

/* note: most of this ordering is important	 */
	P_LoadBlockMap (lumpnum+ML_BLOCKMAP);
	P_LoadVertexes (lumpnum+ML_VERTEXES);
	P_LoadSectors (lumpnum+ML_SECTORS);
	P_LoadSideDefs (lumpnum+ML_SIDEDEFS);
	P_LoadLineDefs (lumpnum+ML_LINEDEFS);
	P_LoadSubsectors (lumpnum+ML_SSECTORS);
	P_LoadNodes (lumpnum+ML_NODES);
	P_LoadSegs (lumpnum+ML_SEGS);

#ifdef MARS
	rejectmatrix = (byte *)W_POINTLUMPNUM(lumpnum+ML_REJECT);
#else
	rejectmatrix = W_CacheLumpNum (lumpnum+ML_REJECT,PU_LEVEL);
#endif

	validcount = Z_Malloc((numlines + 1) * sizeof(*validcount) * 2, PU_LEVEL);
	D_memset(validcount, 0, (numlines + 1) * sizeof(*validcount) * 2);
	validcount[0] = 1; // cpu 0
	validcount[numlines] = 1; // cpu 1

	P_GroupLines ();

	if (netgame == gt_deathmatch)
	{
		deathmatchstarts = Z_Malloc(sizeof(*deathmatchstarts) * MAXDMSTARTS, PU_LEVEL);
	}
	else
	{
		deathmatchstarts = NULL;
	}

	deathmatch_p = deathmatchstarts;
	P_LoadThings (lumpnum+ML_THINGS);

/* */
/* if deathmatch, randomly spawn the active players */
/* */
	if (netgame == gt_deathmatch)
	{
		int i;
		for (i=0 ; i<MAXPLAYERS ; i++)
			if (playeringame[i])
			{	/* must give a player spot before deathmatchspawn */
				mobj_t *mobj = P_SpawnMobj (deathmatchstarts[0].x<<16
				,deathmatchstarts[0].y<<16,0, MT_PLAYER);
				players[i].mo = mobj;
				G_DeathMatchSpawnPlayer (i);
				P_RemoveMobj (mobj);
			}
	}

/* set up world state */
	P_SpawnSpecials ();
	ST_InitEveryLevel ();
	
/*I_Error("free memory: 0x%x\n", Z_FreeMemory(mainzone)); */
/*printf ("free memory: 0x%x\n", Z_FreeMemory(mainzone)); */

	cy = 4;

#ifdef JAGUAR
{
extern byte *debugscreen;
	D_memset (debugscreen,0,32*224);
	
}
#endif

	gamepaused = false;

	gamezonemargin = DEFAULT_GAME_ZONE_MARGIN;
	R_SetupLevel(gamezonemargin);

	I_SetThreadLocalVar(DOOMTLS_VALIDCOUNT, &validcount[0]);

#ifdef MARS
	Mars_CommSlaveClearCache();
#endif
}


/*
=================
=
= P_Init
=
=================
*/

void P_Init (void)
{
	anims = Z_Malloc(sizeof(*anims) * MAXANIMS, PU_STATIC);
	linespeciallist = Z_Malloc(sizeof(*linespeciallist) * MAXLINEANIMS, PU_STATIC);

	activeplats = Z_Malloc(sizeof(*activeplats) * MAXPLATS, PU_STATIC);
	activeceilings = Z_Malloc(sizeof(*activeceilings) * MAXCEILINGS, PU_STATIC);

	P_InitPicAnims ();
#ifndef MARS
	pausepic = W_CacheLumpName ("PAUSED",PU_STATIC);
#endif
}
