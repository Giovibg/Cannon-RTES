#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <allegro.h>
#include "ptask.h"

//-------------------------------------------------------------
// GLOBAL CONSTANTS
//-------------------------------------------------------------
#define XWIN 640
// window x resolution
#define YWIN 480
// window y resolution
#define XBASE 40
// X start for the message
#define YBASE 50
// Y level for the first task
#define YINC 30
// Y increment for the other tasks
#define BKG 0
// background color
//-------------------------------------------------------------


/* Initialize display and keyboard interactions */
void display_init()
{
    allegro_init();
    set_gfx_mode(GFX_AUTODETECT_WINDOWED, XWIN, YWIN, 0, 0);
    clear_to_color(screen, BKG);
    
    install_keyboard();
}


int main(void)
{
    int k, c; //character from keyboard

    display_init();

    do
    {
        k = 0;
        
        if (keypressed())
        {
            c = readkey();
            k = c >> 8;
            
            init_gestor();
            
            
        }
    } while (k != KEY_ESC);

    allegro_exit();
    return 0;
}