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
    display_init();

    do
    {
        k = 0;
        
        if (keypressed())
        {
            c = readkey();
            k = c >> 8;         
            
        }
    } while (k != KEY_ESC);

    allegro_exit();
    return 0;
}