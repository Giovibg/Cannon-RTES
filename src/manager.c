#include <semaphore.h>
#include <allegro.h>
#include "graphic.h"
#include "ptask.h"
#include "main.h"
struct mem_t{

    int score;              // The score of the match; the number of time in which the target has been hit
    int shots;              // The number of bullets that has been fired
    sem_t s_ball, s_W;
    sem_t mutex;
    
};

// Shared memory structure
    struct mem_t *shared_m;

/* Initialization for shared memory */
void mem_t_init(struct mem_t *mem)
{
    mem->score = 0;
    mem->shots = 0;

    sem_init(&mem->s_ball, 0, 0);
    sem_init(&mem->s_W, 0, 0);
    sem_init(&mem->mutex, 0, 1);

}

/* Initialization of the game shared memory */
void manager_init()
{
    
    mem_t_init(&shared_m);
    
}

/* Manager for the game */
ptask manager_game()
{  
    int k;                          // Character from keyboard
    int task_index;

    task_index = ptask_get_index(); 
    manager_init();
    play_screen_init();

    do
    {
        k = 0;

        if(keypressed())
        {
            k = readkey() >> 8;
            if(k == KEY_ENTER) //Ball shoted
            {
                sem_wait(&shared_m->mutex);
                shared_m->score += 1;
                sem_post(&shared_m->mutex);
            }
        }
        ptask_wait_for_period();
    } while (1);
    
   
    
}