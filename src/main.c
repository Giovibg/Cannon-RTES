#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <allegro.h>
#include "main.h"
#include "ptask.h"
#include "graphic.h"

int main(void)
{
    int k, c;                       // characters from keyboard
    gui_init();

    int score = 0;
    int shot = 0;

    do
    {
        k = 0;
        
        if (keypressed())
        {
            c = readkey();
            k = c >> 8;

            if (k == KEY_SPACE)         
            {
                score+=1;
                set_score(score);
            }

            if (k == KEY_ENTER)         
            {
                shot+=1;
                set_rate(shot);
            }
            
        }
    } while (k != KEY_ESC);

    allegro_exit();
    return 0;
}