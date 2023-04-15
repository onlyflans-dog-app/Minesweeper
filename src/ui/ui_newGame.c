#include "_include.h"

#include "image.h"
#include "ui.h"

int nbMine = 10;
int mapSizeX = 9;
int mapSizeY = 9;
int grid[30][30];
int playerGrid[30][30];

nk_bool easy = nk_true;
nk_bool intermediate = nk_false;
nk_bool expert = nk_false;

#define mine 'X'
#define empty 'E'
#define flag 'F'

int ui_newGame_menu(struct sdlContexts* s)
{
    struct nk_context *ctx = s->g;

    if(nk_begin(ctx, "NewGame Menu", nk_rect(0, 0, s->res.w, s->res.h), NK_WINDOW_BACKGROUND)){
        nk_layout_row_dynamic(ctx, 30, 1);

        //nk_label(ctx, "New Game", NK_TEXT_CENTERED);

        nk_layout_row_dynamic(ctx, 30, 1);
        nk_spacer(ctx);
        nk_spacer(ctx);

        nk_layout_row_dynamic(ctx, 30, 3);

        nk_spacer(ctx);
        if(nk_button_label(ctx, "Easy")){
            nbMine = 10;
            mapSizeX = 9;
            mapSizeY = 9;
            intermediate = 0;
            expert = 0;
            mine_newMinefield(mapSizeX, mapSizeY, nbMine);
            s->gameState = GAME_MODE_GAME;
        }
        nk_spacer(ctx);

        nk_spacer(ctx);
        if(nk_button_label(ctx, "Intermediate")){
            nbMine = 40;
            mapSizeX = 16;
            mapSizeY = 16;
            easy = 0;
            expert = 0;
            mine_newMinefield(mapSizeX, mapSizeY, nbMine);
            s->gameState = GAME_MODE_GAME;
        }
        nk_spacer(ctx);
        
        nk_spacer(ctx);
        if(nk_button_label(ctx, "Expert")){
            nbMine = 99;
            mapSizeX = 30;
            mapSizeY = 16;
            easy = 0;
            intermediate = 0;
            mine_newMinefield(mapSizeX, mapSizeY, nbMine);
            s->gameState = GAME_MODE_GAME;
        }
        nk_spacer(ctx);

        nk_layout_row_dynamic(ctx, 30, 1);
        nk_spacer(ctx);
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


int timer = 0;
int timerHold = 0;
time_t start = 0;

int ui_game_menu(struct sdlContexts* s)
{
    struct nk_context *ctx = s->g;

    static int init = 0;

    static struct images cucumber;
    static struct nk_image nkcucumber = {0};

    static struct images flanpaw;
    static struct nk_image nkflanpaw = {0};


    static int pause = 1;
    int found = 0;

    int i = 0;
    int j = 0;
    char str[10];
    nk_bool ret = nk_false;

    if(init == 0){
        init = 1;
        image_load(s, &cucumber, "./res/cucumber.png");
        nkcucumber = nk_image_ptr(cucumber.t);

        image_load(s, &flanpaw, "./res/pawflan.png");
        nkflanpaw = nk_image_ptr(flanpaw.t);

        start = time(NULL);

        timer = 0;
        timerHold = 0;
    }

    if(s->gameState == GAME_MODE_WIN){
        pause = 1;
    }

    if(nk_begin(ctx, "Game Menu", nk_rect(0, 0, s->res.w, s->res.h), NK_WINDOW_BACKGROUND | NK_WINDOW_NO_SCROLLBAR)){
        nk_layout_row_dynamic(ctx, 30, 5);

        if(nk_button_label(ctx, "Back")){
            s->gameState = GAME_MODE_MAIN;
        }

        nk_spacer(ctx);

        // update timer
        if(pause == 0){
            timer = time(NULL) - start;
        } else {
            timerHold += timer;
            timer = 0;
            start = time(NULL);
        }
        
        if(s->gameState == GAME_MODE_WIN){
            sprintf(str, "WIN: %d", timer+timerHold);
        } else {
            sprintf(str, "%d", timer+timerHold);
        }

        if(nk_button_label(ctx, str)){
            pause = !pause;
        }

        nk_spacer(ctx);

        if(nk_button_label(ctx, "Reset")){
            mine_newMinefield(mapSizeX, mapSizeY, nbMine);
            timer = 0;
            timerHold = 0;
            pause = 1;
            s->gameState = GAME_MODE_GAME;
        }

        nk_layout_row_dynamic(ctx, 30, 1);
        nk_spacer(ctx);

        if(mapSizeX == 9){
            nk_layout_row_static(ctx, 45, 45, mapSizeX);
        } else if(mapSizeX == 16){
            nk_layout_row_static(ctx, 30, 30, mapSizeX);
        } else {
            nk_layout_row_static(ctx, 30, 30, mapSizeX);
        }
        for(j = 0; j < mapSizeY; j++){
            for(i = 0; i < mapSizeX; i++){

                if(playerGrid[i][j] == 'X'){
                    sprintf(str, "X");
                } else if(playerGrid[i][j] == 'E'){
                    sprintf(str, "E");
                }  else if(playerGrid[i][j] == 'F'){
                    sprintf(str, "F");
                } else {
                    sprintf(str, "%d", playerGrid[i][j]);
                } 

                struct nk_style_button* button = malloc(sizeof(struct nk_style_button));

                memset(button, 0, sizeof(struct nk_style_button));
                button->normal          = nk_style_item_color(nk_rgba(0x58, 0x3D, 0x31, 255));
                button->hover           = nk_style_item_color(nk_rgba(0x58, 0x3D, 0x31, 255));
                button->active          = nk_style_item_color(nk_rgba(0x58, 0x3D, 0x31, 255));
                button->border_color    = nk_rgba(0xFF, 0xEE, 0xB3, 255);
                button->text_background = nk_rgba(0x58, 0x3D, 0x31, 255);
                button->text_normal     = nk_rgba(0xFF, 0xEE, 0xB3, 255);
                button->text_hover      = nk_rgba(0xFF, 0xEE, 0xB3, 255);
                button->text_active     = nk_rgba(0xFF, 0xEE, 0xB3, 255);
                button->padding         = nk_vec2(2.0f,2.0f);
                button->image_padding   = nk_vec2(0.0f,0.0f);
                button->touch_padding   = nk_vec2(0.0f, 0.0f);
                button->userdata        = nk_handle_ptr(0);
                button->text_alignment  = NK_TEXT_CENTERED;
                button->border          = 1.0f;
                button->rounding        = 4.0f;
                button->draw_begin      = 0;
                button->draw_end        = 0;


                struct nk_style_button* button2 = malloc(sizeof(struct nk_style_button));
                memset(button2, 0, sizeof(struct nk_style_button));
                button2->normal          = nk_style_item_color(nk_rgba(0xFF, 0x00, 0x00, 255));
                button2->hover           = nk_style_item_color(nk_rgba(0xFF, 0x00, 0x00, 255));
                button2->active          = nk_style_item_color(nk_rgba(0xFF, 0x00, 0x00, 255));
                button2->border_color    = nk_rgba(0xFF, 0xEE, 0xB3, 255);
                button2->text_background = nk_rgba(0x58, 0x3D, 0x31, 255);
                button2->text_normal     = nk_rgba(0xFF, 0xEE, 0xB3, 255);
                button2->text_hover      = nk_rgba(0xFF, 0xEE, 0xB3, 255);
                button2->text_active     = nk_rgba(0xFF, 0xEE, 0xB3, 255);
                button2->padding         = nk_vec2(2.0f,2.0f);
                button2->image_padding   = nk_vec2(0.0f,0.0f);
                button2->touch_padding   = nk_vec2(0.0f, 0.0f);
                button2->userdata        = nk_handle_ptr(0);
                button2->text_alignment  = NK_TEXT_CENTERED;
                button2->border          = 1.0f;
                button2->rounding        = 4.0f;
                button2->draw_begin      = 0;
                button2->draw_end        = 0;

                ret = 0;
                if(pause == 0){
                    if(playerGrid[i][j] == 'E'){
                        ret = nk_button_label(ctx, " ");
                    } else if(playerGrid[i][j] == 'X'){
                        ret = nk_button_image_styled(ctx, button2, nkcucumber);
                    } else if(playerGrid[i][j] == 'F'){
                        ret = nk_button_image(ctx, nkflanpaw);
                    } else if(playerGrid[i][j] == 0){
                        ret = nk_button_label_styled(ctx, button, "");
                    } else {
                        nk_button_label_styled(ctx, button, str);
                    }
                } else {
                    if(playerGrid[i][j] == 'E'){
                        ret = nk_button_label(ctx, " ");
                    } else if(playerGrid[i][j] == 'F'){
                        nk_button_label(ctx, " ");
                    } else {
                        nk_button_label_styled(ctx, button, " ");
                    }

                    if(ret){
                        pause = 0;
                    }
                    
                }
                if(s->gameState == GAME_MODE_WIN){
                    pause = 1;
                    ret = 0;
                }

                if(playerGrid[i][j] == 'F' && grid[i][j] == 'X' && found != -1){
                    found++;
                } else if(playerGrid[i][j] == 'F' && grid[i][j] != 'X'){
                    found = -1;
                }

                if(ret){
                    if(s->now.mouse_buttons&1){
                        if(playerGrid[i][j] == 'E'){
                            if(grid[i][j] == 0){
                                playerGrid[i][j] = grid[i][j];
                                while(mine_revealZero());
                            }
                            if(grid[i][j] == 'X'){
                                //s->gameState = GAME_MODE_GAMEOVER;
                                // reveal all map
                                for(i = 0; i < mapSizeX; i++){
                                    for(j = 0; j < mapSizeY; j++){
                                        playerGrid[i][j] = grid[i][j];
                                    }
                                }
                            }
                            playerGrid[i][j] = grid[i][j];
                        }
                    } else if(s->now.mouse_buttons&4){
                        printf("%d\r\n", grid[i][j]);
                        if(playerGrid[i][j] == 'E'){
                            playerGrid[i][j] = 'F';
                        } else if(playerGrid[i][j] == 'F'){
                            playerGrid[i][j] = 'E';
                        }
                    }
                }
            }
        }
        if(found == nbMine){
            printf("You win !\r\n");
            for(i = 0; i < mapSizeX; i++){
                for(j = 0; j < mapSizeY; j++){
                    if(grid[i][j] != 'X'){
                        playerGrid[i][j] = grid[i][j];
                    }
                }
            }
            s->gameState = GAME_MODE_WIN;
        }
    }
    nk_end(ctx);

    return 0;
}

int mine_revealZero()
{
    int i, j;
    int ret = 0;



    // reavel a case if a zero case has been revealed
    for(i = 0; i < mapSizeX; i++){
        for(j = 0; j < mapSizeY; j++){
            if(playerGrid[i][j] == 0){
                if(playerGrid[i-1][j] == 'E'){
                    playerGrid[i-1][j] = grid[i-1][j];
                    ret = 1;
                }
                if(playerGrid[i+1][j] == 'E'){
                    playerGrid[i+1][j] = grid[i+1][j];
                    ret = 1;
                }
                if(playerGrid[i][j-1] == 'E'){
                    playerGrid[i][j-1] = grid[i][j-1];
                    ret = 1;
                }
                if(playerGrid[i][j+1] == 'E'){
                    playerGrid[i][j+1] = grid[i][j+1];
                    ret = 1;
                }
                if(playerGrid[i-1][j-1] == 'E'){
                    playerGrid[i-1][j-1] = grid[i-1][j-1];
                    ret = 1;
                }
                if(playerGrid[i-1][j+1] == 'E'){
                    playerGrid[i-1][j+1] = grid[i-1][j+1];
                    ret = 1;
                }
                if(playerGrid[i+1][j-1] == 'E'){
                    playerGrid[i+1][j-1] = grid[i+1][j-1];
                    ret = 1;
                }
                if(playerGrid[i+1][j+1] == 'E'){
                    playerGrid[i+1][j+1] = grid[i+1][j+1];
                    ret = 1;
                }
            }
        }
    }








    return ret;
}

int mine_newMinefield(int x, int y, int nb)
{
    int i, j;

    srand(time(NULL));  // initialize random seed

    start = time(NULL);
    timer = 0;
    timerHold = 0;
    
    // zero minefield
    for(i = 0; i < x; i++){
        for(j = 0; j < y; j++){
            grid[i][j] = 0;
        }
    }

    // place mines as -1
    for(i = 0; i < nb; i++){
        int x, y;
        do{
            x = rand() % mapSizeX;
            y = rand() % mapSizeY;
        }while(grid[x][y] == 'X');
        grid[x][y] = 'X';
    }

    //place mines as 'X' if already a mine, retry

    // count adjacent mines
    for(i = 0; i < x; i++){
        for(j = 0; j < y; j++){
            if(grid[i][j] != 'X'){
                int count = 0;
                if(grid[i-1][j] == 'X'){
                    count++;
                } 
                if(grid[i][j-1] == 'X'){
                    count++;
                } 
                if(grid[i][j+1] == 'X'){
                    count++;
                } 
                if(grid[i+1][j] == 'X'){
                    count++;
                }

                if(grid[i-1][j-1] == 'X'){
                    count++;
                } 
                if(grid[i-1][j+1] == 'X'){
                    count++;
                }
                if(grid[i+1][j-1] == 'X'){
                    count++;
                }
                if(grid[i+1][j+1] == 'X'){
                    count++;
                }


                grid[i][j] = count;
            }
        }
    }

    // set at -2 player grid
    for(i = 0; i < 30; i++){
        for(j = 0; j < 30; j++){
            playerGrid[i][j] = 'E';
        }
    }


    return 0;
}