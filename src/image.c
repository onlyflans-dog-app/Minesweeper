#include "_include.h"
#include "image.h"

int image_load(struct sdlContexts* sc, struct images* si, char* path)
{
    SDL_Surface* img;

    if(sc == NULL || si == NULL){
        return -1;
    }

    img = IMG_Load(path);
	if(img == NULL)
	{
		return -1;
	}

    si->t = SDL_CreateTextureFromSurface(sc->r, img);
    SDL_FreeSurface(img);

    return 0;
}

int image_draw(struct sdlContexts* sc, struct images* si, int x, int y, int w, int h)
{
    SDL_Rect pos;

    if(sc == NULL || si == NULL){
        return -1;
    }

    pos.x = x;
    pos.y = y;
    pos.w = w;
    pos.h = h;

    SDL_RenderCopy(sc->r, si->t, NULL, &pos);

	return 0;
}