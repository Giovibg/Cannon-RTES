#ifndef MANAGER_H
#define MANAGER_H

#include "ptask.h"
struct mem_t {};

struct mem_t *shared_m;

/* Initialization for shared memory */
void mem_t_init(struct mem_t *mem);

/* Initialization of the game shared memory*/
void manager_init();

/* Manager for the game */
ptask manager_game();

#endif