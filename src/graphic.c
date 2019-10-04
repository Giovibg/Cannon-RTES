#include <allegro.h>
#include <stdio.h>
#include "ptask.h"
#include "graphic.h"
#include "manager.h"


static int score = 0;
static int shots = 0;
static int end_graphic;

/* Change rate and score value */
void change_rate_score(int new_shots, int new_score)
{
    shots = new_shots;
    score = new_score;

    // tmp var for strings
    char s[MSG_L];
    char r[MSG_L];
    
    // Update rate graphic
    sprintf(r, "Rate: %d/%d", score, shots);
    textout_ex(screen, font, r, XWIN - PAD - 70, PAD/2, 15, 0);

    // Update score graphic
    sprintf(s, "Score: %d", score);
    textout_ex(screen, font, s, PAD, PAD/2, 15, 0);
}



/* Task that update Game_Screen during play */
ptask game_play()
{
    end_graphic = 0;
    while(end_graphic != 1)
    {  
        
        control_reader();
        // Update global static variable of shots and score. Protected!
        shots = shared_m.shots;
        score = shared_m.score;
        end_graphic = shared_m.end;
        release_reader();

        change_rate_score(shots, score);

        ptask_wait_for_period();
    }
}

/* Draws game interface and screen */
void play_screen_init()
{
    clear_to_color(screen, BKG);

    // Drawing playground borders
    line(screen, PAD, YWIN - PAD, PAD, PAD, 15);
    line(screen, XWIN - PAD, YWIN - PAD, XWIN - PAD, PAD, 15);
    line(screen, PAD, YWIN - PAD, XWIN - PAD, YWIN - PAD, 15);
    line(screen, XWIN - PAD, PAD, PAD, PAD, 15);

    /* Score statistic info */
    // Score and Rate hit/shot
    change_rate_score(0, 0);
    
    // Game title
    textout_ex(screen, font, "CANNON BALL!", XWIN/2 - 45, PAD/2, 15, 0);
}

/* Draws menu interface */
void menu_screen_init()
{
    clear_to_color(screen, BKG);

     // Drawing playground borders
    line(screen, PAD, YWIN - PAD, PAD, PAD, 15);
    line(screen, XWIN - PAD, YWIN - PAD, XWIN - PAD, PAD, 15);
    line(screen, PAD, YWIN - PAD, XWIN - PAD, YWIN - PAD, 15);
    line(screen, XWIN - PAD, PAD, PAD, PAD, 15);

    // tmp var to for writing message
    char s[MSG_L];

    /* Title and Instructions */
    // Game title
    sprintf(s, "CANNON BALL!");
    textout_ex(screen, font, s, XWIN/2 - 45, YWIN/2, 15, 0);

    sprintf(s, "press space");
    textout_ex(screen, font, s, XWIN/2 - 45, YWIN/2 + 45, 15, 0);
}

/* Initialize display and keyboard interactions */
void display_init()
{
    allegro_init();

    set_color_depth(8);

    set_gfx_mode(GFX_AUTODETECT_WINDOWED, XWIN, YWIN, 0, 0);
    clear_to_color(screen, BKG);
    
    install_keyboard();
}

/* Initialize graphic environment */
void gui_init()
{
    // Initialize display and keyboard interactions
    display_init();

    // Draws game interface and screen
    menu_screen_init();
}