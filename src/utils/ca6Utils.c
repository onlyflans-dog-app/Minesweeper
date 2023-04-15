#include "_include.h"
#include "ca6Utils.h"


#ifdef __CYGWIN__
#include "Windows.h"
#endif

int ca6Utils_openWebpage(char* url)
{
    #ifdef __CYGWIN__
        ShellExecute(NULL, "open", url, NULL, NULL, SW_SHOWNORMAL);
    #endif
        
    #ifdef __LINUX__
        char command[256];
        snprintf(command, sizeof(command), "xdg-open %s", url);
        system(command);
    #endif

    return 0;
}


struct ca6_rects * ca6Utils_setRect(struct ca6_rects *cr, int x, int y, int w, int h)
{
    if(cr == NULL){
        return NULL;
    }

    cr->x = x;
    cr->y = y;
    cr->w = w;
    cr->h = h;

    return cr;
}

int ca6Utils_isInRect(struct ca6_rects *cr, int x, int y)
{
    if(cr->x <= x && cr->x+cr->w >= x){
        if(cr->y <= y && cr->y+cr->h >= y){
            return 1;
        }
    }
    return 0;
}

int ca6Utils_minMax(int value, int min, int max)
{
    if(value < min){
        return min;
    } else if(value > max){
        return max;
    }
    return value;
}

int ca6Utils_minMaxRO(int value, int min, int max)
{
    if(value < min){
        return max;
    } else if(value > max){
        return min;
    }
    return value;
}


#ifdef USESDL2

struct ca6_rects* ca6Utils_sdlRect_ca6Rect(SDL_Rect* sr, struct ca6_rects* cr)
{
    if(sr == NULL || cr == NULL) {
        return NULL;
    }
    cr->x = sr->x;
    cr->y = sr->y;
    cr->w = sr->w;
    cr->h = sr->h;
    return cr;
}

SDL_Rect* ca6Utils_ca6Rect_sdlRect(struct ca6_rects* cr, SDL_Rect* sr)
{
    if(sr == NULL || cr == NULL) {
        return NULL;
    }
    sr->x = cr->x;
    sr->y = cr->y;
    sr->w = cr->w;
    sr->h = cr->h;
    return sr;
}

#endif

#ifdef USENUKLEAR

struct ca6_rects* ca6Utils_nkRect_ca6Rect(struct nk_rect * nr, struct ca6_rects* cr)
{
    if(nr == NULL || cr == NULL) {
        return NULL;
    }
    cr->x = nr->x;
    cr->y = nr->y;
    cr->w = nr->w;
    cr->h = nr->h;
    return cr;
}

struct nk_rect* ca6Utils_ca6Rect_nkRect(struct ca6_rects* cr, struct nk_rect * nr)
{
    if(nr == NULL || cr == NULL) {
        return NULL;
    }
    nr->x = cr->x;
    nr->y = cr->y;
    nr->w = cr->w;
    nr->h = cr->h;
    return nr;
}

#endif
