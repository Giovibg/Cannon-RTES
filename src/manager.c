#include <semaphore.h>
#include <allegro.h>

#include "ptask.h"
#include "graphic.h"
#include "manager.h"
#include "shot.h"
#include "target.h"

/* Initialization for shared memory */
void init_mem_t(struct mem_t *mem)
{
    int i;
    mem->score = 0;
    mem->shots = 0;

    mem->shot_pwr = 1;
    mem->end_charge = -1;
    mem->cannon_degree = -1;
    mem->update_traj = 0;
    mem->end = 0;
    mem->pos_target.x = TARGET_X;
    mem->pos_target.y = TARGET_Y;
    mem->pos_wall.x = (XWIN / 2) - WALL_W/2;
    mem->pos_wall.y = (3*YWIN - 2 * PAD) / 4;

    mem->nR = 0;
    mem->nBR = 0;
    mem->nW = 0;
    mem->nBw = 0;
    
    mem->graphic_d = mem->power_d = mem ->ball_d = mem-> target_d = 0;
    for (i = 0; i < MAX_SHOTS; i++)
    {
        mem->pos[i].x = NO_POS;
        mem->pos[i].y = NO_POS;
    }
    for (int i = 0; i < SEMICFR; i++)
    {
        mem->trajectory.x[i] = NO_POS; 
        mem->trajectory.y[i] = NO_POS;
    }
    for(i = 0; i < MAX_SHOTS + 3; i++)
    {
        mem->pid[i] = -1;
    }
    sem_init(&mem->s_Read, 0, 0);
    sem_init(&mem->s_Write, 0, 0);
    sem_init(&mem->mutex, 0, 1);
}

int check_End()
{
	int	end;
	control_Reader();
	end = shared_m.end;
	release_Reader();
	return end;
}

/* First phase of writer manager protection */
void control_Writer()
{
    sem_wait(&shared_m.mutex);
    if (shared_m.nR > 0 || shared_m.nW > 0)
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
void release_Writer()
{
    sem_wait(&shared_m.mutex);
    shared_m.nW--;
    if (shared_m.nBR > 0)
    {
        // If there are some blocked ball task, i'll unlock them all
        while (shared_m.nBR > 0)
        {
            shared_m.nBR--;
            shared_m.nR++;
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
void control_Reader()
{
    sem_wait(&shared_m.mutex);
    if (shared_m.nW > 0|| shared_m.nBw > 0)
    {
        // If there is an Active writer or a Pending writer, i'll block
        shared_m.nBR++;
    }
    else
    {
        shared_m.nR++;
        sem_post(&shared_m.s_Read);
    }
    sem_post(&shared_m.mutex);
    sem_wait(&shared_m.s_Read);
}

/* Release phase of readers protection */
void release_Reader()
{
    sem_wait(&shared_m.mutex);
    shared_m.nR--;
    if (shared_m.nBw > 0 && shared_m.nR == 0)
    {
        // If there are some blocked Writers and no reader
        shared_m.nBw--;
        shared_m.nW++;
        sem_post(&shared_m.s_Write);
    }
    sem_post(&shared_m.mutex);
}

/* Initialize task params */
tpars init_Param(int prio, int period)
{
    tpars   params = TASK_SPEC_DFL;

    params.period = tspec_from(period, MILLI);
    params.rdline = tspec_from(period, MILLI);
    params.priority = prio;
    params.measure_flag = 1;
    params.act_flag = NOW;
    params.arg = NULL;
    return params;
}

/* Create a new Shot task*/
int create_Shot(int n_shots)
{
    int pid_s;
    static tpars    shot_params;   	// Params for Shot tasks

    /* Create Shots params*/
    shot_params = init_Param(PRIO_B, PERIOD_B);
    pid_s = ptask_create_param(shot, &shot_params);

    control_Writer();
    shared_m.pid[n_shots + 2] = pid_s;
    release_Writer();

    return 1;
}

/* Reset shared Traij */
void reset_SharedTraij()
{
    int i = 0;
    for (i = 0; i < SEMICFR; i++)
    {
        shared_m.trajectory.x[i] = NO_POS;
        shared_m.trajectory.y[i] = NO_POS;
    }
}

/* Manager for the game */
int manager_game()
{  
    int pid_g, pid_t;
    static tpars   params;		// Params for Graphic task

    /* Initialization of the game shared memory */
    init_mem_t(&shared_m);
    /* Create Graphic task */
    params = init_Param(PRIO_G, PERIOD_G);
    pid_g = ptask_create_param(game_play, &params);
    /* Create Target task */
    params = init_Param(PRIO_T, PERIOD_T);
    pid_t = ptask_create_param(target, &params);

    control_Writer();
    shared_m.pid[0] = pid_g;
    shared_m.pid[1] = pid_t;
    release_Writer();

    return 1;
}

/* Task trajectory calculation */
void trajectory_cannon(float speedx, float speedy)
{
    int		i = 0;
    float   old_x, old_y;
    float   x, y;
    float   dt = PERIOD_G * 0.0049; // TScale based on graphic period
    
    old_x = x =  PAD + 80 + 5*OFFSET;
    old_y = y = PAD + 5*OFFSET;
    
    control_Writer();
    reset_SharedTraij();
    release_Writer();

    while ((x <= XWIN) && (YWIN - y < YWIN - PAD) && i <= SEMICFR)
    {
        old_x = x;
        old_y = y;
        x = old_x + (speedx * dt);
        y = old_y + (speedy * dt) - (0.5 * G * dt * dt);
        speedy =  speedy - (G * dt);
        
        control_Writer();
        shared_m.trajectory.x[i] = (int) x;
        shared_m.trajectory.y[i] = (int) (YWIN - y);
        release_Writer();
        
        i += 1;
    }
}

/* Ptask for the charge cannon  */
ptask charge_cannon()
{
    int	up = 1;                 // Says if the pwr should grow or decrease
    int shot_pwr = 0;           
    int end_charge = 0;         // Check if Cannon-charge phase is over or not
    int end_main = 0;
    while (end_charge != -1 && end_main != 1)
    {
        if (up)
        {
            shot_pwr += 1;
            if (shot_pwr == MAX_PWR){ up = 0; }
        }
        else
        {
            shot_pwr -= 1;
            if (shot_pwr == 0){ up = 1; }
        }

        control_Writer();
        shared_m.shot_pwr = shot_pwr;
        release_Writer();

        if (ptask_deadline_miss())
        {
            control_Writer();
            shared_m.power_d += 1;
            release_Writer();
        }

        ptask_wait_for_period();
        control_Reader();
        end_charge = shared_m.end_charge;
        release_Reader();
        end_main = check_End();
    }
    return;
}

