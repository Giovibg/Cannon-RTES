#ifndef MANAGER_H
#define MANAGER_H

#include "ptask.h"
#include "math.h"

//-------------------------------------------------------------
// GLOBAL CONSTANTS
//-------------------------------------------------------------

// PIGRECO
#define PIGRECO 3.1415926535
// window x resolution
#define XWIN 1060
// window y resolution
#define YWIN 680
// diag
#define SEMICFR ((XWIN - 2 * PAD) * 4) / 2
// padding for game's statistics
#define PAD 60
// Shot ball radius
#define RADIUS 5
// Offset bitmap target
#define OFFSET 10
// Alias no position ball
#define NO_POS -1
// Period Graphic task, screen update rate
#define PERIOD_G 25              
// Priority Graphic task
#define PRIO_G 8
// Period Shot task
#define PERIOD_B 20
// Priority Graphic task
#define PRIO_B 9
// Period Target task
#define PERIOD_T 20
// Priority Target task
#define PRIO_T 9
// Period Power Bar task
#define PERIOD_M 150
// Priority Power Bar task
#define PRIO_M 10
// Number of max possible shots
#define MAX_SHOTS 90
// Max power of the shot
#define MAX_PWR 10
// Gravity acceleration
#define G 9.8    
// Cannon X position
#define CANNON_X XWIN - PAD - 9*OFFSET
// Cannon Y position
#define CANNON_Y YWIN - PAD - 10*OFFSET
// Cannon Max degree
#define MAX_DEG 35
// Cannon Min degree
#define MIN_DEG -5
// Target X start position
#define TARGET_X XWIN - PAD - 24*OFFSET
// Target Y start position
#define TARGET_Y YWIN - PAD - 10*OFFSET
// Max right position of the target
#define MAX_TARGET_R TARGET_X + 130
// Max left position of the target
#define MAX_TARGET_L TARGET_X - 60
// Wall width
#define WALL_W 8
// Offset target respect wall
#define OFFSET_WALL 40

//-------------------------------------------------------------
// STRUCTURE DEFINITIONS
//-------------------------------------------------------------
// Structure to keep track of Shots's position
struct pos_t{
    int x;                          // X Shot's Position
    int y;                          // Y Shot's Position
};

struct postrail_t{

    int x[SEMICFR];
    int y[SEMICFR];
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
    struct pos_t pos_wall;          // Wall position 
    struct postrail_t trajectory;   // Trajectory points
    
    
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
tpars init_param(int prio, int period);

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