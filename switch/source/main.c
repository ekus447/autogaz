#include <time.h>
#include <unistd.h>

#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_image.h>
#include <switch.h>

// some switch buttons
#define JOY_A     0
#define JOY_B     1
#define JOY_X     2
#define JOY_Y     3
#define JOY_PLUS  10
#define JOY_MINUS 11
#define JOY_LEFT  12
#define JOY_UP    13
#define JOY_RIGHT 14
#define JOY_DOWN  15

#define SCREEN_W 1280
#define SCREEN_H 720
#define wait 16
#define SURE 8

int state = 0;
int ts;
SDL_Texture *before_tex = NULL, *after_tex = NULL, *polat_tex = NULL;
Mix_Music *agm = NULL;
SDL_Rect sc_pos = { 0, 0, 0, 0 };
SDL_Window* window;

int initGaz(void) {
    romfsInit();
    chdir("romfs:/");

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_AUDIO);
    Mix_Init(MIX_INIT_OGG);
    IMG_Init(IMG_INIT_PNG);

    SDL_InitSubSystem(SDL_INIT_JOYSTICK);
    SDL_JoystickEventState(SDL_ENABLE);
    SDL_JoystickOpen(0);

    SDL_InitSubSystem(SDL_INIT_AUDIO);
    Mix_AllocateChannels(5);
    Mix_OpenAudio(48000, AUDIO_S16, 2, 4096);

    agm = Mix_LoadMUS("resources/agm.ogg");

    return 0;
}

int freeGaz(void) {
    SDL_DestroyTexture(before_tex);

    Mix_HaltChannel(-1);
    Mix_FreeMusic(agm);

    IMG_Quit();
    Mix_CloseAudio();
    Mix_Quit();
    SDL_Quit();
    romfsExit();

    return 0;
}

int drawAGImage(SDL_Renderer* renderer, SDL_Texture *texture) {
    if (texture) SDL_RenderCopy(renderer, texture, NULL, &sc_pos);
    return 0;
}

int main(int argc, char** argv) {

    initGaz();

    int exit_requested = 0;
    SDL_Event event;

    SDL_Window* window = SDL_CreateWindow("Auto Gaz", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_W, SCREEN_H, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);


    // load res's
    SDL_Surface *before = IMG_Load("resources/before.png");
    if (before) {
        sc_pos.w = before->w;
        sc_pos.h = before->h;
        before_tex = SDL_CreateTextureFromSurface(renderer, before);
        SDL_FreeSurface(before);
    }

    SDL_Surface *after = IMG_Load("resources/after.png");
    if (after) {
        sc_pos.w = after->w;
        sc_pos.h = after->h;
        after_tex = SDL_CreateTextureFromSurface(renderer, after);
        SDL_FreeSurface(after);
    }

    SDL_Surface *polat = IMG_Load("resources/polat.png");
    if (polat) {
        sc_pos.w = polat->w;
        sc_pos.h = polat->h;
        polat_tex = SDL_CreateTextureFromSurface(renderer, polat);
        SDL_FreeSurface(polat);
    }


    while (!exit_requested
        && appletMainLoop()
        ) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                exit_requested = 1;
            
            if (event.type == SDL_JOYBUTTONDOWN) {
                if (event.jbutton.button == JOY_B && state == 0)
                    state = 1;
                if (event.jbutton.button == JOY_B && state == 3)
                    state = 0;
            }

        }

        int tsCur = time(NULL);
        if (tsCur == ts + SURE) state = 3;

        if (state == 0) {
            drawAGImage(renderer, before_tex);
        } else if (state == 1) {
            drawAGImage(renderer, after_tex);
            ts = time(NULL);
            state = 2;
            Mix_PlayMusic(agm, 1);
        } else if (state == 2) {
            drawAGImage(renderer, after_tex);
        } else if (state == 3) {
            drawAGImage(renderer, polat_tex);
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(wait);
    }

    freeGaz();
    return 0;
}
