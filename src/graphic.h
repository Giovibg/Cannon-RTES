#ifndef GRAPHIC_H
#define GRAPHIC_H

//-------------------------------------------------------------
// GLOBAL CONSTANTS
//-------------------------------------------------------------
#define XWIN 1060
// window x resolution
#define YWIN 680
// window y resolution
#define PAD 60
// padding for game's statistics
#define BKG 0
// background color
#define MSG_L 50
// string lenght
//-------------------------------------------------------------

/* Change rate and score value */
void change_rate_score(int new_shots, int new_score);

/* Draws game interface and screen */
void play_screen_init();

/* Draws menu interface */
void menu_screen_init();

/* Initialize display and keyboard interactions */
void display_init();

/* Initialize graphic environment */
void gui_init();

#endif