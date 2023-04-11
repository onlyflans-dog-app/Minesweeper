#pragma once

#include "_include.h"


struct images {
    SDL_Texture* t;
};

int image_draw(struct sdlContexts* s, struct images* img, int x, int y, int w, int h);
int image_load(struct sdlContexts* s, struct images* si, char* path);