#include "_include.h"

#include "ui.h"

// place holder for setting menu
int offset = 0;
int speed = 0;


int useFlanTheme = 1;

int ui_setting_menu(struct sdlContexts* s)
{
    struct nk_context *ctx = s->g;

    if(nk_begin(ctx, "Setting Menu", nk_rect(0, 0, s->res.w, s->res.h), NK_WINDOW_NO_SCROLLBAR)){


        nk_layout_row_dynamic(ctx, 30, 1);
        nk_label(ctx, "Setting", NK_TEXT_CENTERED);
        nk_spacer(ctx);

        nk_layout_row_dynamic(ctx, 30, 3);
/*
        nk_spacer(ctx);
        speed = nk_propertyi(ctx, "Speed", -100, speed, 100, 1, 1);
        nk_spacer(ctx);

        nk_spacer(ctx);
        offset = nk_propertyi(ctx, "Offset", -1000, offset, 1000, 1, 1);
        nk_spacer(ctx);

        nk_spacer(ctx);
        nk_checkbox_label(ctx, "Use Flan Theme", &useFlanTheme);
        nk_spacer(ctx);

*/
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