#ifndef GRAPHIC_H
#define GRAPHIC_H

#include "ptask.h"
#include "manager.h"

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

// Reset postrail_t trail
void reset_trail();

/* Print trajectory preview */
void update_trajectory(int color);

// Retrieve new trajectory
void retrieve_trajectory();

/* Draws menu interface */
void menu_screen_init();

// Draws cannon power bar's line 
void draw_Pwrline();

// Draw playground borders
void draw_Borders();

/* Draw playground borders */
void draw_PwrBar();

// Draw a new circlefill rapresenting a shot
void draw_Shots(struct pos_t posi, int color);

/* Initialize display and keyboard interactions */
void display_init();

/* Initialize graphic environment */
void gui_init();

/* Import Bitmaps */
void import_bitmap();

#endif