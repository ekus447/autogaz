#ifndef audio_header__
#define audio_header__

#ifdef __cplusplus
extern "C"
{
#endif

#include <hal/debug.h>
#include <hal/video.h>
#include <hal/xbox.h>
#include <windows.h>
#include <string.h>
#include <hal/audio.h>
#include <xboxkrnl/xboxkrnl.h>
#include <assert.h>

int initAGM(void);
int playAGM(void);

#ifdef __cplusplus
}
#endif

#endif