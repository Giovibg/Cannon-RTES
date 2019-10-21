#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <allegro.h>
#include <math.h>

#include "main.h"
#include "ptask.h"
#include "manager.h"
#include "graphic.h"

static int	cannon_degree = 0;		// Degree of cannon     
static int	k = 0;				    // Character from keyboard

/* Set Update trajectory. Protected! */
void set_UpdateTrajectory(int bool)
{
    control_Writer();
    shared_m.update_traj = bool;
    shared_m.cannon_degree = -180;
    release_Writer();
}

/* Calculate speed on axis X and Y and compute the trajectory */
void speedxy(int shot_pwr)
{
    float	speed_x = 0;			// Horizontal speed
    float	speed_y = 0;			// Vertical speed
    float	cannon_rad;            	// Degree of the cannon in radiant

    cannon_rad = (cannon_degree * 3 * PIGRECO) / 180;
    speed_x = shot_pwr * cos(cannon_rad);
    speed_y = shot_pwr * sin(cannon_rad);
    trajectory_cannon(speed_x,speed_y);
}

/* Set if trajectory should be updated. Protected! */
void update_CannonDegree(int cannon_degree)
{
    control_Writer();
    shared_m.cannon_degree = cannon_degree;
    release_Writer();
}

/* Phase where the user can set the Cannon Degree */
int set_CannonDegree(int shot_pwr)
{
    int	bool_manager = 3;   // Var to check if change to next phase
    
    if(keypressed()) 
    {    
	    k = readkey() >> 8;                
        if (k == KEY_UP && cannon_degree <= MAX_DEG){
            cannon_degree += 1;
            update_CannonDegree(cannon_degree);
        }
        else if (k == KEY_DOWN && cannon_degree >= MIN_DEG){
            cannon_degree -= 1;
            update_CannonDegree(cannon_degree);
        }
        else if (k == KEY_ENTER){
            bool_manager = 4;
        }
        else if (k == KEY_ESC){
            bool_manager = 5;
        }
        speedxy(shot_pwr);
    }
    return bool_manager;
}

/* End the Charge cannon phase and return the power of the shot */
int get_CannonPwr()
{
    int	shot_pwr;
    int scaler[10] = {22, 22, 17, 17, 15,   
                        13, 13, 12, 11, 10}; // Scaler makes speed more uniform 

    control_Writer();
    shared_m.end_charge = -1;
    release_Writer();

    control_Reader();
    shot_pwr = shared_m.shot_pwr;
    release_Reader();

    return scaler[shot_pwr - 1] * shot_pwr;
}

/* Create the cannon task and set shared memory variable to correct value */
int set_CannonPwr()
{
    int pid;
    tpars	params;

    /* Create cannon_charge task */
    params = init_Param(PRIO_M, PERIOD_M);
    pid = ptask_create_param(charge_cannon, &params);

    control_Writer();
    shared_m.end_charge = 0;
    shared_m.pid[0] = pid;
    release_Writer();

    return 2;
}
 
/* Set end var to 1 and waits for join */
void check_Join(int n_shots)
{
    int i;
    int local_pid[n_shots + 3];

    control_Writer();
    shared_m.end = 1;
    release_Writer();

    /* Retrieve all pid from shared_m */
    for(i = 0; i <= n_shots + 2; i++)
    {
        control_Reader();
        local_pid[i] = shared_m.pid[i];
        release_Reader();
    }

    /* Waits all the tasks to end */
    for(i = 0; i <= n_shots + 2; i ++)
    {
        if(local_pid[i] != -1)
        {
            pthread_join(ptask_get_threadid(local_pid[i]), 0);
        }
    }
}

int main(void)
{  
    int	bool_manager = 0;               	// Check manager task activated
    int n_shots = 0;                    	// Number of total shot
    int shot_pwr = 0;                   	// Power of the shot

    init_Gui();
    ptask_init(SCHED_FIFO, GLOBAL, NO_PROTOCOL);  
    do { 
        if (keypressed()){ 
            k = readkey() >> 8;
            /* If press space, game start */
            if (k == KEY_SPACE && bool_manager == 0){ 
                bool_manager = manager_game();
            }   
            /* bool manager = 1 and enter in charge_cannon phase */
            if (k == KEY_ENTER && bool_manager == 1){
                k = 0;
                bool_manager = set_CannonPwr();
            }
            /* bool manager = 2 and exit from charge_cannon phase */
            if (k == KEY_ENTER && bool_manager == 2){
                shot_pwr = get_CannonPwr();
                bool_manager = 3;
                set_UpdateTrajectory(1);
                speedxy(shot_pwr);
            }
            /* Set the cannon Degree */
            while (bool_manager == 3){ 
                bool_manager = set_CannonDegree(shot_pwr);
            }
            /* Create a new Shot  */
            if (n_shots < MAX_SHOTS && bool_manager == 4){   
                set_UpdateTrajectory(0);
                n_shots += 1;
                bool_manager = create_Shot(n_shots);   
            }
        }
    } while(k != KEY_ESC); 

    if(bool_manager != 0){ check_Join(n_shots); }
    allegro_exit();
    return 0;
}
