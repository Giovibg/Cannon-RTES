#include <allegro.h>
#include <stdio.h>

#include "ptask.h"
#include "graphic.h"
#include "manager.h"

static BITMAP *target;         
static BITMAP *cannon;  
static struct postrail_t trail; 
struct pos_t pos[MAX_SHOTS];
struct pos_t old[MAX_SHOTS];

/* Import Bitmaps */
void import_bitmap()
{
    target = load_bitmap("img/ship_r.bmp", NULL);
    draw_sprite(screen, target,TARGET_X,TARGET_Y);
    cannon = load_bitmap("img/can_r.bmp", NULL);
    draw_sprite(screen, cannon,PAD + 4*OFFSET,YWIN - PAD - 5*OFFSET);
}

/* Draws cannon power bar's line */
void draw_Pwrline(int shot_pwr)
{
    int j;
    int color;
    for(j = 1; j <= shot_pwr; j++)
    {
        if(j % MAX_PWR < 3 && 1 <= j % MAX_PWR)
        {
            color = 2;      //Dark Green
        }
        if(3 <= j % MAX_PWR && j%MAX_PWR <= 6)
        {
            color = 10;     //Light Green
        }
        if(j % MAX_PWR > 6 && j % MAX_PWR <= 8)
        {
            color = 14;     //Yellow
        }
        if(j % MAX_PWR > 8 || j % MAX_PWR == 0)
        {
            color = 12;     //Red
        }
        rectfill(screen,PAD + OFFSET + 1, (YWIN - PAD - (j - 1) * OFFSET) - 1,
                 PAD + 3 * OFFSET - 1, YWIN - PAD - j* OFFSET + 2, color);
        //line(screen,  PAD + OFFSET, YWIN - PAD - j*OFFSET, PAD + 3*OFFSET, YWIN - PAD - j*OFFSET, 0);
       
    }
    /*for(;j <= 10; j++)
    {
       // line(screen,  PAD + OFFSET, YWIN - PAD - j*OFFSET, PAD + 3*OFFSET, YWIN - PAD - j*OFFSET, 0);
    }*/
}

/* Draw Power Box */
void draw_PwrBar()
{
    line(screen,  PAD + 3*OFFSET, YWIN - PAD, 
        PAD + 3*OFFSET, YWIN - PAD - MAX_PWR * OFFSET, 15);
    line(screen,  PAD + OFFSET, YWIN - PAD,
         PAD + 1*OFFSET, YWIN - PAD - MAX_PWR * OFFSET, 15);
    line(screen,  PAD + OFFSET, YWIN - PAD - MAX_PWR*OFFSET - 1,
         PAD + 3*OFFSET, YWIN - PAD - MAX_PWR * OFFSET - 1, 15);
}

/* Change Target bitmap */
void change_target(int x, int y)
{
    draw_sprite(screen, target,x,y);
}

/* Rotate cannon bitmap */
void change_cannon(int cannon_degree)
{
    fixed f_cannon_degree;

    f_cannon_degree = -itofix(cannon_degree);
   
    rotate_sprite(screen,cannon,PAD + 4*OFFSET,YWIN - PAD - 5*OFFSET, f_cannon_degree);
    line(screen, PAD, YWIN - PAD, XWIN - PAD, YWIN - PAD, 15);
}

/* Change rate and score value */
void change_rate_score(int new_shots, int new_score)
{
    // tmp var for strings
    char s[MSG_L];
    char r[MSG_L];
    
    // Update rate graphic
    sprintf(r, "Rate: %d/%d", new_score, new_shots);
    textout_ex(screen, font, r, XWIN - PAD - 70, PAD/2, 15, 0);

    // Update score graphic
    sprintf(s, "Score: %d", new_score);
    textout_ex(screen, font, s, PAD, PAD/2, 15, 0);
}

/* Draw a new Shot ball */
void draw_Shots(struct pos_t pos, int color)
{
    circlefill(screen, pos.x, pos.y, RADIUS, color);
}

/* Draw playground borders */
void draw_Borders()
{  
    line(screen, XWIN - PAD, PAD, PAD, PAD, 15);
    line(screen, PAD, YWIN - PAD, PAD, PAD, 15);
    line(screen, XWIN - PAD, YWIN - PAD, XWIN - PAD, PAD, 15);
    line(screen, PAD, YWIN - PAD, XWIN - PAD, YWIN - PAD, 15);
}

/* Draw Wall */
void draw_wall(int score)
{
    struct pos_t local_wall;
    
    // Retrieve x, y position of wall and score
    control_reader();
    local_wall.x = shared_m.pos_wall.x;
    local_wall.y = shared_m.pos_wall.y;
    release_reader();
    rectfill(screen,local_wall.x, YWIN - PAD,
                     local_wall.x + (WALL_W),local_wall.y,WHITE);

}

void retrieve_trajectory()
{
    int j = 0;

    control_reader();           //import trajectory to local
    trail.x[0] = shared_m.trajectory.x[0];
    trail.y[0] = shared_m.trajectory.y[0];
    release_reader();
    while(trail.y[j] != NO_POS) 
    {                           // SHAREDM.TRAJECTORY.X[XWIN] È UN VALORE,
        j += 1;                 // SHAREDM.TRAJECTORY.Y[XWIN] È FUORI ARRAY
        control_reader();
        trail.x[j] = shared_m.trajectory.x[j];
        trail.y[j] = shared_m.trajectory.y[j];
        release_reader();
    }
}

void update_trajectory(int color)
{
    int j = 0;
    while(trail.y[j] != NO_POS) // STA ROBA NON È BELLA, XK XWIN > YWIN,
    {                           // SHAREDM.TRAJECTORY.X[XWIN] È UN VALORE,
                                // SHAREDM.TRAJECTORY.Y[XWIN] È FUORI ARRAY
        putpixel(screen, trail.x[j], trail.y[j], color);
        j += 1;   
    }
}

/* Draws game interface and screen */
void play_screen_init()
{
    clear_to_color(screen, BKG);

    /* Drawing playground borders */
    draw_Borders();

    /* Score statistic info */
    // Score and Rate hit/shot
    change_rate_score(0, 0);
    
    // Game title
    textout_ex(screen, font, "CANNON BALL!", XWIN/2 - 45, PAD/2, 15, 0);

    // Drawing cannon power bar
    draw_PwrBar();
}

/* Draws menu interface */
void menu_screen_init()
{
    clear_to_color(screen, BKG);

     // Drawing playground borders
    draw_Borders();

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

// Initialize graphic environment
void gui_init()
{
    // Initialize display and keyboard interactions
    display_init();

    // Draws game interface and screen
    menu_screen_init();
}

/* Task that update Game_Screen during play */
ptask game_play()
{
    int i;
    int shots, score;
    int end_charge = -1;
    int shot_pwr = 0;
    int old_pwr = 0;
    int cannon_degree = -1;
    int old_cannon_degree = -1;
    int target_x = TARGET_X;
    int update_traj = 0;

    for(i = 0; i < MAX_SHOTS; i++)
    {
        pos[i].x = NO_POS;
        pos[i].y = NO_POS;
    }

    import_bitmap();

    while(1)
    {  
        i = 0;  

        // Retrieve necessary data for updating graphics 
        // from Shared Memory. Protected!
        control_reader();
        shots = shared_m.shots; 
        score = shared_m.score;
        end_charge = shared_m.end_charge;
        shot_pwr = shared_m.shot_pwr;
        cannon_degree = shared_m.cannon_degree;
        target_x = shared_m.pos_target.x;
        update_traj = shared_m.update_traj;
        release_reader();

        play_screen_init();
        draw_wall(score);
        if(old_cannon_degree != cannon_degree || old_pwr != shot_pwr)
        {
            retrieve_trajectory();
            old_cannon_degree = cannon_degree;
            old_pwr = shot_pwr;
        }

        if(update_traj)
        {
           update_trajectory(10);
        }

        change_rate_score(shots, score);
        change_cannon(cannon_degree);
        change_target(target_x, TARGET_Y);

        if (end_charge != -1)
        {
            draw_Pwrline(shot_pwr);
        }

        for(i = 0; i < MAX_SHOTS; i++)
        {
            old[i].x = pos[i].x;
            old[i].y = pos[i].y;

            control_reader();
            pos[i].x = shared_m.pos[i].x;
            pos[i].y = shared_m.pos[i].y;
            release_reader();
            draw_Shots(old[i], BKG);
            if(pos[i].x != NO_POS && pos[i].y != NO_POS)
            {
                draw_Shots(pos[i], WHITE);
            }
        }
        ptask_wait_for_period();
    }
}