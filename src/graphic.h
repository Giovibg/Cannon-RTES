#ifndef GRAPHIC_H
#define GRAPHIC_H

//-------------------------------------------------------------
// GLOBAL CONSTANTS
//-------------------------------------------------------------
#define XWIN 1060
// window x resolution
#define YWIN 680
// window y resolution
#define PAD 60
// padding for game's statistics
#define XBASE 40
// X start for the message
#define YBASE 50
// Y level for the first task
#define YINC 30
// Y increment for the other tasks
#define BKG 0
// background color
#define MSG_L 50
// string lenght
//-------------------------------------------------------------

/* Change shot value */
void set_rate(int new_shot);

/* Change score value */
void set_score(int new_score);

/* Draws game interface and screen */
void screen_init();

/* Initialize display and keyboard interactions */
void display_init();

/* Initialize graphic environment */
void gui_init();

#endif