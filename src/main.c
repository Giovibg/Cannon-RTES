#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <allegro.h>
#include <math.h>

#include "main.h"
#include "ptask.h"
#include "manager.h"
#include "graphic.h"

int main(void)
{  
    int k;                      // Character from keyboard

    int bool_manager = 0;       // Check manager task activated
    int n_shots = 0;        
    
    int shot_pwr = 0;           // Power of the shot
    float cannon_degree = 10;     //Start degree of cannon
    float speed_x = 0;            // Horizontal speed
    float speed_y = 0;            // Vertical speed

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

                control_writer();
                shared_m.end_charge = 0;
                release_writer();

                bool_manager = 2;
                /* Create cannon_charge task */
                params = init_param(PRIO_M, PERIOD_M);
                ptask_create_param(charge_cannon, &params);
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

            if ((k == KEY_UP || k == KEY_DOWN) && bool_manager == 3)
            {
                
                do{
                    
                    if (k == KEY_UP)
                    {
                        cannon_degree += 1;
                        printf("Gradi cannone: %f\n",cannon_degree);
                    }
                    else
                    {
                        cannon_degree -= 1 ;
                        printf("Gradi cannone: %f\n",cannon_degree);
                    }
                    float rad =  (cannon_degree * M_PI) / 180;
                    printf("Radiant:%f\n",rad);
                    speed_x = shot_pwr * cos(rad);
                    printf("Horizontal v: %f\n",speed_x);
                    speed_y = shot_pwr * sin(rad);
                    printf("Vertical v: %f\n",speed_y);
                    k = readkey() >> 8;
                    trajectory_cannon(speed_x,speed_y);
                }while(k != KEY_ENTER);

                
                if(n_shots < MAX_SHOTS)
                {
                    printf("Make shot\n");
                    shot_create();
                    n_shots += 1;
                    bool_manager = 1;
                }
            }

        }
        
    }while(k != KEY_ESC);    
    
    allegro_exit();
    return 0;
}