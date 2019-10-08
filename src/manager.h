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
#define PERIOD_G 20
// Period Graphic task
#define PRIO_G 8
// Priority Graphic task
#define PERIOD_B 20
// Period Shot task
#define PRIO_B 9
// Priority Shot task
#define MAX_SHOTS 90
// Number of max possible shots

//-------------------------------------------------------------
// STRUCTURE DEFINITIONS
//-------------------------------------------------------------
// Structure to keep track of Shots's position
struct pos_t{
    int x;                          // X Shot's Position
    int y;                          // Y Shot's Position
};

// Shared memory structure definitios
struct mem_t{

    int score;                      // Score of the match; the number of time in which the target has been hit
    int shots;                      // Number of bullets that has been fired
    struct pos_t pos[MAX_SHOTS];    // Positions of all the Shots
    
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
/* Initialization for shared memory */
void mem_t_init(struct mem_t *mem);

/* First phase of writers protection */
void control_writer();

/* Release phase of writers protection */
void release_writer();

/* First phase of readers protection */
void control_reader();

/* Release phase of readers protection */
void release_reader();

/* Initialize Ball task params */
tpars init_param(int PRIO, int PERIOD);

/* Initialization of the game shared memory*/
void manager_init();

/* Create a new Shot task*/
void shot_create();

/* Manager for the game */
void manager_game();

#endif