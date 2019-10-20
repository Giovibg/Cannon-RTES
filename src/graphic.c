#include <allegro.h>
#include <stdio.h>

#include "ptask.h"
#include "graphic.h"
#include "manager.h"

static BITMAP	*target;                    // Ship target bitmap   
static BITMAP   *cannon;                    // Cannon bitmap
struct pos_t    pos[MAX_SHOTS];             // Actual ball positions
struct pos_t    old[MAX_SHOTS];             // Old ball positions
static struct postrail_t    trail;          // Trajectory points preview

/* Import Bitmaps */
void import_bitmap()
{
    target = load_bitmap("img/ship_r.bmp", NULL);
    draw_sprite(screen, target,TARGET_X,TARGET_Y);
    cannon = load_bitmap("img/can_r.bmp", NULL);
    draw_sprite(screen, cannon,PAD + 4*OFFSET,YWIN - PAD - 5*OFFSET);
}

/* Draw the colored blocks that indicate the power of the shot */
void charge_phase(int shot_pwr, int j_init)
{
    int	j;
    int color;

    for (j = j_init; j <= shot_pwr; j++)
    {
        if (j % MAX_PWR < 3 && 1 <= j % MAX_PWR)
        {
            color = 2;      //Dark Green
        }
        if (3 <= j % MAX_PWR && j%MAX_PWR <= 6)
        {
            color = 10;     //Light Green
        }
        if (j % MAX_PWR > 6 && j % MAX_PWR <= 8)
        {
            color = 14;     //Yellow
        }
        if (j % MAX_PWR > 8 || j % MAX_PWR == 0)
        {
            color = 12;     //Red
        }
        rectfill(screen,PAD + OFFSET + 1, (YWIN - PAD - (j - 1) * OFFSET) - 1,
                 PAD + 3 * OFFSET - 1, YWIN - PAD - j* OFFSET + 2, color); 
    }
}

/* Draws cannon power bar's line */
void draw_Pwrline(int shot_pwr, int end_charge)
{    
    if (end_charge != -1)
    {
        charge_phase(shot_pwr, 1);
    }
    else
    {
        charge_phase(shot_pwr, shot_pwr);   
    }
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
   
    rotate_sprite(screen,cannon,PAD + 4*OFFSET,YWIN - PAD - 5*OFFSET, 
                                                            f_cannon_degree);
    line(screen, PAD, YWIN - PAD, XWIN - PAD, YWIN - PAD, 15);
}

/* Change rate and score value */
void change_rate_score(int new_shots, int new_score)
{
    // tmp var for strings
    char    s[MSG_L];
    char    r[MSG_L];
    
    // Update rate graphic
    sprintf(r, "Rate: %d/%d", new_score, new_shots);
    textout_ex(screen, font, r, XWIN - PAD - 70, PAD/2, WHITE, BKG);

    // Update score graphic
    sprintf(s, "Score: %d", new_score);
    textout_ex(screen, font, s, PAD, PAD/2, WHITE, BKG);
}

/* Update deadline miss */
void update_deadline()
{
    int	graphic;
    int ball;
    int power;
    int target;

    /* strings to print */
    char    g[MSG_L];
    char    b[MSG_L];
    char    p[MSG_L];
    char    t[MSG_L];

    //Retrieve deadline counter. Protected!
    control_reader();           
    graphic = shared_m.graphic_d;
    ball = shared_m.ball_d;
    power = shared_m.power_d;
    target = shared_m.target_d;
    release_reader();

    //Update deadlines
    sprintf(g, "Graphic deadline miss: %d",graphic);
    textout_ex(screen, font, g, PAD, YWIN - PAD/2, WHITE, BKG);
    sprintf(b, "Ball deadline miss: %d",ball);
    textout_ex(screen, font, b, PAD + 250 + OFFSET, YWIN - PAD/2, WHITE, BKG);
    sprintf(p, "Power bar deadline miss: %d",power);
    textout_ex(screen, font, p, PAD + (2 * 250) + OFFSET, YWIN - PAD/2, 
                                                                WHITE, BKG);
    sprintf(t, "Target deadline miss: %d",target);
    textout_ex(screen, font, t, PAD + (3 * 250) + OFFSET, YWIN - PAD/2, 
                                                                WHITE, BKG);
}

/* Draw a new Shot ball */
void draw_Shots(struct pos_t pos, int color)
{
    circlefill(screen, pos.x, pos.y, RADIUS, color);
}

/* Draw playground borders */
void draw_Borders()
{  
    line(screen, XWIN - PAD, PAD, PAD, PAD, WHITE);
    line(screen, PAD, YWIN - PAD, PAD, PAD, WHITE);
    line(screen, XWIN - PAD, YWIN - PAD, XWIN - PAD, PAD, WHITE);
    line(screen, PAD, YWIN - PAD, XWIN - PAD, YWIN - PAD, WHITE);
}

/* Draw Wall */
void draw_wall(int target_x)
{
    struct pos_t    local_wall;
    
    // Retrieve x, y position of wall
    control_reader();
    local_wall.x = shared_m.pos_wall.x;
    local_wall.y = shared_m.pos_wall.y;
    release_reader();
    // Wall printed only if on left respect the target
    if (target_x - 30 > local_wall.x + WALL_W/2)
    {
        rectfill(screen,local_wall.x, YWIN - PAD,
                 local_wall.x + (WALL_W),local_wall.y,WHITE);
    }
}

void retrieve_trajectory()
{
    int j = 0;

    // Import trajectory to local
    control_reader();           
    trail.x[0] = shared_m.trajectory.x[0];
    trail.y[0] = shared_m.trajectory.y[0];
    release_reader();

    while (trail.y[j] != NO_POS && trail.x[j] != NO_POS && j < SEMICFR) 
    {          
        j += 1;
        control_reader();
        trail.x[j] = shared_m.trajectory.x[j];
        trail.y[j] = shared_m.trajectory.y[j];
        release_reader();
    }
}
/* Print trajectory preview */
void update_trajectory(int color)
{
    int	j = 0;
    while (trail.y[j] != NO_POS && trail.x[j] != NO_POS && j < SEMICFR)
    {          
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
    
    /* Game title */
    textout_ex(screen, font, "CANNON BALL!", XWIN/2 - 45, PAD/2, WHITE, 0);

    /* Drawing cannon power bar */
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
    textout_ex(screen, font, s, XWIN/2 - 45, YWIN/2, WHITE, 0);

    sprintf(s, "press space");
    textout_ex(screen, font, s, XWIN/2 - 45, YWIN/2 + 45, WHITE, 0);

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

/* Retrieve necessary data for updating graphics 
 from Shared Memory. Protected! 
 */
void retrieve_sharedm(int *shots, int *score, int *end_charge, int *shot_pwr, 
                            int *cannon_degree, int *target_x, int *update_traj)
{
    control_reader();
    *shots = shared_m.shots; 
    *score = shared_m.score;
    *end_charge = shared_m.end_charge;
    *shot_pwr = shared_m.shot_pwr;
    *cannon_degree = shared_m.cannon_degree;
    *target_x = shared_m.pos_target.x;
    *update_traj = shared_m.update_traj;
    release_reader();
}

/* Draws all the Shots with the new positions*/
void update_Shots()
{
    int i;

    for (i = 0; i < MAX_SHOTS; i++)
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
}

/* Check Deadline miss */
void check_DeadlineMiss()
{
    if (ptask_deadline_miss())
    {
        control_writer();
        shared_m.graphic_d += 1;
        release_writer();
    }
}

/* Task that update Game_Screen during play */
ptask game_play()
{
    int	shots, score = 0;
    int end_charge = -1;
    int shot_pwr = 1;
    int cannon_degree = -1;
    int old_cannon_degree = -1;
    int target_x = TARGET_X;
    int update_traj = 0;
    
    import_bitmap();
    while (1)
    {  
        /* Retrieve necessary data to update graphic */
        retrieve_sharedm(&shots, &score, &end_charge, &shot_pwr, 
                            &cannon_degree, &target_x, &update_traj);

        play_screen_init();
        update_deadline();
        draw_wall(target_x);

        /* Retrive new trajectory when cannon angle changed */
        if (old_cannon_degree != cannon_degree)
        {   
            retrieve_trajectory();
            if (cannon_degree != -180) // Fake position to update trajectory
            {
                old_cannon_degree = cannon_degree;
            }
            else
            {
                cannon_degree = old_cannon_degree;
                control_writer();
                shared_m.cannon_degree = old_cannon_degree;
                release_writer();
            }
        }
        if (update_traj)
        {
            update_trajectory(GREEN);
        }

        change_rate_score(shots, score);
        change_cannon(cannon_degree);
        change_target(target_x, TARGET_Y);
        draw_Pwrline(shot_pwr, end_charge);
        update_Shots();
        check_DeadlineMiss();
        
        ptask_wait_for_period();
    }
}