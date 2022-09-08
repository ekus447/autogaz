#include <jo/jo.h>

static jo_palette palette1;

int state = 0;

static int before_id;
static int after_id;
static int polat_id;

int startTS = 0;

void draw(void)
{
    jo_sprite_set_palette(palette1.id);
	jo_sprite_draw3D(0, 0, 0, 500);
}

jo_palette *tga_palette_handling(void)
{
    jo_create_palette(&palette1);
    return (&palette1);
}

void playAGM(void) {
    jo_audio_play_cd_track(2, 3, false);
}

void initImages() {
    before_id = jo_sprite_add_tga("TEX", "BEFORE.TGA", JO_COLOR_Transparent);
    after_id = jo_sprite_add_tga("TEX", "AFTER.TGA", JO_COLOR_Transparent);
    polat_id = jo_sprite_add_tga("TEX", "POLAT.TGA", JO_COLOR_Transparent);
}

void gamepad(void)
{
    if (!jo_is_pad1_available())
        return ;

    if (state == 0 && jo_is_pad1_key_down(JO_KEY_A)) //numpad 1
    {
        state = 1;
    } else if (state == 3 && jo_is_pad1_key_down(JO_KEY_B)) //numpad 2
    {
        state = 0;
    }
}

void stateHandler(void) {
    int curTS = jo_get_ticks(); // 1 second = 1000 tick
    if ((startTS != 0) && state == 2 && (curTS >= startTS + 7500)) state = 3; // 7.5 sec

    switch(state) {
        case 0:
            jo_sprite_draw3D(before_id, 0, 0, 500);
            break;
        case 1:
            jo_sprite_draw3D(after_id, 0, 0, 500);
            startTS = jo_get_ticks();
            playAGM();
            state = 2;
            break;
        case 2:
            jo_sprite_draw3D(after_id, 0, 0, 500);
            break;
        case 3:
            jo_sprite_draw3D(polat_id, 0, 0, 500);
            startTS = 0;
            break;
    }
}

void jo_main(void)
{
	jo_core_init(JO_COLOR_DarkGray);
	jo_set_tga_palette_handling(tga_palette_handling);
    initImages();

	jo_core_add_callback(draw);
    jo_core_add_callback(stateHandler);
    jo_core_add_callback(gamepad);
	jo_core_run();
}