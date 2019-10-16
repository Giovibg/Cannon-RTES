#ifndef GRAPHIC_H
#define GRAPHIC_H

#include "ptask.h"
#include "manager.h"

//-------------------------------------------------------------
// GLOBAL CONSTANTS
//-------------------------------------------------------------

// background color
#define BKG 0

#define WHITE 15
// string lenght
#define MSG_L 50  

#define RED 12

//-------------------------------------------------------------

/* Change rate and score value */
void change_rate_score(int new_shots, int new_score);

// Retrieve necessary data for updating graphics 
// from Shared Memory. Protected!
void retrieve_sharedm(int *shots, int*score, int *end_charge, int *shot_pwr, int *cannon_degree, int *target_x, int *update_traj);

/* Task that update Game_Screen during play */
ptask game_play();

/* Draws game interface and screen */
void play_screen_init();

/* Reset postrail_t trail */
void reset_trail();

/* Print trajectory preview */
void update_trajectory(int color);

/* Retrieve new trajectory */
void retrieve_trajectory();

/* Draws menu interface */
void menu_screen_init();

/* Draws cannon power bar's line */
void draw_Pwrline();

/* Draw playground borders */
void draw_Borders();

/* Draw Wall */
void draw_wall(int tagret_x);

/* Draw playground borders */
void draw_PwrBar();

/* Draw a new circlefill rapresenting a shot */
void draw_Shots(struct pos_t posi, int color);

/* Initialize display and keyboard interactions */
void display_init();

/* Initialize graphic environment */
void gui_init();

/* Import Bitmaps */
void import_bitmap();

#endif