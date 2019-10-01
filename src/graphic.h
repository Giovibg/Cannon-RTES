#ifndef GRAPHIC_H
#define GRAPHIC_H

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
void display_init();

#endif