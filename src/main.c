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
    int k;                          // Character from keyboard
    int i = 0;
    
    /* Initialize graphic environment */
    gui_init();

    // Wait "KEY_SPACE" from player in the menu
    do
    {
        k = 0;
        if (keypressed())
        {
            k = readkey() >> 8;
            // Funz che fa lampeggiare la scritta press space
        }
    } while(k != KEY_ESC && k != KEY_SPACE);    

    if(k != KEY_ESC)
    {
        manager_game();
    }

    allegro_exit();
    return 0;
}