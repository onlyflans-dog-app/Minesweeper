#pragma once

#define SOFTNAME    "Purin's Minesweeper"
#define SOFTVERSION "00.00.01-dev"
#define SOFTAUTHOR  "onlyflans.dog"
#define SOFTYEAR    "2023"

#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"
#include "stdarg.h"
#include "string.h"
#include "math.h"
#include "assert.h"
#include "limits.h"
#include "dirent.h"
#include "time.h"

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"
#include <SDL2/SDL_mixer.h>

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#include "../../Nuklear/nuklear.h"
#include "nuklear_sdl_renderer.h"

#define USESDL2
#define USENUKLEAR

#define SOFTBUILD   __DATE__ " " __TIME__

struct inputs {
    int mouse_x, mouse_y;
    int scroll;
    Uint32 mouse_buttons;
    uint8_t key[SDL_NUM_SCANCODES];
    uint8_t hasFocus;
};

struct sdlContexts {
	SDL_Window*  w;                                                             // windows contex
	SDL_Surface* s;                                                             // surface contex
    SDL_Renderer* r;                                                            // renderer contex
    struct nk_context *g;                                                       // gui contex

    SDL_Rect res;                                                               // curent resolution
    SDL_Rect oldRes;                                                            // previous resolution

    struct inputs now;                                                          // curent input contex
    struct inputs prev;                                                         // previous inputs contex

    const uint8_t *kstate;                                                      // ptr to realtime keyboard state (avoid using, use curent/previous keyboard state instead)

    int gameState;                                                              // current game state/mode
};
