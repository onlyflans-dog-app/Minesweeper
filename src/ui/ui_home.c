#include "_include.h"

#include "ui.h"

int ui_main_menu(struct sdlContexts* s)
{
    struct nk_context *ctx = s->g;

    if(nk_begin(ctx, "Main Menu", nk_rect(0, 0, s->res.w, s->res.h), NK_WINDOW_BACKGROUND)){
        nk_layout_row_dynamic(ctx, 30, 1);
        //nk_label(ctx, SOFTNAME, NK_TEXT_CENTERED);

        nk_layout_row_dynamic(ctx, 120, 1);
        nk_spacer(ctx);
        
        nk_layout_row_dynamic(ctx, 30, 3);

        nk_spacer(ctx);
        if(nk_button_label(ctx, "New Game")){
            s->gameState = GAME_MODE_NEWGAME;
        }
        nk_spacer(ctx);

        // nk_spacer(ctx);
        // if(nk_button_label(ctx, "Settings")){
        //     s->gameState = GAME_MODE_SETTINGS;
        // }
        // nk_spacer(ctx);

        nk_spacer(ctx);
        if(nk_button_label(ctx, "About")){
            s->gameState = GAME_MODE_ABOUT;
        }
        nk_spacer(ctx);

        nk_spacer(ctx);
        if(nk_button_label(ctx, "Exit")){
            exit(0);
        }
        nk_spacer(ctx);
    }
    nk_end(ctx);

    return 0;
}