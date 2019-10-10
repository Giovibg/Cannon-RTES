#ifndef GRAPHIC_H
#define GRAPHIC_H

#include "ptask.h"

//-------------------------------------------------------------
// GLOBAL CONSTANTS
//-------------------------------------------------------------
#define BKG 0
// background color
#define WHITE 15
// background color
#define MSG_L 50  
// string lenght
#define RED 12
//-------------------------------------------------------------

/* Change rate and score value */
void change_rate_score(int new_shots, int new_score);

/* Task that update Game_Screen during play */
ptask game_play();

/* Draws game interface and screen */
void play_screen_init();

/* Draws menu interface */
void menu_screen_init();

/* Initialize display and keyboard interactions */
void display_init();

/* Initialize graphic environment */
void gui_init();

/* Import Bitmaps */
void import_bitmap();

/* Print trajectory preview */
void update_trajectory(int color);


#endif