#include <hal/debug.h>
#include <hal/video.h>
#include <string>
#include <vector>
#include <windows.h>
#include <SDL.h>
#include <SDL_image.h>

#include "utils/input.h"
#include "utils/audio.h"
#include "utils/draw.h"

#define SURE 3601 // ok tamam şimdi bu 7 sn fln sebebini sorgulama
int state = 0;

SDL_Surface *before, *after, *polat;

int freeGaz(void) {
    input_free();
    SDL_Quit();
    return 0;
}

static void printSDLErrorAndReboot(void)
{
    debugPrint("SDL_Error: %s\n", SDL_GetError());
    debugPrint("Rebooting in 5 seconds.\n");
    Sleep(5000);
    XReboot();
}

static void printIMGErrorAndReboot(void)
{
    debugPrint("SDL_Image Error: %s\n", IMG_GetError());
    debugPrint("Rebooting in 5 seconds.\n");
    Sleep(5000);
    XReboot();
}

int initGaz(void) {
    input_init();
    initAGM();
    initAGImage();

    before = IMG_Load("D:\\assets\\before.jpg");
    after = IMG_Load("D:\\assets\\after.jpg");
    polat = IMG_Load("D:\\assets\\polat.jpg");

    if (!before || !after || !polat) {
        freeGaz();
        printIMGErrorAndReboot();
    }

    return 0;
}

int gaz(void) {

    initGaz();

    while (true) {
        
        input_poll();
        // 0: before, 1: after, 2: none, 3: polat

        if (state == 0) {
            drawAGImage(before);
        } else if (state == 1) {
            drawAGImage(after);
            state = 2;
            playAGM();
            Sleep(SURE);
            state = 3;
        } else if (state == 3) {
            drawAGImage(polat);
        }

        if (state == 0 && input_button_down(SDL_CONTROLLER_BUTTON_A)) state = 1;
        if (state == 3 && input_button_down(SDL_CONTROLLER_BUTTON_A)) state = 0;
    }


    freeGaz();
    return 0;
}

int main(void) {
    XVideoSetMode(640, 480, 32, REFRESH_DEFAULT);

    gaz();
    return 0;
}