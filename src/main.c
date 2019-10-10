#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <allegro.h>
#include <math.h>

#include "main.h"
#include "ptask.h"
#include "manager.h"
#include "graphic.h"

/* Create the cannon task and set the shared memory variable to the correct value */
create_cannon()
{
    tpars params;

    control_writer();
    shared_m.end_charge = 0;
    release_writer();

    // Create cannon_charge task
    params = init_param(PRIO_M, PERIOD_M);
    ptask_create_param(charge_cannon, &params);
}

int main(void)
{  
    int k;                          // Character from keyboard

    int bool_manager = 0;           // Check manager task activated
    int n_shots = 0;        
    int shot_pwr = 0;               // Power of the shot

    float cannon_degree = 0;        // Start degree of cannon
    float cannon_rad;               // Degree of the cannon in radiant
    float speed_x = 0;              // Horizontal speed
    float speed_y = 0;              // Vertical speed

    tpars params;

    /* Initialize graphic environment */
    gui_init();

    /* Ptask initialization */
    ptask_init(SCHED_FIFO, GLOBAL, NO_PROTOCOL);

    do
    {
        k = 0;
        if (keypressed())
        {   
            k = readkey() >> 8;
            printf("Main k: %d\n", k);

            /* If press space, game start */
            if (k == KEY_SPACE && bool_manager == 0)
            {             
                bool_manager = 1;
                manager_game();
            }   

            // bool manager = 1 e enter -> fase carica
            if (k == KEY_ENTER && bool_manager == 1)
            {
                k = 0;
                bool_manager = 2;

                // Create the cannon task and set the shared memory variable to the correct value
                create_cannon();
            }

            if (k == KEY_ENTER && bool_manager == 2)
            {
               
                control_writer();
                shared_m.end_charge = -1;
                release_writer();

                control_reader();
                shot_pwr = shared_m.shot_pwr;
                release_reader();
                shot_pwr *= 10;
                bool_manager = 3;
                printf("POTENZA!!: %d\n", shot_pwr);
            }

            while(bool_manager == 3)
            { 
                if(keypressed()) 
                {   
                    k = readkey() >> 8;                
                    if (k == KEY_UP && cannon_degree <= MAX_DEG)
                    {
                        cannon_degree += 1;
                        control_writer();
                        shared_m.cannon_degree = cannon_degree;
                        release_writer();
                        printf("Gradi cannone: %f\n",cannon_degree);
                    }
                    else if(k == KEY_DOWN && cannon_degree >= MIN_DEG)
                    {
                        cannon_degree -= 1 ;
                        control_writer();
                        shared_m.cannon_degree = cannon_degree;
                        release_writer();
                        printf("Gradi cannone: %f\n",cannon_degree);
                    }
                    else if (k == KEY_ENTER || k == KEY_ESC)
                    {
                        bool_manager = 4;
                    }

                    cannon_rad = (cannon_degree * M_PI) / 180;
                    printf("Radiant:%f\n",cannon_rad);

                    speed_x = shot_pwr * cos(cannon_rad);
                    printf("Horizontal v: %f\n",speed_x);

                    speed_y = shot_pwr * sin(cannon_rad);
                    printf("Vertical v: %f\n",speed_y);

                    trajectory_cannon(speed_x,speed_y);
                }
            }
                
            if(n_shots < MAX_SHOTS && bool_manager == 4)
            {
                printf("Make shot\n");
                shot_create();
                n_shots += 1;
                bool_manager = 1;
            }
        }
        
    }while(k != KEY_ESC);    
    
    allegro_exit();
    return 0;
}