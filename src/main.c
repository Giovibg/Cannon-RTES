#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <allegro.h>
#include <math.h>

#include "main.h"
#include "ptask.h"
#include "manager.h"
#include "graphic.h"

static int cannon_degree = 0;               // Degree of cannon     
static int k = 0;                           // Character from keyboard

/* Set Update trajectory. Protected! */
void set_UpdateTrajectory(int bool)
{
    control_writer();
    shared_m.update_traj = bool;
    shared_m.cannon_degree = -180;
    release_writer();
}

/* Set if trajectory should be updated. Protected! */
void update_CannonDegree(int cannon_degree)
{
    control_writer();
    shared_m.cannon_degree = cannon_degree;
    release_writer();
}

void speedxy(int shot_pwr)
{
    float speed_x = 0;              // Horizontal speed
    float speed_y = 0;              // Vertical speed
    float cannon_rad;               // Degree of the cannon in radiant

    cannon_rad = (cannon_degree * 3 * PIGRECO) / 180;
    // printf("Radiant:%f\n",cannon_rad);
    speed_x = shot_pwr * cos(cannon_rad);
    // printf("Horizontal v: %f\n",speed_x);

    speed_y = shot_pwr * sin(cannon_rad);
    // printf("Vertical v: %f\n",speed_y);
    trajectory_cannon(speed_x,speed_y);
}

/* Phase where the user can set the Cannon angle */
int set_CannonAngle(int shot_pwr)
{
    int bool_manager = 3;           // Var to check if change to next phase
    
    if(keypressed()) 
    {    
        k = readkey() >> 8;                
        if (k == KEY_UP && cannon_degree <= MAX_DEG)
        {
            cannon_degree += 1;
            update_CannonDegree(cannon_degree);
        }
        else if(k == KEY_DOWN && cannon_degree >= MIN_DEG)
        {
            cannon_degree -= 1;
            update_CannonDegree(cannon_degree);
        }
        else if(k == KEY_ENTER)
        {
            bool_manager = 4;
        }
        else if(k == KEY_ESC)
        {
            bool_manager = 5;
        }

        speedxy(shot_pwr);
    }
    return bool_manager;
}

/* End the Charge cannon phase and return the power of the shot */
int get_CannonPwr()
{
    int shot_pwr;
    int ret;
    control_writer();
    shared_m.end_charge = -1;
    release_writer();

    control_reader();
    shot_pwr = shared_m.shot_pwr;
    release_reader();
    //Scaler makes speed omogeneous
    int scaler[10] = {22, 22, 17, 17, 15, 13, 13, 12, 11, 10};
    ret = scaler[shot_pwr - 1] * shot_pwr;
}

/* Create the cannon task and set the shared memory variable to the correct value */
int set_CannonPwr()
{
    tpars params;

    control_writer();
    shared_m.end_charge = 0;
    release_writer();

    // Create cannon_charge task
    params = init_param(PRIO_M, PERIOD_M);
    ptask_create_param(charge_cannon, &params);

    return 2;
}

int main(void)
{  
    int bool_manager = 0;               // Check manager task activated
    int n_shots = 0;        
    int shot_pwr = 0;                   // Power of the shot
    int last_degree = 0;                    // Degree of the previw  

    gui_init();
    /* Ptask initialization */
    ptask_init(SCHED_FIFO, GLOBAL, NO_PROTOCOL);  
    do{
        if (keypressed()){   
            k = readkey() >> 8;
            printf("Main k: %d\n", k);
            /* If press space, game start */
            if (k == KEY_SPACE && bool_manager == 0){ bool_manager = manager_game(); }   
            /* bool manager = 1 and enter in charge_cannon phase */
            if (k == KEY_ENTER && bool_manager == 1)
            {
                k = 0;
                bool_manager = set_CannonPwr();
            }
            /* bool manager = 2 and exit from charge_cannon phase */
            if (k == KEY_ENTER && bool_manager == 2)
            {
                shot_pwr = get_CannonPwr();
                bool_manager = 3;
                set_UpdateTrajectory(1);
                speedxy(shot_pwr);
            }
            /* Set the cannon angle */
            while(bool_manager == 3)
            { 
                bool_manager = set_CannonAngle(shot_pwr);
            }

            /* Create a new Shot  */
            if(n_shots < MAX_SHOTS && bool_manager == 4)
            {   
                set_UpdateTrajectory(0);
                n_shots += 1;
                bool_manager = shot_create();   
            }
        }
    } while(k != KEY_ESC);    
    allegro_exit();
    return 0;
}