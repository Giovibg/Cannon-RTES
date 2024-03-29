#ifndef MANAGER_H
#define MANAGER_H

#include "ptask.h"

//-------------------------------------------------------------
// GLOBAL CONSTANTS
//-------------------------------------------------------------

// PIGRECO
#define PIGRECO 3.1415926535
// Window x resolution
#define XWIN 1060
// Window y resolution
#define YWIN 680
// Max number of position of the trajectory
#define SEMICFR 1500
// Padding for game's statistics
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
#define PRIO_G 9
// Period Shot task
#define PERIOD_B 20
// Priority Shot task
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
#define WALL_W 15
// Offset target respect wall
#define OFFSET_WALL 40

//-------------------------------------------------------------
// STRUCTURE DEFINITIONS
//-------------------------------------------------------------

// Structure that represent a 2D point in space
struct pos_t{
    int x;                          // X Position
    int y;                          // Y Position
};

// Structure X, Y point trajectory
struct postrail_t{

    int x[SEMICFR];                 // All X trajectory points   
    int y[SEMICFR];                 // All Y trajectory points
};

// Shared memory structure definitions
struct mem_t{

    int score;                      // Score of the match;
    int shots;                      // Number of bullets that has been fired
    int shot_pwr;                   // Power of the shot
    int end_charge;                 // Check if end cannon charge process
    int cannon_degree;              // Cannon rotation
    int update_traj;                // Check if trajectory has to be updated
    int graphic_d;                  // Graphic deadline miss
    int target_d;                   // Target deadline
    int ball_d;                     // Ball deadline miss
    int power_d;                    // PowerBar deadline miss
    int end;                        // Exit. Stop tasks
    int pid[MAX_SHOTS + 3];         // Vector with tasks's PID

    struct pos_t pos[MAX_SHOTS];    // Positions of all the Shots
    struct pos_t pos_target;        // Target's position
    struct pos_t pos_wall;          // Wall's position 
    struct postrail_t trajectory;   // Trajectory points
    
    
    int nR;                         // Total number of Reader task
    int nBR;                        // Total number of Blocked ball task
    int nW;                         // Total number of writing process
    int nBw;                        // Total number of Blocked Writing process

    sem_t s_Read, s_Write;          // Semaphore for Reader and Writers tasks
    sem_t mutex;
};

/* Shared memory structure */
struct mem_t shared_m;

//-------------------------------------------------------------
// FUNCTIONS
//-------------------------------------------------------------

/*
 * Initialization for shared memory 
 */
void init_mem_t(struct mem_t *mem);

/* 
 * Check end to terminate task 
 */
int check_End();

/* 
 * First phase of writers protection 
 */
void control_Writer();

/* 
 * Release phase of writers protection 
 */ 
void release_Writer();

/* 
 * First phase of readers protection 
 */
void control_Reader();

/* 
 * Release phase of readers protection 
 */
void release_Reader();

/* 
 * Initialize task params 
 * int prio     -> priority of the task
 * int period   -> period of the task
 */
tpars init_Param(int prio, int period);

/* 
 * Create a new Shot task 
 */
int create_Shot(int n_shot);

/* 
 * Reset shared Traij 
 */
void reset_SharedTraij();

/* 
 * Manager for the game 
 */
int manager_game();

/* 
 * Task trajectory calculation
 * float speedx -> float that represent speed on x axis 
 * float speedy -> float that represent speed on y axis
 */ 
void trajectory_cannon(float speedx, float speedy);

/* 
 * Ptask for the charge cannon power bar
 */
ptask charge_cannon();

#endif