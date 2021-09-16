#include <pspkernel.h>
#include <pspctrl.h>

#include <pspaudio.h>
#include <pspaudiolib.h>
#include <psppower.h>
#include "mp3player.h"

#include "./gLib2D/glib2d.h"
#include "./callbacks.h"

PSP_MODULE_INFO("App",0,1,1);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER | THREAD_ATTR_VFPU);

#include <stdio.h>
#include <time.h>
#define SURE 8

int main()
{
  callbacks_setup();
  pspAudioInit();
  MP3_Init(1);

  SceCtrlData pad;
  g2dTexture* before = g2dTexLoad("resources/before.png", G2D_SWIZZLE);
  g2dTexture* after = g2dTexLoad("resources/after.png", G2D_SWIZZLE);
  g2dTexture* polat = g2dTexLoad("resources/polat.png", G2D_SWIZZLE);
  MP3_Load("resources/agm.mp3");

  int alpha = 255,
      x = G2D_SCR_W/2,
      y = G2D_SCR_H/2,
      w = (before == NULL ? 10 : before->w),
      h = (before == NULL ? 10 : before->h),
      rot = 0;

  int state = 0;
  int ts;

  while (1)
  {
    sceCtrlPeekBufferPositive(&pad,1);
    g2dClear(WHITE);

    bool button_x_clicked = false;
    bool button_o_clicked = false;
    if (pad.Buttons & PSP_CTRL_CROSS) button_x_clicked = true;
    if (pad.Buttons & PSP_CTRL_CIRCLE) button_o_clicked = true;

    if (state == 0 && button_x_clicked) state = 1;

    int tsCur = time(NULL);
    if (tsCur == ts + SURE) state = 3;

    if (state == 0) {
        g2dBeginRects(before);
    } else if (state == 1) {
        g2dBeginRects(after);
        ts = time(NULL);
        MP3_Play();
        state = 2;
    } else if (state == 2) {
        g2dBeginRects(after);
    } else if (state == 3) {
        MP3_Stop();
        g2dBeginRects(polat);
        if (button_o_clicked) state = 0;
    }

    if ((before == NULL) | (after == NULL) | (polat == NULL)) g2dSetColor(RED);
    g2dSetCoordMode(G2D_CENTER);
    g2dSetAlpha(alpha);
    g2dSetScaleWH(w,h);
    g2dSetCoordXY(x,y);
    g2dSetRotation(rot);
    g2dAdd();
    g2dEnd();

    g2dFlip(G2D_VSYNC);
  }

  MP3_Stop();
  MP3_FreeTune();
  sceKernelExitGame();
  return 0;
}
