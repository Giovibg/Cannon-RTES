#ifndef MANAGER_H
#define MANAGER_H

struct mem_t {};

/* Initialization for shared memory */
void mem_t_init(struct mem_t *mem);

/* Initialization of the game shared memory*/
void manager_init();

/* Manager for the game */
void manager_game();

#endif