#include <semaphore.h>
#include <allegro.h>

#include "ptask.h"
#include "graphic.h"
#include "manager.h"
#include "shot.h"
#include "target.h"

// Params for Shot tasks
static tpars shot_params;

/* Initialization for shared memory */
void mem_t_init(struct mem_t *mem)
{
    int i;
    mem->score = 0;
    mem->shots = 0;

    mem->shot_pwr = 1;
    mem->end_charge = -1;
    mem->cannon_degree = -1;
    mem->update_traj = 0;

    mem->pos_target.x = TARGET_X;
    mem->pos_target.y = TARGET_Y;

    mem->nball = 0;
    mem->nBball = 0;
    mem->nW = 0;
    mem->nBw = 0;

    for(i = 0; i < MAX_SHOTS; i++)
    {
        mem->pos[i].x = NO_POS;
        mem->pos[i].y = NO_POS;
    }
    
    for(int i = 0; i < XWIN; i++)
    {
        mem->trajectory.x[i] = NO_POS; 
    }
    for(int j=0; j < YWIN; j++)
    {
        mem->trajectory.y[j] = NO_POS;
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

/* Create a new Shot task*/
int shot_create()
{
    int ret;

    /* Create Shots params*/
    shot_params = init_param(PRIO_B, PERIOD_B);
    ret = ptask_create_param(shot, &shot_params);

    printf("Ho creato pallina con ret: %d\n", ret);

    return 1;
}

/* Reset shared Trail */
void reset_shared_trail()
{
    int i = 0;
    for(i = 0; i <= XWIN; i++)
    {
        shared_m.trajectory.x[i] = NO_POS;
    }
    i = 0;
    for(i = 0; i <= YWIN; i++)
    {
        shared_m.trajectory.y[i] = NO_POS;
    }
}

/* Task trajectory calculation */
void trajectory_cannon(float speedx, float speedy)
{
    float old_x, old_y;
    float x, y;
    int i = 0;
    old_x = x =  PAD + 80 + 5*OFFSET;
    old_y = y = PAD + 5*OFFSET;
    float dt = PERIOD_G * 0.0042; // TScale based on graphic period
    
    control_writer();
    reset_shared_trail();
    release_writer();

    while((x <= XWIN) && (YWIN - y < YWIN - PAD))
    {
        old_x = x;
        old_y = y;
        x = old_x + (speedx * dt);
        y = old_y + (speedy * dt) - (0.5 * G * dt * dt);
        speedy =  speedy - (G * dt);
        // printf("X: %f\n Y: %f\n",x,y);
        control_writer();
        shared_m.trajectory.x[i] = (int) x;
        shared_m.trajectory.y[i] = (int) (YWIN - y);
        release_writer();
        i += 1;
        // printf("valore i:%d\n",i);
    }
}

/* Manager for the game */
int manager_game()
{  
    int task_index;                 // var per debug
    tpars params;                   // Params for Graphic task

    // linee per debug
    task_index = ptask_get_index(); 
    printf("Task index manager: %d\n",task_index);

    /* Initialization of the game shared memory */
    mem_t_init(&shared_m);
   
    /* Draws game interface and screen */
    //play_screen_init();

    /* Create graphic task */
    params = init_param(PRIO_G, PERIOD_G);
    ptask_create_param(game_play, &params);

    /* Create Target task */
    params = init_param(PRIO_T, PERIOD_T);
    ptask_create_param(target, &params);

    return 1;
}

/* Charge cannon */
ptask charge_cannon()
{
    int up = 1;                 // Var that says if the pwr should grow or decrease
    int shot_pwr = 0;
    int end_charge = 0;
    int pwr;

    do
    {
        if(up)
        {
            shot_pwr += 1;
            control_writer();
            shared_m.shot_pwr = shot_pwr;
            release_writer();
            if (shot_pwr == MAX_PWR)
            {
                up = 0;
            }
        }
        else
        {
            shot_pwr -= 1;
            control_writer();
            shared_m.shot_pwr = shot_pwr;
            release_writer();
            if (shot_pwr == 0)
            {
                up = 1;
            }
        }  

        control_reader();
        end_charge = shared_m.end_charge;
        pwr = shared_m.shot_pwr;
        release_reader();

        ptask_wait_for_period();

    } while(end_charge != -1);

    return;
}

