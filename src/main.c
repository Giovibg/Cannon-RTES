#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <allegro.h>
#include "main.h"
#include "ptask.h"
#include "manager.h"
#include "graphic.h"

tpars init_param(void)
{
    tpars params = TASK_SPEC_DFL;
    params.period = tspec_from(PERIOD_M, MILLI);
    params.rdline = tspec_from(PERIOD_M, MILLI);
    params.priority = PRIO_M;
    params.measure_flag = 1;
    params.act_flag = NOW;
    return params;
}

int main(void)
{
    
    int k;                  // Character from keyboard
    int ret;                // Return ptask index
    tpars params;           // Manager task params
    int bool_manager = 0;   // Check manager task activated

    /* Initialize graphic environment */
    gui_init();

    /* Initialize manager task params */
    params = init_param();

    /* Ptask initialization */
    ptask_init(SCHED_FIFO, GLOBAL, PRIO_INHERITANCE);

    do
    {
        k = 0;
        if (keypressed())
        {
            k = readkey() >> 8;

            /* If press space, game start */
            if (k == KEY_SPACE && bool_manager == 0)
            {     
                bool_manager = 1;
                ret = ptask_create_param(manager_game, &params);
                printf("Task id:%d\n",ret);
            }
            

        }
    } while(k != KEY_ESC);    

    allegro_exit();
    return 0;
}