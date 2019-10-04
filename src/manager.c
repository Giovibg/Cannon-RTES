#include <semaphore.h>
#include <allegro.h>
#include "graphic.h"
#include "ptask.h"
#include "manager.h"

/* Initialization for shared memory */
void mem_t_init(struct mem_t *mem)
{
    mem->score = 0;
    mem->shots = 0;
    mem->nball = 0;
    mem->nBball = 0;
    mem->nW = 0;

    sem_init(&mem->s_ball, 0, 0);
    sem_init(&mem->s_W, 0, 0);
    sem_init(&mem->mutex, 0, 1);
    
}

/* Initialization of the game shared memory */
void manager_init()
{
    mem_t_init(&shared_m);
}

/* Initialize Ball task params */
tpars init_param(void)
{
    tpars params = TASK_SPEC_DFL;
    params.period = tspec_from(PERIOD_B, MILLI);
    params.rdline = tspec_from(PERIOD_B, MILLI);
    params.priority = PRIO_B;
    params.measure_flag = 1;
    params.act_flag = NOW;
    return params;
}

/* Manager for the game */
ptask manager_game()
{  
    int k;                          // Character from keyboard
    int task_index;                 // var per debug
    int ret;                        // Return ptask index
    tpars params;                   // Params for Ball task

    // linee per debug
    task_index = ptask_get_index(); 
    printf("Task index manager: %d\n",task_index);

    /* Initialization of the game shared memory */
    manager_init();

    /* Draws game interface and screen */
    play_screen_init();

    params = init_param();
    ptask_create_param(game_play, &params);

    do
    {
        k = 0;
        if(keypressed())
        {
            k = readkey() >> 8;
            if(k == KEY_ENTER) // Ball shoted
            {
                /* Create a new Ball task
                params = init_param();
                ret = ptask_create_param(manager_game, &params);
                */

                sem_wait(&shared_m.mutex);
                if (shared_m.nball > 0 || shared_m.nW > 0)
                {
                    // If someone is reading, or is writing, i'll block
                    shared_m.nBw++;
                }
                else
                {
                    // If nobody is writing or reading, i can go
                    shared_m.nW++;
                    sem_post(&shared_m.s_W);
                }
                
                sem_post(&shared_m.mutex);
                sem_wait(&shared_m.s_W);

                // Write the buffer!
                shared_m.shots +=1;

                sem_wait(&shared_m.mutex);
                shared_m.nW--;
                if (shared_m.nBball > 0)
                {
                    // If there are some blocked ball task, i'll unlock them all
                    while(shared_m.nBball > 0)
                    {
                        shared_m.nBball--;
                        shared_m.nball++;
                        sem_post(&shared_m.s_ball);
                    }
                }
                else if (shared_m.nBw > 0)
                {
                    // I'll unlcok Writer
                    shared_m.nBw--;
                    shared_m.nW++;
                    sem_post(&shared_m.s_W);
                }
                sem_post(&shared_m.mutex);
            }
            ptask_wait_for_period();
        }
    } while (k != KEY_ESC);
}