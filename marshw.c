/*
  Victor Luchits

  The MIT License (MIT)

  Copyright (c) 2021 Victor Luchits, Joseph Fenton

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
*/

#include "marshw.h"

static volatile uint16_t mars_activescreen = 0;

static char mars_gamepadport[MARS_MAX_CONTROLLERS];
static char mars_mouseport;
static volatile uint16_t mars_controlval[2];

volatile unsigned mars_vblank_count = 0;
volatile unsigned mars_pwdt_ovf_count = 0;
volatile unsigned mars_swdt_ovf_count = 0;
unsigned mars_frtc2msec_frac = 0;
const uint8_t* mars_newpalette = NULL;
uint16_t mars_thru_rgb_reference = 0;

int16_t mars_requested_lines = 224;
uint16_t mars_framebuffer_height = 224;

uint16_t mars_cd_ok = 0;
uint16_t mars_num_cd_tracks = 0;

uint16_t mars_refresh_hz = 0;

const int NTSC_CLOCK_SPEED = 23011360; // HZ
const int PAL_CLOCK_SPEED = 22801467; // HZ

static volatile int16_t mars_brightness = 0;

void sec_dma1_handler(void) MARS_ATTR_DATA_CACHE_ALIGN;
void pri_cmd_handler(void) MARS_ATTR_DATA_CACHE_ALIGN;
void sec_cmd_handler(void) MARS_ATTR_DATA_CACHE_ALIGN;

static void intr_handler_stub(void) MARS_ATTR_DATA_CACHE_ALIGN;
static void intr_handler_stub(void) {}

static void (*pri_cmd_cb)(void) = &intr_handler_stub;
static void (*sci_cmd_cb)(void) = &intr_handler_stub;
static void (*sci_dma1_cb)(void) = &intr_handler_stub;

void Mars_WaitFrameBuffersFlip(void)
{
	while ((MARS_VDP_FBCTL & MARS_VDP_FS) != mars_activescreen);
}

void Mars_FlipFrameBuffers(char wait)
{
	mars_activescreen = !mars_activescreen;
	MARS_VDP_FBCTL = mars_activescreen;
	if (wait) Mars_WaitFrameBuffersFlip();
}

char Mars_FramebuffersFlipped(void)
{
	return (MARS_VDP_FBCTL & MARS_VDP_FS) == mars_activescreen;
}

void Mars_InitLineTable(void)
{
	int j;
	int blank;
	int offset = 0; // 224p or 240p
	volatile unsigned short* lines = &MARS_FRAMEBUFFER;

	// initialize the lines section of the framebuffer

	if (mars_requested_lines == -240)
	{
		// letterboxed 240p
		offset = (240 - 224) / 2;
	}

	for (j = 0; j < mars_framebuffer_height; j++)
		lines[offset+j] = j * 320 / 2 + 0x100;

	blank = j * 320 / 2;

	// set the rest of the line table to a blank line
	for (; j < 256; j++)
		lines[offset+j] = blank + 0x100;

	for (j = 0; j < offset; j++)
		lines[j] = blank + 0x100;

	// make sure blank line is clear
	for (j = blank; j < (blank + 160); j++)
		lines[j] = 0;
}

void Mars_SetBrightness(int16_t brightness)
{
	mars_brightness = brightness;
}

int Mars_BackBuffer(void) {
	return mars_activescreen;
}

char Mars_UploadPalette(const uint8_t* palette)
{
	int	i;
	unsigned short* cram = (unsigned short *)&MARS_CRAM;
	int br = mars_brightness;

	if ((MARS_SYS_INTMSK & MARS_SH2_ACCESS_VDP) == 0)
		return 0;

	for (i = 0; i < 256; i++) {
		int r = br + *palette++;
		int g = br + *palette++;
		int b = br + *palette++;

		if (r < 0) r = 0; else if (r > 255) r = 255;
		if (g < 0) g = 0; else if (g > 255) g = 255;
		if (b < 0) b = 0; else if (b > 255) b = 255;

		unsigned b1 = b;
		unsigned g1 = g;
		unsigned r1 = r;

		b1 = ((b1 >> 3) & 0x1f) << 10;
		g1 = ((g1 >> 3) & 0x1f) << 5;
		r1 = ((r1 >> 3) & 0x1f) << 0;

		cram[i] = 0x8000 | r1 | g1 | b1;
	}

	#ifdef MDSKY
	// Allow MD VDP to show through for this palette index.
	cram[MARS_MD_PIXEL_THRU_INDEX] = cram[mars_thru_rgb_reference] & 0x7FFF;
	#endif

	return 1;
}

int Mars_PollMouse(void)
{
	unsigned int mouse1, mouse2;
	int port = mars_mouseport;

	if (port < 0)
		return -1;

	while (MARS_SYS_COMM0); // wait until 68000 has responded to any earlier requests
	MARS_SYS_COMM0 = 0x0500 | port; // tells 68000 to read mouse
	while (MARS_SYS_COMM0 == (0x0500 | port)); // wait for mouse value

	mouse1 = MARS_SYS_COMM0;
	mouse2 = MARS_SYS_COMM2;
	MARS_SYS_COMM0 = 0; // tells 68000 we got the mouse value

	return (int)((mouse1 << 16) | mouse2);
}

int Mars_ParseMousePacket(int mouse, int* pmx, int* pmy)
{
	int mx, my;

	// (YO XO YS XS S  M  R  L  X7 X6 X5 X4 X3 X2 X1 X0 Y7 Y6 Y5 Y4 Y3 Y2 Y1 Y0)

	mx = ((unsigned)mouse >> 8) & 0xFF;
	// check overflow
	if (mouse & 0x00400000)
		mx = (mouse & 0x00100000) ? -256 : 256;
	else if (mouse & 0x00100000)
		mx |= 0xFFFFFF00;

	my = mouse & 0xFF;
	// check overflow
	if (mouse & 0x00800000)
		my = (mouse & 0x00200000) ? -256 : 256;
	else if (mouse & 0x00200000)
		my |= 0xFFFFFF00;

	*pmx = mx;
	*pmy = my;

	return mouse;
}

int Mars_GetWDTCount(void)
{
	unsigned int cnt = SH2_WDT_RTCNT;
	return (int)((mars_pwdt_ovf_count << 8) | cnt);
}

void Mars_InitVideo(int lines)
{
	int i;
	char NTSC;
	int mars_lines = lines == 240 || lines == -240 ? MARS_240_LINES : MARS_224_LINES;

	while ((MARS_SYS_INTMSK & MARS_SH2_ACCESS_VDP) == 0);

	MARS_VDP_DISPMODE = mars_lines | MARS_VDP_MODE_256;
	NTSC = (MARS_VDP_DISPMODE & MARS_NTSC_FORMAT) != 0;

	// change 4096.0f to something else if WDT TCSR is changed!
	mars_frtc2msec_frac = 4096.0f * 1000.0f / (NTSC ? NTSC_CLOCK_SPEED : PAL_CLOCK_SPEED) * 65536.0f;

	mars_refresh_hz = NTSC ? 60 : 50;
	mars_requested_lines = lines;
	mars_framebuffer_height = lines == 240 ? 240 : 224;
	mars_activescreen = MARS_VDP_FBCTL;

	Mars_FlipFrameBuffers(1);

	for (i = 0; i < 2; i++)
	{
		volatile int* p, * p_end;

		Mars_InitLineTable();

		p = (int*)(&MARS_FRAMEBUFFER + 0x100);
		p_end = (int*)p + 320 / 4 * mars_framebuffer_height;
		do {
			*p = 0;
		} while (++p < p_end);

		Mars_FlipFrameBuffers(1);
	}

	Mars_SetMDColor(1, 0);
}

void Mars_Init(void)
{
	int i;

	/* no controllers or mouse by default */
	for (i = 0; i < MARS_MAX_CONTROLLERS; i++)
		mars_gamepadport[i] = -1;
	mars_mouseport = -1;

	//SH2_WDT_WTCSR_TCNT = 0xA518; /* WDT TCSR = clr OVF, IT mode, timer off, clksel = Fs/2 */
	
	SH2_WDT_WTCSR_TCNT = 0x5A00; /* WDT TCNT = 0 */
	SH2_WDT_WTCSR_TCNT = 0xA53E; /* WDT TCSR = clr OVF, IT mode, timer on, clksel = Fs/4096 */

	/* init hires timer system */
	SH2_WDT_VCR = (65<<8) | (SH2_WDT_VCR & 0x00FF); // set exception vector for WDT
	SH2_INT_IPRA = (SH2_INT_IPRA & 0xFF0F) | 0x0020; // set WDT INT to priority 2

	Mars_UpdateCD();

	if (mars_cd_ok & 0x1)
	{
		/* if the CD is present, give it seconds to init */
		Mars_WaitTicks(180);
	}
}

uint16_t* Mars_FrameBufferLines(void)
{
	uint16_t* lines = (uint16_t*)&MARS_FRAMEBUFFER;
	if (mars_requested_lines == -240)
		lines += (240 - 224) / 2;
	return lines;
}

void Mars_ReadSRAM(uint8_t * buffer, int offset, int len)
{
	uint8_t *ptr = buffer;

	while (MARS_SYS_COMM0);
	while (len-- > 0) {
		MARS_SYS_COMM2 = offset++;
		MARS_SYS_COMM0 = 0x0100;    /* Read SRAM */
		while (MARS_SYS_COMM0);
		*ptr++ = MARS_SYS_COMM2 & 0x00FF;
	}
}

void Mars_WriteSRAM(const uint8_t* buffer, int offset, int len)
{
	const uint8_t *ptr = buffer;

	while (MARS_SYS_COMM0);
	while (len-- > 0) {
		MARS_SYS_COMM2 = offset++;
		MARS_SYS_COMM0 = 0x0200 | *ptr++;    /* Write SRAM */
		while (MARS_SYS_COMM0);
	}
}

void Mars_UpdateCD(void)
{
	while (MARS_SYS_COMM0);
	MARS_SYS_COMM0 = 0x0600;
	while (MARS_SYS_COMM0);
	mars_cd_ok = MARS_SYS_COMM2;
	mars_num_cd_tracks = (mars_cd_ok >> 2) & 63;
	mars_cd_ok = mars_cd_ok & 0x3;
}

void Mars_UseCD(int usecd)
{
	while (MARS_SYS_COMM0);

	if (!mars_cd_ok)
		return;

	MARS_SYS_COMM2 = usecd & 1;
	MARS_SYS_COMM0 = 0x0700;
	while (MARS_SYS_COMM0);
}

void Mars_PlayTrack(char usecd, int playtrack, void *vgmptr, int vgmsize, char looping)
{
	Mars_UseCD(usecd);

	if (!usecd)
	{
		int i;
		uint16_t s[4];

		s[0] = (uintptr_t)vgmsize>>16, s[1] = (uintptr_t)vgmsize&0xffff;
		s[2] = (uintptr_t)vgmptr >>16, s[3] = (uintptr_t)vgmptr &0xffff;

		for (i = 0; i < 4; i++) {
			MARS_SYS_COMM2 = s[i];
			MARS_SYS_COMM0 = 0x0301+i;
			while (MARS_SYS_COMM0);
		}
	}

	MARS_SYS_COMM2 = playtrack | (looping ? 0x8000 : 0x0000);
	MARS_SYS_COMM0 = 0x0300; /* start music */
	while (MARS_SYS_COMM0);
}

void Mars_MCDLoadSfx(uint16_t id, void *data, uint32_t data_len)
{
	int i;
	uint16_t s[4];

	while (MARS_SYS_COMM0);

	s[0] = (uintptr_t)data_len>>16, s[1] = (uintptr_t)data_len&0xffff;
	s[2] = (uintptr_t)data>>16, s[3] = (uintptr_t)data&0xffff;

	for (i = 0; i < 4; i++) {
		MARS_SYS_COMM2 = s[i];
		MARS_SYS_COMM0 = 0x1D01+i;
		while (MARS_SYS_COMM0);
	}

	MARS_SYS_COMM2 = id;
	MARS_SYS_COMM0 = 0x1D00; /* load sfx */
	while (MARS_SYS_COMM0);
}

void Mars_MCDPlaySfx(uint8_t src_id, uint16_t buf_id, uint8_t pan, uint8_t vol)
{
	if (src_id == 0)
		return;

	while (MARS_SYS_COMM0);

	MARS_SYS_COMM2 = (pan<<8)|vol;
	MARS_SYS_COMM0 = 0x1E00|src_id;
	while (MARS_SYS_COMM0);

	MARS_SYS_COMM2 = buf_id;
	MARS_SYS_COMM0 = 0x1E01;

	while (MARS_SYS_COMM0);
}

int Mars_MCDGetSfxPlaybackStatus(void)
{
	while (MARS_SYS_COMM0);
	MARS_SYS_COMM0 = 0x1F00;
	while (MARS_SYS_COMM0);
	return MARS_SYS_COMM2;
}

void Mars_MCDClearSfx(void)
{
	while (MARS_SYS_COMM0);
	MARS_SYS_COMM0 = 0x2000;
	while (MARS_SYS_COMM0);
}

void Mars_MCDUpdateSfx(uint8_t src_id, uint8_t pan, uint8_t vol)
{
	if (src_id == 0)
		return;

	while (MARS_SYS_COMM0);
	MARS_SYS_COMM2 = (pan<<8)|vol;
	MARS_SYS_COMM0 = 0x2100|src_id;
	while (MARS_SYS_COMM0);
}

void Mars_MCDStopSfx(uint8_t src_id)
{
	if (src_id == 0)
		return;
	while (MARS_SYS_COMM0);
	MARS_SYS_COMM0 = 0x2200|src_id;
	while (MARS_SYS_COMM0);
}

void Mars_MCDFlushSfx(void)
{
	while (MARS_SYS_COMM0);
	MARS_SYS_COMM0 = 0x2300;
	while (MARS_SYS_COMM0);
}

void Mars_StopTrack(void)
{
	while (MARS_SYS_COMM0);
	MARS_SYS_COMM0 = 0x0400; /* stop music */
	while (MARS_SYS_COMM0);
}

void Mars_SetMusicVolume(uint8_t volume)
{
	while (MARS_SYS_COMM0);
	MARS_SYS_COMM0 = 0x1800|volume;
	while (MARS_SYS_COMM0);
}

void Mars_WaitTicks(int ticks)
{
	unsigned ticend = mars_vblank_count + ticks;
	while (mars_vblank_count < ticend);
}

/*
 *  MD network functions
 */

static inline unsigned short GetNetByte(void)
{
	while (MARS_SYS_COMM0);
	MARS_SYS_COMM0 = 0x1200;	/* get a byte from the network */
	while (MARS_SYS_COMM0);
	return MARS_SYS_COMM2;		/* status:byte */
}

/*
 *  Get a byte from the network. The number of ticks to wait for a byte
 *  is passed in. A wait time of 0 means return immediately. The return
 *  value is -2 for a timeout/no bytes are waiting, -1 if a network error
 *  occurred, and 0 to 255 for a received byte.
 */
int Mars_GetNetByte(int wait)
{
	unsigned short ret;
	unsigned ticend;

	if (!wait)
	{
		/* no wait - return a value immediately */
		ret = GetNetByte();
		return (ret == 0xFF00) ? -2 : (ret & 0xFF00) ? -1 : (int)(ret & 0x00FF);
	}

	/* quick check for byte in rec buffer */
	ret = GetNetByte();
	if (ret != 0xFF00)
		return (ret & 0xFF00) ? -1 : (int)(ret & 0x00FF);

	/* nothing waiting - do timeout loop */
	ticend = mars_vblank_count + wait;
	while (mars_vblank_count < ticend)
	{
		ret = GetNetByte();
		if (ret == 0xFF00)
			continue;	/* no bytes waiting */
		/* GetNetByte returned a byte or a net error */
		return (ret & 0xFF00) ? -1 : (int)(ret & 0x00FF);
	}
	return -2;	/* timeout */
}

/*
 *  Put a byte to the network. Returns -1 if timeout.
 */
int Mars_PutNetByte(int val)
{
	while (MARS_SYS_COMM0);
	MARS_SYS_COMM0 = 0x1300 | (val & 0x00FF);	/* send a byte to the network */
	while (MARS_SYS_COMM0);
	return (MARS_SYS_COMM2 == 0xFFFF) ? -1 : 0;
}

void Mars_SetupNet(int type)
{
	while (MARS_SYS_COMM0);
	MARS_SYS_COMM0 = 0x1400 | (type & 255);		/* init joyport 2 for networking */
	while (MARS_SYS_COMM0);
}

void Mars_CleanupNet(void)
{
	while (MARS_SYS_COMM0);
	MARS_SYS_COMM0 = 0x1500;		/* cleanup networking */
	while (MARS_SYS_COMM0);
}

void Mars_SetNetLinkTimeout(int timeout)
{
	while (MARS_SYS_COMM0);
	MARS_SYS_COMM2 = timeout;
	MARS_SYS_COMM0 = 0x1700;
	while (MARS_SYS_COMM0);
}


/*
 *  MD video debug functions
 */
void Mars_SetMDCrsr(int x, int y)
{
	while (MARS_SYS_COMM0);
	MARS_SYS_COMM2 = (x<<6)|y;
	MARS_SYS_COMM0 = 0x0800;			/* set current md cursor */
}

void Mars_GetMDCrsr(int *x, int *y)
{
	unsigned t;
	while (MARS_SYS_COMM0);
	MARS_SYS_COMM0 = 0x0900;			/* get current md cursor */
	while (MARS_SYS_COMM0);
	t = MARS_SYS_COMM2;
	*y = t & 31;
	*x = t >> 6;
}

void Mars_SetMDColor(int fc, int bc)
{
	while (MARS_SYS_COMM0);
	MARS_SYS_COMM0 = 0x0A00 | (bc << 4) | fc;			/* set font fg and bg colors */
}

void Mars_GetMDColor(int *fc, int *bc)
{
	while (MARS_SYS_COMM0);
	for (MARS_SYS_COMM0 = 0x0B00; MARS_SYS_COMM0;);		/* get font fg and bg colors */
	*fc = (unsigned)(MARS_SYS_COMM2 >> 0) & 15;
	*bc = (unsigned)(MARS_SYS_COMM2 >> 4) & 15;
}

void Mars_SetMDPal(int cpsel)
{
	while (MARS_SYS_COMM0);
	MARS_SYS_COMM0 = 0x0C00 | cpsel;	/* set palette select */
}

void Mars_MDPutChar(char chr)
{
	while (MARS_SYS_COMM0);
	MARS_SYS_COMM0 = 0x0D00 | chr;		/* put char at current cursor pos */
}

void Mars_ClearNTA(void)
{
	while (MARS_SYS_COMM0);
	MARS_SYS_COMM0 = 0x0E00;			/* clear name table a */
}

void Mars_MDPutString(char *str)
{
	while (*str)
		Mars_MDPutChar(*str++);
}

void Mars_SetBankPage(int bank, int page)
{
	while (MARS_SYS_COMM0);
	MARS_SYS_COMM0 = 0x1600 | (page<<3) | bank;
	while (MARS_SYS_COMM0);
}

void Mars_SetBankPageSec(int bank, int page)
{
	volatile unsigned short bcomm4 = MARS_SYS_COMM4;

	MARS_SYS_COMM4 = 0x1600 | (page<<3) | bank;
	while (MARS_SYS_COMM4 != 0x1000);

	MARS_SYS_COMM4 = bcomm4;
}

int Mars_ROMSize(void)
{
	return *((volatile uint32_t *)0x020001a4) - *((volatile uint32_t *)0x020001a0) + 1;
}

void Mars_DetectInputDevices(void)
{
	unsigned i;
	unsigned ctrl_wait = 0xFF00;

	mars_mouseport = -1;
	for (i = 0; i < MARS_MAX_CONTROLLERS; i++)
		mars_gamepadport[i] = -1;

	for (i = 0; i < MARS_MAX_CONTROLLERS; i++)
	{
		/* wait on COMM0 */
		while (MARS_SYS_COMM0 != ctrl_wait);

		int val = MARS_SYS_COMM2;
		if (val == 0xF000)
		{
			mars_controlval[i] = 0;
		}
		else if (val == 0xF001)
		{
			mars_mouseport = i;
			mars_controlval[i] = 0;
		}
		else
		{
			mars_gamepadport[i] = i;
			mars_controlval[i] |= val;
		}

		MARS_SYS_COMM0 = ++ctrl_wait;
		++ctrl_wait;
	}

	/* swap controller 1 and 2 around if the former isn't present */
	if (mars_gamepadport[0] < 0 && mars_gamepadport[1] >= 0)
	{
		mars_gamepadport[0] = mars_gamepadport[1];
		mars_gamepadport[1] = -1;
	}
}

int Mars_ReadController(int ctrl)
{
	int val;
	int port;

	if (ctrl < 0 || ctrl >= MARS_MAX_CONTROLLERS)
		return -1;

	port = mars_gamepadport[ctrl];
	if (port < 0)
		return -1;

	val = mars_controlval[port];
	mars_controlval[port] = 0;
	return val;
}

#ifdef MDSKY
/*
Fade the MD palette
*/
void Mars_FadeMDPaletteFromBlack(int fade_degree)
{
	while (MARS_SYS_COMM0);
	MARS_SYS_COMM2 = fade_degree;
	MARS_SYS_COMM0 = 0x1001;
}

void Mars_ScrollMDSky(short scroll_x, short scroll_y_base, short scroll_y_offset, short scroll_y_pan) {
	while (MARS_SYS_COMM0);
	MARS_SYS_COMM2 = scroll_y_base;
	MARS_SYS_COMM0 = 0x1101;

	while (MARS_SYS_COMM0);
	MARS_SYS_COMM2 = scroll_y_offset;
	MARS_SYS_COMM0 = 0x1102;

	while (MARS_SYS_COMM0);
	MARS_SYS_COMM2 = scroll_y_pan;
	MARS_SYS_COMM0 = 0x1103;

	while (MARS_SYS_COMM0);
	MARS_SYS_COMM2 = scroll_x;
	MARS_SYS_COMM0 = 0x1104;
}

/*
Load the MD sky tiles, palettes, and pattern name table into the MD VDP.
*/
void Mars_LoadMDSky(void *sky_metadata_ptr,
		void *sky_names_a_ptr, int sky_names_a_size,
		void *sky_names_b_ptr, int sky_names_b_size,
		void *sky_palettes_ptr, int sky_palettes_size,
		void *sky_tiles_ptr, int sky_tiles_size)
{
	int i;

	uint16_t s[4];


	// Load metadata

	s[0] = 0, s[1] = 8;
	s[2] = ((uintptr_t)sky_metadata_ptr >>16), s[3] = (uintptr_t)sky_metadata_ptr &0xffff;

	for (i = 0; i < 4; i++) {
		while (MARS_SYS_COMM0);
		MARS_SYS_COMM2 = s[i];
		MARS_SYS_COMM0 = 0x0F01+i;
	}


	// Load pattern name table A

	s[0] = (uintptr_t)sky_names_a_size>>16, s[1] = (uintptr_t)sky_names_a_size&0xffff;
	s[2] = ((uintptr_t)sky_names_a_ptr >>16), s[3] = (uintptr_t)sky_names_a_ptr &0xffff;

	for (i = 0; i < 4; i++) {
		while (MARS_SYS_COMM0);
		MARS_SYS_COMM2 = s[i];
		MARS_SYS_COMM0 = 0x0F01+i;
	}


	// Load pattern name table B

	s[0] = (uintptr_t)sky_names_b_size>>16, s[1] = (uintptr_t)sky_names_b_size&0xffff;
	s[2] = ((uintptr_t)sky_names_b_ptr >>16), s[3] = (uintptr_t)sky_names_b_ptr &0xffff;

	for (i = 0; i < 4; i++) {
		while (MARS_SYS_COMM0);
		MARS_SYS_COMM2 = s[i];
		MARS_SYS_COMM0 = 0x0F01+i;
	}


	// Load palettes

	s[0] = (uintptr_t)sky_palettes_size>>16, s[1] = (uintptr_t)sky_palettes_size&0xffff;
	s[2] = ((uintptr_t)sky_palettes_ptr >>16), s[3] = (uintptr_t)sky_palettes_ptr &0xffff;

	for (i = 0; i < 4; i++) {
		while (MARS_SYS_COMM0);
		MARS_SYS_COMM2 = s[i];
		MARS_SYS_COMM0 = 0x0F01+i;
	}


	// Load tiles

	s[0] = (uintptr_t)sky_tiles_size>>16, s[1] = (uintptr_t)sky_tiles_size&0xffff;
	s[2] = ((uintptr_t)sky_tiles_ptr >>16), s[3] = (uintptr_t)sky_tiles_ptr &0xffff;

	for (i = 0; i < 4; i++) {
		while (MARS_SYS_COMM0);
		MARS_SYS_COMM2 = s[i];
		MARS_SYS_COMM0 = 0x0F01+i;
	}

	while (MARS_SYS_COMM0);
}
#endif

void Mars_CtlMDVDP(int sel)
{
	while (MARS_SYS_COMM0);
	MARS_SYS_COMM0 = 0x1900 | (sel & 0x00FF);
	while (MARS_SYS_COMM0);
}

void Mars_StoreWordColumnInMDVRAM(int c)
{
	while (MARS_SYS_COMM0);
	MARS_SYS_COMM0 = 0x1A00|c;		/* sel = to VRAM, column in LB of comm0, start move */
}

void Mars_LoadWordColumnFromMDVRAM(int c, int offset, int len)
{
	while (MARS_SYS_COMM0 != 0);
	MARS_SYS_COMM2 = (((uint16_t)len)<<8) | offset;  /* (length<<8)|offset */
	MARS_SYS_COMM0 = 0x1B00|c;		/* sel = to VRAM, column in LB of comm0, start move */
}

void Mars_SwapWordColumnWithMDVRAM(int c)
{
    while (MARS_SYS_COMM0);
    MARS_SYS_COMM0 = 0x1C00|c;        /* sel = swap with VRAM, column in LB of comm0, start move */
}

void Mars_Finish(void)
{
	while (MARS_SYS_COMM0 != 0);
}

void pri_vbi_handler(void)
{
	mars_vblank_count++;

	if (mars_newpalette)
	{
		if (Mars_UploadPalette(mars_newpalette))
			mars_newpalette = NULL;
	}
}

void Mars_SetPriCmdCallback(void (*cb)(void))
{
	sci_cmd_cb = cb;
}

void Mars_SetSecCmdCallback(void (*cb)(void))
{
	pri_cmd_cb = cb;
}

void Mars_SetSecDMA1Callback(void (*cb)(void))
{
	sci_dma1_cb = cb;
}

/* ======================== INTERRUPT HANDLERS ======================== */

void pri_cmd_handler(void)
{
	switch (MARS_SYS_COMM0)
	{
		case 0xFF00:
			Mars_DetectInputDevices();
			break;
		default:
			pri_cmd_cb();
			break;
	}
}

void sec_cmd_handler(void)
{
	sci_cmd_cb();
}

void sec_dma1_handler(void)
{
	SH2_DMA_CHCR1; // read TE
	SH2_DMA_CHCR1 = 0; // clear TE

	sci_dma1_cb();
}
