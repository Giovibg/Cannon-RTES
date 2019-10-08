#include <semaphore.h>
#include <allegro.h>

#include "ptask.h"
#include "graphic.h"
#include "manager.h"
#include "shot.h"

// Params for Shot tasks
static tpars shot_params;

/* Initialization for shared memory */
void mem_t_init(struct mem_t *mem)
{
    int i;

    mem->nball = 0;
    mem->nBball = 0;
    mem->nW = 0;

    mem->score = 0;
    mem->shots = 0;

    for(i = 0; i < MAX_SHOTS; i++)
    {
        mem->pos[i].x = -1;
        mem->pos[i].y = -1;
    }
    
    sem_init(&mem->s_Read, 0, 0);
    sem_init(&mem->s_Write, 0, 0);
    sem_init(&mem->mutex, 0, 1);
}

/* First phase of writer manager protection */
void control_writer()
{
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
        sem_post(&shared_m.s_Write);
    }
                
    sem_post(&shared_m.mutex);
    sem_wait(&shared_m.s_Write);
}


/* Release phase of writer manager protection */
void release_writer()
{
    sem_wait(&shared_m.mutex);
    shared_m.nW--;
    if (shared_m.nBball > 0)
    {
        // If there are some blocked ball task, i'll unlock them all
        while(shared_m.nBball > 0)
        {
            shared_m.nBball--;
            shared_m.nball++;
            sem_post(&shared_m.s_Read);
        }
    }
    else if (shared_m.nBw > 0)
    {
        // I'll unlcok Writer
        shared_m.nBw--;
        shared_m.nW++;
        sem_post(&shared_m.s_Write);
    }
    sem_post(&shared_m.mutex);
}

/* First phase of readers protection */
void control_reader()
{
    sem_wait(&shared_m.mutex);
    if (shared_m.nW > 0|| shared_m.nBw > 0)
    {
        // If there is an Active writer or a Pending writer, i'll block
        shared_m.nBball++;
    }
    else
    {
        shared_m.nball++;
        sem_post(&shared_m.s_Read);
    }
    sem_post(&shared_m.mutex);
    sem_wait(&shared_m.s_Read);
}

/* Release phase of readers protection */
void release_reader()
{
    sem_wait(&shared_m.mutex);
    shared_m.nball--;
    if(shared_m.nBw > 0 && shared_m.nball == 0)
    {
        // If there are some blocked Writers and no reader
        shared_m.nBw--;
        shared_m.nW++;
        sem_post(&shared_m.s_Write);
    }
    sem_post(&shared_m.mutex);
}

/* Initialize task params */
tpars init_param(int PRIO, int PERIOD)
{
    tpars params = TASK_SPEC_DFL;
    params.period = tspec_from(PERIOD, MILLI);
    params.rdline = tspec_from(PERIOD, MILLI);
    params.priority = PRIO;
    params.measure_flag = 1;
    params.act_flag = NOW;
    params.arg = NULL;
    return params;
}

/* Initialization of the game shared memory */
void manager_init()
{
    mem_t_init(&shared_m);
}

/* Create a new Shot task*/
void shot_create()
{
    int ret;

    /* Create Shots params*/
    shot_params = init_param(PRIO_B, PERIOD_B);
    ret = ptask_create_param(shot, &shot_params);

    printf("Ho creato pallina con ret: %d\n", ret);
}

/* Manager for the game */
void manager_game()
{  
    int task_index;                 // var per debug
    tpars params;                   // Params for Graphic task

    // linee per debug
    task_index = ptask_get_index(); 
    printf("Task index manager: %d\n",task_index);

    /* Initialization of the game shared memory */
    manager_init();
   
    /* Draws game interface and screen */
    play_screen_init();

    /* Create graphic task */
    params = init_param(PRIO_G, PERIOD_G);
    ptask_create_param(game_play, &params);
}