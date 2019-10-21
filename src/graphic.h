#ifndef GRAPHIC_H
#define GRAPHIC_H

#include "ptask.h"
#include "manager.h"

//-------------------------------------------------------------
// GLOBAL CONSTANTS
//-------------------------------------------------------------

#define BKG 0       // background color
#define RED 12
#define WHITE 15
#define GREEN 10
#define MSG_L 70    // string lenght  

//-------------------------------------------------------------
// FUNCTIONS
//-------------------------------------------------------------

/* Change rate and score value */
void change_rate_score(int new_shots, int new_score);

/* Update deadline miss */
void update_Deadline();

/* Import Bitmaps */
void import_Bitmap();

/* 
 * Draw the colored blocks that indicate the power of the shot 
 * int shot_pwr -> Power of the shot
 * int j_init   -> Indicate at which point start the cycle
 */
void charge_phase(int shot_pwr, int j_init);

/* 
 * Draws cannon power bar's line 
 * int shot_pwr     -> Power of the shot
 * int end_charge   -> Bool to check if charge phase should end
 */
void draw_Pwrline(int shot_pwr, int end_charge);

/* 
 * Draw playground borders 
 */
void draw_PwrBar();

/* Change Target bitmap */
void change_Target(int x, int y);

/* Rotate cannon bitmap */
void change_Cannon(int cannon_degree);

/* Change rate and score value */
void change_RateScore(int new_shots, int new_score);

/* 
 * Draw a new circlefill representing a shot 
 * struct pos_t pos    -> struct that represent X and Y Shot coordinates
 * int color           -> color of the Shot
 */
void draw_Shots(struct pos_t pos, int color);

/* Draw playground borders */
void draw_Borders();

/* Draw Wall */
void draw_Wall(int tagret_x);

/* Retrieve new trajectory */
void retrieve_Trajectory();

/* Print trajectory preview */
void update_Trajectory(int color);

/* Draws game interface and screen */
void init_PlayScreen();

/* Draws menu interface */
void init_MenuScreen();

/* Initialize display and keyboard interactions */
void init_Display();

/* Initialize graphic environment */
void init_Gui();

/* 
 * Retrieve necessary data for updating graphics 
 * from Shared Memory. Protected!
 */
void retrieve_Sharedm(int *shots, int*score, int *end_charge, int *shot_pwr, 
                        int *cannon_degree, int *target_x, int *update_traj);

/* Draws all the Shots with the new positions*/
void update_Shots();

/* Check Deadline miss */
void check_DeadlineMiss();

/* All Dynamic update in graphic task */
void change_Dynamic(int shots, int score, int cannon_degree, int target_x, 
                        int shot_pwr, int end_charge);

/* Task that update Game_Screen during play */
ptask game_play();

#endif