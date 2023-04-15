#include "_include.h"

#include "ui/ui.h"
#include "image.h"


#ifndef __linux__
#include "Windows.h"
#endif

struct sdlContexts sc;

int main_sdlInit(struct sdlContexts* s);
int main_sldEvent(void);
int main_theme(struct sdlContexts* s);
int main_startScreen(struct sdlContexts* sc);

#ifdef main
#undef main
#endif

int main(int argc, char* argv[])
{
    struct nk_context *ctx;
    float font_scale = 1;

#ifdef __CYGWIN__
    HWND windowHandle = GetConsoleWindow();
    ShowWindow(windowHandle,SW_HIDE);
#endif

    memset(&sc, 0, sizeof(struct sdlContexts));

    printf("%s %s %s\r\n", SOFTNAME, SOFTVERSION, SOFTBUILD);fflush(stdout);
    printf("© %s %s\r\n", SOFTYEAR, SOFTAUTHOR);fflush(stdout);

    sc.res.w = 1025;
    sc.res.h = 620;

    main_sdlInit(&sc);

    ctx = nk_sdl_init(sc.w, sc.r);
    sc.g = ctx;



    {
        int render_w, render_h;
        int window_w, window_h;
        float scale_x, scale_y;
        SDL_GetRendererOutputSize(sc.r, &render_w, &render_h);
        SDL_GetWindowSize(sc.w, &window_w, &window_h);
        scale_x = (float)(render_w) / (float)(window_w);
        scale_y = (float)(render_h) / (float)(window_h);
        SDL_RenderSetScale(sc.r, scale_x, scale_y);
        font_scale = scale_y;
    }

    struct nk_font_atlas *atlas;
    struct nk_font_config config = nk_font_config(0);
    struct nk_font *font;
    nk_sdl_font_stash_begin(&atlas);
    font = nk_font_atlas_add_from_file(atlas, "./res/DroidSans.ttf", 16 * font_scale, &config);
    if(!font){
        printf("Error: font not found\r\n");
        font = nk_font_atlas_add_default(atlas, 13 * font_scale, &config);
    }
    nk_sdl_font_stash_end();
    font->handle.height /= font_scale;
    nk_style_set_font(ctx, &font->handle);

    main_theme(&sc);

    SDL_Surface* icon = IMG_Load("./res/paw.ico");
    SDL_SetWindowIcon(sc.w, icon);

    sc.gameState = GAME_MODE_STARTSCREEN;

    main_theme(&sc);


    // load image background
    struct images ibg;
    image_load(&sc, &ibg, "./res/bg.png");



    while(1){
        main_theme(&sc);

        SDL_GetWindowSize(sc.w, &sc.res.w, &sc.res.h);
        SDL_GetWindowPosition(sc.w, &sc.res.x, &sc.res.y);

        nk_input_begin(ctx);
        main_sldEvent();
        nk_input_end(ctx);

        SDL_SetRenderDrawColor(sc.r, 0, 0, 0, 255);
        SDL_RenderFillRect(sc.r, NULL);

// ********************************************************************************************************
// game update start



// game update end
// ********************************************************************************************************
// pre NK game render start

    if(sc.gameState != GAME_MODE_STARTSCREEN){
        image_draw(&sc, &ibg, 0, 0, sc.res.w, sc.res.h);
    }


// pre NK game render end
// ********************************************************************************************************
// NK gui start

        if(!uidebug){
            switch(sc.gameState){
                case GAME_MODE_STARTSCREEN:
                    main_startScreen(&sc);
                break;
                case GAME_MODE_MAIN:
                    ui_main_menu(&sc);
                break;

                case GAME_MODE_NEWGAME:
                    ui_newGame_menu(&sc);
                break;
                case GAME_MODE_GAME:
                case GAME_MODE_WIN:
                    ui_game_menu(&sc);
                break;

                case GAME_MODE_SETTINGS:
                    ui_setting_menu(&sc);
                break;
                case GAME_MODE_ABOUT:
                    ui_about_menu(&sc);
                break;
                default:
                    ui_main_menu(&sc);
                break;
            }
        }

// NK gui end
// ********************************************************************************************************

        nk_sdl_render(NK_ANTI_ALIASING_ON);

// ********************************************************************************************************
// post NK game render start



// post NK game render end
// ********************************************************************************************************

        SDL_RenderPresent(sc.r);
        SDL_Delay(50);
    }

    return 0;
}

int main_sdlInit(struct sdlContexts* s)
{
    SDL_SetHint("SDL_HINT_MOUSE_FOCUS_CLICKTHROUGH", "1");
    SDL_SetHint(SDL_HINT_VIDEO_HIGHDPI_DISABLED, "0");

    if (SDL_Init(SDL_INIT_EVERYTHING)) {
        return -1;
    }

    sc.w = SDL_CreateWindow(SOFTNAME, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, s->res.w, s->res.h, SDL_WINDOW_SHOWN);
    sc.r = SDL_CreateRenderer(sc.w , -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	sc.s = SDL_GetWindowSurface( sc.w );

    SDL_SetWindowMinimumSize(sc.w, s->res.w, s->res.h);
    SDL_SetRenderDrawBlendMode(sc.r, SDL_BLENDMODE_BLEND);

    sc.kstate = SDL_GetKeyboardState(NULL);

    return 0;
}

int main_sldEvent(void)
{
    SDL_PumpEvents();

    memcpy(&sc.prev, &sc.now, sizeof(struct inputs));

    sc.now.scroll = 0;

    SDL_Event ev;

    while (SDL_PollEvent( &ev )!=0) {
        nk_sdl_handle_event(&ev);
        switch (ev.type) {
            case SDL_QUIT:
                exit(0);
                break;
            case SDL_WINDOWEVENT:
                if(ev.window.event == SDL_WINDOWEVENT_CLOSE){
                    exit(0);
                }
            break;
            case SDL_MOUSEWHEEL:
                sc.now.scroll = ev.wheel.y;
            break;

            case SDL_MOUSEMOTION:
                if (SDL_GetMouseFocus() == sc.w) {
                    sc.now.hasFocus = 1;
                } else {
                    sc.now.hasFocus = 0;
                }
            break;
            case SDL_DROPFILE:
                //dropFile(ev.drop.file);
                //SDL_free(ev.drop.file);
                //ev.drop.file = NULL;
            break;

            default:
                break;
        }
    }

    sc.now.mouse_buttons = SDL_GetMouseState(&sc.now.mouse_x, &sc.now.mouse_y);

    memcpy(&sc.now.key, sc.kstate, SDL_NUM_SCANCODES);

    return 0;
}

int main_theme(struct sdlContexts* s)
{
    struct nk_color table[NK_COLOR_COUNT];
    table[NK_COLOR_TEXT] = nk_rgba(0x00, 0x00, 0x00, 255);

    table[NK_COLOR_WINDOW] = nk_rgba(0xea, 0xde, 0xf1, 0);

    table[NK_COLOR_HEADER] = nk_rgba(0xfa, 0xad, 0xd6, 255);

    table[NK_COLOR_BORDER] = nk_rgba(0xB3, 0x84, 0xE7, 255);

    table[NK_COLOR_BUTTON] = nk_rgba(0xfa, 0xad, 0xd6, 255);
    table[NK_COLOR_BUTTON_HOVER] = nk_rgba(0xFF, 0xF0, 0xF8, 255);
    table[NK_COLOR_BUTTON_ACTIVE] = nk_rgba(0xE1, 0x89, 0xBD, 255);

    table[NK_COLOR_TOGGLE] = nk_rgba(0xfa, 0xad, 0xd6, 255);
    table[NK_COLOR_TOGGLE_HOVER] = nk_rgba(0xfa, 0xad, 0xd6, 255);
    table[NK_COLOR_TOGGLE_CURSOR] = nk_rgba(0x87, 0x58, 0xbc, 255);

    table[NK_COLOR_SELECT] = nk_rgba(57, 67, 61, 255);
    table[NK_COLOR_SELECT_ACTIVE] = nk_rgba(48, 83, 111, 255);

    table[NK_COLOR_SLIDER] = nk_rgba(0xB3, 0x84, 0xE7, 255);
    table[NK_COLOR_SLIDER_CURSOR] = nk_rgba(0xB3, 0x84, 0xE7, 255);
    table[NK_COLOR_SLIDER_CURSOR_HOVER] = nk_rgba(0xB3, 0x84, 0xE7, 255);
    table[NK_COLOR_SLIDER_CURSOR_ACTIVE] = nk_rgba(0xB3, 0x84, 0xE7, 255);

    table[NK_COLOR_PROPERTY] = nk_rgba(0xfa, 0xad, 0xd6, 255);

    table[NK_COLOR_EDIT] = nk_rgba(0xea, 0xde, 0xf1, 225);
    table[NK_COLOR_EDIT_CURSOR] = nk_rgba(0xB3, 0x84, 0xE7, 255);

    table[NK_COLOR_COMBO] = nk_rgba(0xea, 0xde, 0xf1, 255);

    table[NK_COLOR_CHART] = nk_rgba(50, 58, 61, 255);
    table[NK_COLOR_CHART_COLOR] = nk_rgba(48, 83, 111, 255);
    table[NK_COLOR_CHART_COLOR_HIGHLIGHT] = nk_rgba(255, 0, 0, 255);

    table[NK_COLOR_SCROLLBAR] = nk_rgba(0xfa, 0xad, 0xd6, 255);
    table[NK_COLOR_SCROLLBAR_CURSOR] = nk_rgba(0xE1, 0x89, 0xBD, 255);
    table[NK_COLOR_SCROLLBAR_CURSOR_HOVER] = nk_rgba(0xE1, 0x89, 0xBD, 255);
    table[NK_COLOR_SCROLLBAR_CURSOR_ACTIVE] = nk_rgba(0xE1, 0x89, 0xBD, 255);

    table[NK_COLOR_TAB_HEADER] = nk_rgba(48, 83, 111, 255);
    nk_style_from_table(s->g, table);
    return 0;
}


int main_themePurin(struct sdlContexts* s)
{
    struct nk_color table[NK_COLOR_COUNT];
    table[NK_COLOR_TEXT] = nk_rgba(0x87, 0x58, 0xbc, 255);

    table[NK_COLOR_WINDOW] = nk_rgba(0xea, 0xde, 0xf1, 0);

    table[NK_COLOR_HEADER] = nk_rgba(0xfa, 0xad, 0xd6, 255);

    table[NK_COLOR_BORDER] = nk_rgba(0xB3, 0x84, 0xE7, 255);

    table[NK_COLOR_BUTTON] = nk_rgba(0xfa, 0xad, 0xd6, 255);
    table[NK_COLOR_BUTTON_HOVER] = nk_rgba(0xFF, 0xF0, 0xF8, 255);
    table[NK_COLOR_BUTTON_ACTIVE] = nk_rgba(0xE1, 0x89, 0xBD, 255);

    table[NK_COLOR_TOGGLE] = nk_rgba(0xfa, 0xad, 0xd6, 255);
    table[NK_COLOR_TOGGLE_HOVER] = nk_rgba(0xfa, 0xad, 0xd6, 255);
    table[NK_COLOR_TOGGLE_CURSOR] = nk_rgba(0x87, 0x58, 0xbc, 255);

    table[NK_COLOR_SELECT] = nk_rgba(57, 67, 61, 255);
    table[NK_COLOR_SELECT_ACTIVE] = nk_rgba(48, 83, 111, 255);

    table[NK_COLOR_SLIDER] = nk_rgba(0xB3, 0x84, 0xE7, 255);
    table[NK_COLOR_SLIDER_CURSOR] = nk_rgba(0xB3, 0x84, 0xE7, 255);
    table[NK_COLOR_SLIDER_CURSOR_HOVER] = nk_rgba(0xB3, 0x84, 0xE7, 255);
    table[NK_COLOR_SLIDER_CURSOR_ACTIVE] = nk_rgba(0xB3, 0x84, 0xE7, 255);

    table[NK_COLOR_PROPERTY] = nk_rgba(0xfa, 0xad, 0xd6, 255);

    table[NK_COLOR_EDIT] = nk_rgba(0xea, 0xde, 0xf1, 225);
    table[NK_COLOR_EDIT_CURSOR] = nk_rgba(0xB3, 0x84, 0xE7, 255);

    table[NK_COLOR_COMBO] = nk_rgba(0xea, 0xde, 0xf1, 255);

    table[NK_COLOR_CHART] = nk_rgba(50, 58, 61, 255);
    table[NK_COLOR_CHART_COLOR] = nk_rgba(48, 83, 111, 255);
    table[NK_COLOR_CHART_COLOR_HIGHLIGHT] = nk_rgba(255, 0, 0, 255);

    table[NK_COLOR_SCROLLBAR] = nk_rgba(0xfa, 0xad, 0xd6, 255);
    table[NK_COLOR_SCROLLBAR_CURSOR] = nk_rgba(0xE1, 0x89, 0xBD, 255);
    table[NK_COLOR_SCROLLBAR_CURSOR_HOVER] = nk_rgba(0xE1, 0x89, 0xBD, 255);
    table[NK_COLOR_SCROLLBAR_CURSOR_ACTIVE] = nk_rgba(0xE1, 0x89, 0xBD, 255);

    table[NK_COLOR_TAB_HEADER] = nk_rgba(48, 83, 111, 255);
    nk_style_from_table(s->g, table);
    return 0;
}

int main_themeFlan(struct sdlContexts* s)
{
    struct nk_color table[NK_COLOR_COUNT];
    table[NK_COLOR_TEXT] = nk_rgba(0xFF, 0xEE, 0xB3, 255);

    table[NK_COLOR_WINDOW] = nk_rgba(0x7D, 0x5B, 0x4A, 0);

    table[NK_COLOR_HEADER] = nk_rgba(0x58, 0x3D, 0x31, 255);

    table[NK_COLOR_BORDER] = nk_rgba(0xFF, 0xEE, 0xB3, 255);

    table[NK_COLOR_BUTTON] = nk_rgba(0x58, 0x3D, 0x31, 255);
    table[NK_COLOR_BUTTON_HOVER] = nk_rgba(0x9C, 0x6B, 0x4F, 255);
    table[NK_COLOR_BUTTON_ACTIVE] = nk_rgba(0x89, 0x5F, 0x42, 255);

    table[NK_COLOR_TOGGLE] = nk_rgba(0x9C, 0x6B, 0x4F, 255);
    table[NK_COLOR_TOGGLE_HOVER] = nk_rgba(0x9C, 0x6B, 0x4F, 255);
    table[NK_COLOR_TOGGLE_CURSOR] = nk_rgba(0xFF, 0xEE, 0xB3, 255);

    table[NK_COLOR_SELECT] = nk_rgba(57, 67, 61, 255);
    table[NK_COLOR_SELECT_ACTIVE] = nk_rgba(48, 83, 111, 255);

    table[NK_COLOR_SLIDER] = nk_rgba(0xFF, 0xEE, 0xB3, 255);
    table[NK_COLOR_SLIDER_CURSOR] = nk_rgba(0xFF, 0xEE, 0xB3, 255);
    table[NK_COLOR_SLIDER_CURSOR_HOVER] = nk_rgba(0xFF, 0xEE, 0xB3, 255);
    table[NK_COLOR_SLIDER_CURSOR_ACTIVE] = nk_rgba(0xFF, 0xEE, 0xB3, 255);

    table[NK_COLOR_PROPERTY] = nk_rgba(0x58, 0x3D, 0x31, 255);

    table[NK_COLOR_EDIT] = nk_rgba(0x7D, 0x5B, 0x4A, 225);
    table[NK_COLOR_EDIT_CURSOR] = nk_rgba(0xFF, 0xEE, 0xB3, 255);

    table[NK_COLOR_COMBO] = nk_rgba(0x7D, 0x5B, 0x4A, 255);

    table[NK_COLOR_CHART] = nk_rgba(50, 58, 61, 255);
    table[NK_COLOR_CHART_COLOR] = nk_rgba(48, 83, 111, 255);
    table[NK_COLOR_CHART_COLOR_HIGHLIGHT] = nk_rgba(255, 0, 0, 255);

    table[NK_COLOR_SCROLLBAR] = nk_rgba(0xA7, 0x7F, 0x6D, 255);
    table[NK_COLOR_SCROLLBAR_CURSOR] = nk_rgba(0x57, 0x3B, 0x2E, 255);
    table[NK_COLOR_SCROLLBAR_CURSOR_HOVER] = nk_rgba(0x57, 0x3B, 0x2E, 255);
    table[NK_COLOR_SCROLLBAR_CURSOR_ACTIVE] = nk_rgba(0x57, 0x3B, 0x2E, 255);

    table[NK_COLOR_TAB_HEADER] = nk_rgba(48, 83, 111, 255);
    nk_style_from_table(s->g, table);
    return 0;
}


int main_startScreen(struct sdlContexts* sc)
{
    static int init = 0;

    static struct images si1;

    if(init == 0){
        init = 1;
        image_load(sc, &si1, "res/pawflan.png");

    }

    image_draw(sc, &si1, sc->res.w/2-200/2, sc->res.h/2-200/2, 200, 200);

    if(sc->now.key[SDL_SCANCODE_SPACE] == 1 || sc->now.key[SDL_SCANCODE_RETURN] == 1){
        sc->gameState = GAME_MODE_MAIN;
    }

    return 0;
}