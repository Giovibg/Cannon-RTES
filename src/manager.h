#ifndef MANAGER_H
#define MANAGER_H

#include "ptask.h"

//-------------------------------------------------------------
// GLOBAL CONSTANTS
//-------------------------------------------------------------
#define XWIN 1060
// window x resolution
#define YWIN 680
// window y resolution
#define PAD 60
// padding for game's statistics
#define RADIUS 5
// Shot ball radius
#define OFFSET 10
// Offset bitmap target
#define NO_POS -1
// Alias no position ball
#define PERIOD_G 20            // BISOGNA METTERE UN VALORE OTTIMO NON A CASO  
// Period Graphic task
#define PRIO_G 8
// Priority Graphic task
#define PERIOD_B 20
// Period Shot task
#define PRIO_B 9
// Priority Graphic task
#define PERIOD_T 500
// Period Target task
#define PRIO_T 9
// Period manager task
#define PERIOD_M 400
// Priority manager task
#define PRIO_M 10
// Priority Target task
#define MAX_SHOTS 90
// Number of max possible shots
#define MAX_PWR 10
// Max power of the shot
#define G 9.8
// Gravity acceleration
#define CANNON_X XWIN - PAD - 9*OFFSET
// Cannon X position
#define CANNON_Y YWIN - PAD - 10*OFFSET
// Cannon X position
#define TARGET_X XWIN - PAD - 24*OFFSET
// Target X start position
#define TARGET_Y YWIN - PAD - 10*OFFSET
// Target Y start position
#define MAX_DEG 25
// Max degree of the cannon
#define MIN_DEG 0
// Max degree of the cannon
#define MAX_TARGET_R TARGET_X + 60
// Max right position of the target
#define MAX_TARGET_L TARGET_X - 60
// Max left position of the target

//-------------------------------------------------------------
// STRUCTURE DEFINITIONS
//-------------------------------------------------------------
// Structure to keep track of Shots's position
struct pos_t{
    int x;                          // X Shot's Position
    int y;                          // Y Shot's Position
};

struct postrail_t{
    int x[XWIN];
    int y[YWIN];
};

// Shared memory structure definitios
struct mem_t{

    int score;                      // Score of the match; the number of time in which the target has been hit
    int shots;                      // Number of bullets that has been fired

    int shot_pwr;                   // Power of the shot
    int end_charge;                 // Var to segnalate the end of the cannon charge process
    int cannon_degree;              // Cannon rotation
    int update_traj;
    struct pos_t pos[MAX_SHOTS];    // Positions of all the Shots
    struct pos_t pos_target;        // Position of target

    struct postrail_t trajectory;  // Trajectory points
    
    
    int nball;                      // Number of Reader task
    int nBball;                     // Number of Blocked ball task
    int nW;                         // Number of writing process
    int nBw;                        // Number of Blocked Writing process#define PERIOD_M 30

    sem_t s_Read, s_Write;          // Semaphor for Reader task and Writers task
    sem_t mutex;
    
};

// Shared memory structure
struct mem_t shared_m;

//-------------------------------------------------------------
// FUNCTIONS
//-------------------------------------------------------------
// Initialization for shared memory 
void mem_t_init(struct mem_t *mem);

// First phase of writers protection 
void control_writer();

// Release phase of writers protection 
void release_writer();

// First phase of readers protection 
void control_reader();

// Release phase of readers protection 
void release_reader();

// Initialize task params 
tpars init_param(int PRIO, int PERIOD);

// Create a new Shot task
int shot_create();

// Reset shared Trail
void reset_shared_trail();

// Task trajectory calculation 
void trajectory_cannon(float speedx, float speedy);

// Manager for the game
int manager_game();

// Charge cannon 
ptask charge_cannon();

#endif