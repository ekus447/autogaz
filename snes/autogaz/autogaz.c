#include <snes.h>

extern char patterns, patterns_end;
extern char palette;
extern char map, map_end;
extern char soundbrr,soundbrrend;
brrsamples tadasound;

extern char snesfont;

int frames;
u8 keyapressed=0;

int main(void) {

spcBoot();

	u16 scrX=0, scrY=0;
	u16 pad0, move;

	consoleInit();
	spcAllocateSoundRegion(159);
	spcSetSoundEntry(15, 15, 4, &soundbrrend-&soundbrr, &soundbrr, &tadasound);
	consoleInitText(0, 1, &snesfont);
	bgInitTileSet(1, &patterns, &palette, 0, (&patterns_end - &patterns), 16*2, BG_16COLORS, 0x4000);
	bgInitMapSet(1, &map, (&map_end - &map),SC_64x64, 0x1000);
	setMode(BG_MODE1,0);  bgSetDisable(2);
	setScreenOn();

	int state = -1;

	while(1) {

    move = 0;

		scanPads();


		if ((padsCurrent(0) & KEY_A) && state == -1) {
			if (keyapressed ==0) {
				keyapressed = 1;
				scrY = 200;
				move = 1;
				state = 0;
				spcPlaySound(0);
			}
		}  if ((padsCurrent(0) & KEY_B) && state == 3) {
				spcStop();
				scrX = 0;
				scrY = 0;
				move = 1;
				state = -1;
				consoleDrawText(1,5,"                   ");
				consoleDrawText(4,7,"                        ");
				consoleDrawText(4,8,"            ");
 		}
		else
			keyapressed = 0;


			if (state == 1 && (frames > 380)) {
				state = 2;
				spcStop();
			}

			if (state == 0) {
				scrY = 210;
				move = 1;
				spcPlaySound(0);
				state = 1;
			} else if (state == 1) {
				frames += 1;
			} else if (state == 2) {
				scrX = 800;
				scrY = 800;
				move = 1;
				frames = 0;
				state = 3;
			} else if (state == 3) {
				consoleDrawText(1,5,"POLAT ABI DIYOR KI:");
				consoleDrawText(4,7,"\"Afern evlat, artik bir");
				consoleDrawText(4,8,"super gucsn!\"");
				spcStop();
			}

			if (move) bgSetScroll(1, scrX,scrY);


			spcProcess();
			WaitForVBlank();

	}
	return 0;
}
