#include <allegro.h>
#include <stdio.h>

#include "ptask.h"
#include "graphic.h"
#include "manager.h"

static int score = 0;
static int shots = 0;
static BITMAP *target;         
static BITMAP *cannon;          
struct pos_t pos[MAX_SHOTS];
struct pos_t old[MAX_SHOTS];

/* Import Bitmaps */
void import_bitmap()
{
    target = load_bitmap("img/ship_r.bmp", NULL);
    blit(target,screen,0,0,XWIN - PAD - 9*OFFSET,YWIN - PAD - 10*OFFSET,100,100);
    cannon = load_bitmap("img/can_r.bmp", NULL);
    blit(cannon,screen,0,0,PAD + 5*OFFSET,YWIN - PAD - 5*OFFSET,80,80);
}
/* Change rate and score value */
void change_rate_score(int new_shots, int new_score)
{
    shots = new_shots;
    score = new_score;

    // tmp var for strings
    char s[MSG_L];
    char r[MSG_L];
    
    // Update rate graphic
    sprintf(r, "Rate: %d/%d", score, shots);
    textout_ex(screen, font, r, XWIN - PAD - 70, PAD/2, 15, 0);

    // Update score graphic
    sprintf(s, "Score: %d", score);
    textout_ex(screen, font, s, PAD, PAD/2, 15, 0);
}

void draw_ball(struct pos_t pos, int color)
{
    circlefill(screen, pos.x, pos.y, RADIUS, color);
}

void update_trajectory(int color)
{
    struct pos_t trail;
    int j = 0;
    while(shared_m.trajectory[j].x != -1)
    {
        control_reader();
        trail.x = shared_m.trajectory[j].x;
        trail.y = shared_m.trajectory[j].y;
        release_reader();
        putpixel(screen, trail.x, trail.y, color);
        j += 1;
    }
}


/* Task that update Game_Screen during play */
ptask game_play()
{
    int i, j;
    int end_charge = -1;
    int shot_pwr = 0;

    for(i = 0; i < MAX_SHOTS; i++)
    {
        pos[i].x = NO_POS;
        pos[i].y = NO_POS;
    }

    while(1)
    {  
        i = 0;
        import_bitmap();
        
        
        change_rate_score(shots, score);

        control_reader();// Update global static variable of shots and score. Protected!
        shots = shared_m.shots; 
        score = shared_m.score;
        end_charge = shared_m.end_charge;
        shot_pwr = shared_m.shot_pwr;
        release_reader();

        if (end_charge != -1)
        {
            for(j = 1; j <= shot_pwr; j++)
            {
                line(screen,  PAD + 1*OFFSET, YWIN - PAD - j*OFFSET, PAD + 3*OFFSET, YWIN - PAD - j*OFFSET, 15);
            }
            for(;j <= 10; j++)
            {
                line(screen,  PAD + 1*OFFSET, YWIN - PAD - j*OFFSET, PAD + 3*OFFSET, YWIN - PAD - j*OFFSET, 0);
            }
        }
       
        update_trajectory(RED);
        for(i = 0; i < MAX_SHOTS; i++)
        {
            old[i].x = pos[i].x;
            old[i].y = pos[i].y;

            control_reader();
            pos[i].x = shared_m.pos[i].x;
            pos[i].y = shared_m.pos[i].y;
            release_reader();
            draw_ball(old[i], BKG);
            if(pos[i].x != NO_POS && pos[i].y != NO_POS)
            {
                draw_ball(pos[i], WHITE);
            }
        }
        ptask_wait_for_period();
    }
}

/* Draws game interface and screen */
void play_screen_init()
{
    clear_to_color(screen, BKG);

    // Drawing playground borders
    line(screen, PAD, YWIN - PAD, PAD, PAD, 15);
    line(screen, XWIN - PAD, YWIN - PAD, XWIN - PAD, PAD, 15);
    line(screen, PAD, YWIN - PAD, XWIN - PAD, YWIN - PAD, 15);
    line(screen, XWIN - PAD, PAD, PAD, PAD, 15);

    /* Score statistic info */
    // Score and Rate hit/shot
    change_rate_score(0, 0);
    
    // Game title
    textout_ex(screen, font, "CANNON BALL!", XWIN/2 - 45, PAD/2, 15, 0);

    line(screen,  PAD + 3*OFFSET, YWIN - PAD - OFFSET, PAD + 3*OFFSET, YWIN - PAD - 13*OFFSET, 15);
    line(screen,  PAD + 1*OFFSET, YWIN - PAD - OFFSET, PAD + 1*OFFSET, YWIN - PAD - 13*OFFSET, 15);
    line(screen,  PAD + 1*OFFSET, YWIN - PAD - 13*OFFSET, PAD + 3*OFFSET, YWIN - PAD - 13*OFFSET, 15);
}

/* Draws menu interface */
void menu_screen_init()
{
    clear_to_color(screen, BKG);

     // Drawing playground borders
    line(screen, PAD, YWIN - PAD, PAD, PAD, 15);
    line(screen, XWIN - PAD, YWIN - PAD, XWIN - PAD, PAD, 15);
    line(screen, PAD, YWIN - PAD, XWIN - PAD, YWIN - PAD, 15);
    line(screen, XWIN - PAD, PAD, PAD, PAD, 15);

    // tmp var to for writing message
    char s[MSG_L];

    /* Title and Instructions */
    // Game title
    sprintf(s, "CANNON BALL!");
    textout_ex(screen, font, s, XWIN/2 - 45, YWIN/2, 15, 0);

    sprintf(s, "press space");
    textout_ex(screen, font, s, XWIN/2 - 45, YWIN/2 + 45, 15, 0);
}

/* Initialize display and keyboard interactions */
void display_init()
{
    allegro_init();

    set_color_depth(8);

    set_gfx_mode(GFX_AUTODETECT_WINDOWED, XWIN, YWIN, 0, 0);
    clear_to_color(screen, BKG);
    
    install_keyboard();
}

/* Initialize graphic environment */
void gui_init()
{
    // Initialize display and keyboard interactions
    display_init();

    // Draws game interface and screen
    menu_screen_init();
}