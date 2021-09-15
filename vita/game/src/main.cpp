#include <psp2/kernel/processmgr.h>
#include <psp2/ctrl.h>
#include <vita2d.h>
#include <psp2/audioout.h>
#include "soloud.h"
#include "soloud_wav.h"

#include <math.h>
#include <stdio.h>
#include <time.h>

#define BUTTON_X_VAL 000040000
#define BUTTON_O_VAL 000020000
#define SURE 8

SoLoud::Soloud gSoloud;
SoLoud::Wav agmWave;

int main(void)
{
    srand (time(NULL));

    gSoloud.init();

    sceCtrlSetSamplingMode(SCE_CTRL_MODE_ANALOG);

    const int screenWidth = 960;
    const int screenHeight = 544;
    
    vita2d_texture *beforeSprite;
    vita2d_texture *afterSprite;
    vita2d_texture *polatSprite;

    agmWave.load("app0:/resources/agm.ogg");
    
    vita2d_init();
    vita2d_set_clear_color(RGBA8(238, 228, 225, 255));
 
    beforeSprite = vita2d_load_PNG_file("app0:resources/before.png");
    afterSprite = vita2d_load_PNG_file("app0:resources/after.png");
    polatSprite = vita2d_load_PNG_file("app0:resources/polat.png");
    
    vita2d_set_vblank_wait(1);

    int state = 0;
    int ts;

    SceCtrlData ctrl;

    while (1)   
    {
        sceCtrlPeekBufferPositive(0, &ctrl, 1);
        unsigned int button = ctrl.buttons;

        vita2d_start_drawing();		
        vita2d_clear_screen();

        bool button_x_clicked = false;
        bool button_o_clicked = false;
        if (button == BUTTON_X_VAL) button_x_clicked = true;
        if (button == BUTTON_O_VAL) button_o_clicked = true;

        if (state == 0 && button_x_clicked) state = 1;

        int tsCur = time(NULL);
        if (tsCur == ts + SURE) state = 3;
		
        if (state == 0) {
            vita2d_draw_texture(beforeSprite, 0, 0);
        } else if (state == 1) {
            vita2d_draw_texture(afterSprite, 0, 0);
            ts = time(NULL);
            gSoloud.play(agmWave);
            state = 2;
        } else if (state == 2) {
            vita2d_draw_texture(afterSprite, 0, 0);
        } else if (state == 3) {
            vita2d_draw_texture(polatSprite, 0, 0);
            if (button_o_clicked) state = 0;
        }

        vita2d_end_drawing();
        vita2d_swap_buffers();
    }
    
    vita2d_fini();
    vita2d_free_texture(beforeSprite);
    vita2d_free_texture(afterSprite);
    vita2d_free_texture(polatSprite);
          
    sceKernelExitProcess(0);
    return 0;
}