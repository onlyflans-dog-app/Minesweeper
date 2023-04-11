#include "_include.h"

#include "ui.h"

int uidebug = 0;

int ui_debug_menu(struct sdlContexts* s)
{
    struct nk_context *ctx = s->g;

    if(s->now.key[SDL_SCANCODE_F1] && !s->prev.key[SDL_SCANCODE_F1]){
        uidebug = !uidebug;
    }

    if(uidebug){
        if(nk_begin(ctx, "Debug Menu", nk_rect(0, 0, s->res.w, s->res.h), NK_WINDOW_NO_SCROLLBAR)){
            nk_layout_row_dynamic(ctx, 30, 1);
            nk_label(ctx, "Debug Option", NK_TEXT_CENTERED);

            nk_layout_row_dynamic(ctx, 30, 3);

            if(nk_button_label(ctx, "Main Menu")){
                s->gameState = GAME_MODE_MAIN;
                uidebug = 0;
            }

            nk_spacer(ctx);

        }
        nk_end(ctx);
    }

    return 0;
}