#include <allegro.h>
#include <stdio.h>
#include "graphic.h"

static int score = 0;
static int shot = 0;

/* Change rate value */
void set_rate(int new_shot)
{
    shot = new_shot;

    char s[MSG_L];
    
    sprintf(s, "Rate: %d/%d", score, shot);
    textout_ex(screen, font, s, XWIN - PAD - 30, PAD/2, 15, 0);
}

/* Change score value */
void set_score(int new_score)
{
    score = new_score;

    char s[MSG_L];
    
    sprintf(s, "Score: %d", score);
    textout_ex(screen, font, s, PAD, PAD/2, 15, 0);
}

/* Draws game interface and screen */
void screen_init()
{
    // Drawing playground borders
    line(screen, PAD, YWIN - PAD, PAD, PAD, 15);
    line(screen, XWIN - PAD, YWIN - PAD, XWIN - PAD, PAD, 15);
    line(screen, PAD, YWIN - PAD, XWIN - PAD, YWIN - PAD, 15);
    line(screen, XWIN - PAD, PAD, PAD, PAD, 15);

    // tmp var to for writing message
    char s[MSG_L];

    /* Score statistic info */
    // Score
    sprintf(s, "Score: %d", score);
    textout_ex(screen, font, s, PAD, PAD/2, 15, 0);
    
    // Game title
    textout_ex(screen, font, "CANNON BALL!", XWIN/2 - 45, PAD/2, 15, 0);

    // Rate hit/shot
    sprintf(s, "Rate: %d/%d", score, shot);
    textout_ex(screen, font, s, XWIN - PAD - 30, PAD/2, 15, 0);
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
    screen_init();
}