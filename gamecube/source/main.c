//grrlib
#include <grrlib.h>

//required libas
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

//textures(png)
#include "after_png.h"
#include "before_png.h"
#include "polat_png.h"

//audio(mp3)
#include <asndlib.h>
#include <mp3player.h>
#include "music_mp3.h"

static void ExitGame();
#define SURE 9

GRRLIB_texImg *PolatIMG;
GRRLIB_texImg *BeforeIMG;
GRRLIB_texImg *AfterIMG;

int state = 0;
int ts;

int main() {

    GRRLIB_Init();
    PAD_Init();
    ASND_Init();
	  MP3Player_Init();

    PolatIMG = GRRLIB_LoadTexturePNG(polat_png);
    BeforeIMG = GRRLIB_LoadTexturePNG(before_png);
    AfterIMG = GRRLIB_LoadTexturePNG(after_png);


    while (true) {

        PAD_ScanPads();

        u16 buttonsDown = PAD_ButtonsDown(0);
        if (state == 0 && buttonsDown & PAD_BUTTON_A) {
          state = 1;
        } else if (state == 0 && buttonsDown & PAD_BUTTON_START) {
          ExitGame();
        }

        int tsCur = time(NULL);
        if (tsCur == ts + SURE) state = 3;

        if (state == 0) {
            GRRLIB_DrawImg( 0, 0, BeforeIMG, 0, 1, 1, RGBA(255, 255, 255, 255) );
        } else if (state == 1) {
            GRRLIB_DrawImg( 0, 0, AfterIMG, 0, 1, 1, RGBA(255, 255, 255, 255) );
            ts = time(NULL);
            MP3Player_PlayBuffer(music_mp3, music_mp3_size, NULL);
            state = 2;
        } else if (state == 2) {
            GRRLIB_DrawImg( 0, 0, AfterIMG, 0, 1, 1, RGBA(255, 255, 255, 255) );
        } else if (state == 3) {
            GRRLIB_DrawImg( 0, 0, PolatIMG, 0, 1, 1, RGBA(255, 255, 255, 255) );
            if (buttonsDown & PAD_BUTTON_A) state = 0;
        }


        GRRLIB_Render();

    }

    ExitGame();
    return 0;
}

static void ExitGame() {

    GRRLIB_FreeTexture(PolatIMG);
    GRRLIB_FreeTexture(BeforeIMG);
    GRRLIB_FreeTexture(AfterIMG);
    GRRLIB_Exit();


    exit(0);
}
