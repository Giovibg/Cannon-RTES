#ifndef MANAGER_H
#define MANAGER_H

#include "ptask.h"

//-------------------------------------------------------------
// GLOBAL CONSTANTS
//-------------------------------------------------------------
#define PERIOD_B 20
// Period manager task
#define PRIO_B 8
// Priority manager task

//-------------------------------------------------------------
// STRUCTURE DEFINITIONS
//-------------------------------------------------------------
// Shared memory structure definitios
struct mem_t{

    int score;              // Score of the match; the number of time in which the target has been hit
    int shots;              // Number of bullets that has been fired
    
    int nball;              // Number of Reader task
    int nBball;             // Number of Blocked ball task
    int nW;                 // Number of writing process
    int nBw;                // Number of Blocked Writing process#define PERIOD_M 30
    int end;                // End all task
    sem_t s_Read, s_Write;      // Semaphor for Reader task and Writers task
    sem_t mutex;
    
};
// Shared memory structure
struct mem_t shared_m;

//-------------------------------------------------------------
// FUNCTIONS
//-------------------------------------------------------------

/* Initialize Ball task params */
tpars init_param(void);

/* Initialization for shared memory */
void mem_t_init(struct mem_t *mem);

/* Initialization of the game shared memory*/
void manager_init();

/* Manager for the game */
ptask manager_game();

/* First phase of writers protection */
void control_writer();

/* Release phase of writers protection */
void release_writer();

/* First phase of readers protection */
void control_reader();

/* Release phase of readers protection */
void release_reader();

#endif