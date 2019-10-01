#include <allegro.h>
#include "graphic.h"

/* Initialize display and keyboard interactions */
void display_init()
{
    allegro_init();
    set_gfx_mode(GFX_AUTODETECT_WINDOWED, XWIN, YWIN, 0, 0);
    clear_to_color(screen, BKG);
    
    install_keyboard();
}