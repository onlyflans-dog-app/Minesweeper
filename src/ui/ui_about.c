#include "_include.h"

#include "ui.h"

int ui_about_menu(struct sdlContexts* s)
{
    struct nk_context *ctx = s->g;

    if(nk_begin(ctx, "About Menu", nk_rect(0, 0, s->res.w, s->res.h), NK_WINDOW_BACKGROUND)){
        nk_layout_row_dynamic(ctx, 30, 1);

        nk_layout_row_dynamic(ctx, 30, 3);

        nk_spacer(ctx);
        if(nk_button_label(ctx, "Visit the official website")){
            ca6Utils_openWebpage("https://onlyflans.dog/game/minesweeper");
        }
        nk_spacer(ctx);

        nk_spacer(ctx);
        if(nk_button_label(ctx, "About Inukai Purin")){
            ca6Utils_openWebpage("https://twitter.com/inukaipurin");
        }
        nk_spacer(ctx);

        nk_spacer(ctx);
        if(nk_button_label(ctx, "About OnlyFlans")){
            ca6Utils_openWebpage("https://onlyflans.dog");
        }
        nk_spacer(ctx);

        nk_layout_row_dynamic(ctx, 30, 1);
        nk_spacer(ctx);

        nk_layout_row_dynamic(ctx, 30, 3);
        nk_spacer(ctx);
        if(nk_button_label(ctx, "Back")){
            s->gameState = GAME_MODE_MAIN;
        }
        nk_spacer(ctx);

    }
    nk_end(ctx);

    return 0;
}