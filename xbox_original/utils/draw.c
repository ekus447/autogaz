#include <hal/debug.h>
#include <hal/xbox.h>
#include <hal/video.h>
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <SDL.h>
#include <SDL_image.h>

static const int SCREEN_WIDTH = 640;
static const int SCREEN_HEIGHT = 480;

SDL_Surface *screenSurface;
SDL_Window *window;

int initAGImage(void)
{
    
    SDL_LogSetPriority(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO);

    window = SDL_CreateWindow("autogaz",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        SCREEN_WIDTH, SCREEN_HEIGHT,
        SDL_WINDOW_SHOWN);

    if(window == NULL)
    {
        debugPrint( "Window could not be created!\n");
        SDL_VideoQuit();
    }

    if (!(IMG_Init(IMG_INIT_JPG) & IMG_INIT_JPG)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't intialize SDL_image.\n");
        SDL_VideoQuit();
    }

    screenSurface = SDL_GetWindowSurface(window);
    if (!screenSurface) {
        SDL_VideoQuit();
    }

    return 0;
}


int drawAGImage(SDL_Surface *imageSurface) {
    SDL_BlitSurface(imageSurface, NULL, screenSurface, NULL);
    SDL_UpdateWindowSurface(window);
    return 0;
}