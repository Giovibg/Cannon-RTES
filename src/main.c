#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <allegro.h>

#include "main.h"
#include "ptask.h"
#include "manager.h"
#include "graphic.h"

int main(void)
{
    
    int k;                  // Character from keyboard

    int bool_manager = 0;   // Check manager task activated
    int n_shots = 0;        

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

            /* If press enter after pressing space once, create ball task */
            if (k == KEY_ENTER && bool_manager == 1 && n_shots < MAX_SHOTS)
            {   
                printf("Sono main e creo una ballina\n");
                shot_create();
                n_shots += 1;
            }    
        }
        
    } while(k != KEY_ESC);    
    
    allegro_exit();
    return 0;
}