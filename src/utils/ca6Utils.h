#pragma once

#include "_include.h"

struct ca6_rects {
    int x;
    int y;
    int w;
    int h;
};


struct ca6_rects* ca6Utils_setRect(struct ca6_rects *cr, int x, int y, int w, int h);
int ca6Utils_isInRect(struct ca6_rects *cr, int x, int y);
int ca6Utils_minMax(int value, int min, int max);
int ca6Utils_minMaxRO(int value, int min, int max);



#ifdef USESDL2
struct ca6_rects* ca6Utils_sdlRect_ca6Rect(SDL_Rect* sr, struct ca6_rects* cr);
SDL_Rect* ca6Utils_ca6Rect_sdlRect(struct ca6_rects* cr, SDL_Rect* sr);
#endif

#ifdef USENUKLEAR
struct ca6_rects* ca6Utils_nkRect_ca6Rect(struct nk_rect * nr, struct ca6_rects* cr);
struct nk_rect* ca6Utils_ca6Rect_nkRect(struct ca6_rects* cr, struct nk_rect * nr);
#endif