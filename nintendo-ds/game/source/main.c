#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <nds.h>
#include <nf_lib.h>
#include <time.h>

#define TIMER_SPEED (BUS_CLOCK / 1024)
#define __SURE 7

typedef enum
{
	timerState_Stop,
	timerState_Pause,
	timerState_Running
} TimerStates;

int main(int argc, char **argv)
{
	NF_Set2D(0, 0);
	NF_Set2D(1, 0);
	NF_Set2D(2, 0);

	consoleDemoInit();
	iprintf("\n NitroFS init. Please wait.\n\n");

	swiWaitForVBlank();

	NF_SetRootFolder("NITROFS");

	soundEnable();
	NF_InitRawSoundBuffers();

	NF_Set2D(0, 5);
	NF_Set2D(1, 5);
	NF_Set2D(2, 5);

	NF_InitBitmapBgSys(0, 1);
	NF_InitBitmapBgSys(1, 1);
	NF_InitBitmapBgSys(2, 1);

	NF_Init16bitsBgBuffers();

	NF_Init16bitsBackBuffer(0);
	NF_Init16bitsBackBuffer(1);
	NF_Init16bitsBackBuffer(2);

	NF_Enable16bitsBackBuffer(0);
	NF_Enable16bitsBackBuffer(1);
	NF_Enable16bitsBackBuffer(2);

	NF_LoadBMP("bmp/before", 0);
	NF_LoadBMP("bmp/after", 1);
	NF_LoadBMP("bmp/polat", 2);

	NF_LoadRawSound("sfx/agm", 0, 11025, 0);

	s8 before = 0;
	s8 after = 1;
	s8 polat = 2;

	u16 keys = 0;
	int state = 0;

	uint ticks = 0;
	TimerStates tstate = timerState_Stop;

	while (1)
	{
		scanKeys();
		keys = keysDown();

		if (tstate == timerState_Running)
		{
			ticks += timerElapsed(0);
		}

		if (state == 0 && (keys & KEY_B))
			state = 1;

		int secs = ticks / TIMER_SPEED;

		if (secs == __SURE)
			state = 3;

		if (state == 0)
		{
			NF_Draw16bitsImage(0, before, 0, 0, false);
		}
		else if (state == 1)
		{
			NF_Draw16bitsImage(0, after, 0, 0, false);
			timerStart(0, ClockDivider_1024, 0, NULL);
			tstate = timerState_Running;
			NF_PlayRawSound(0, 127, 64, false, 0);
			state = 2;
		}
		else if (state == 2)
		{
			NF_Draw16bitsImage(0, after, 0, 0, false);
		}
		else if (state == 3)
		{
			timerStop(0);
			ticks = 0;
			tstate = timerState_Stop;
			state = 4;
		}
		else if (state == 4)
		{
			NF_Draw16bitsImage(0, polat, 0, 0, false);
			if (keys & KEY_A)
				state = 0;
		}

		swiWaitForVBlank();

		NF_Flip16bitsBackBuffer(0);
		NF_Flip16bitsBackBuffer(1);
		NF_Flip16bitsBackBuffer(2);
	}

	return 0;
}
