#ifndef draw_header__
#define draw_header__

#ifdef __cplusplus
extern "C"
{
#endif

#include <hal/debug.h>
#include <hal/xbox.h>
#include <hal/video.h>
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <SDL.h>
#include <SDL_image.h>


int initAGImage(void);
void drawAGImage(SDL_Surface *imageSurface);

#ifdef __cplusplus
}
#endif

#endif