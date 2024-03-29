#include "manager.h"
#include "ptask.h"
#include "shot.h"
#include <stdlib.h>

/* Wall collision rebounce */
void rebounce_wall(struct postrail_t *reb, int j)
{
    int wall_x;
    int dist_xwallx;
    int shift = 1;
    control_Reader();
    wall_x = shared_m.pos_wall.x;
    release_Reader();
    
    
    while (reb->x[j] != NO_POS)
    {
        dist_xwallx = reb->x[j] - wall_x;
        reb->x[j] = (wall_x - dist_xwallx);
        reb->y[j] += (shift * 0.005); 
        j += 1;
        shift += 1;
    }
}

/* Check shot out of borders */
int check_border(int x, int y, int index) 
{
    int ret = 0;

    if (x >= XWIN - PAD - RADIUS  ||
        y <= PAD + RADIUS || 
        y >= YWIN - PAD - RADIUS)
    {
        control_Writer();
        shared_m.pos[index].x = NO_POS;     // Delete ball
        shared_m.pos[index].y = NO_POS;
        release_Writer();
        ret = 1;
    }
    return ret;
}

/* Check ball hit target */
int check_target(int x, int y, int index)
{
    int	ret = 0;
    struct pos_t	target_p;

    // Get target position. Protected!
    control_Reader();
    target_p.x = shared_m.pos_target.x;     
    target_p.y = shared_m.pos_target.y;
    release_Reader();

    // Condition ball hit target
    if (x >= (target_p.x - RADIUS) && 
        y >= (target_p.y - OFFSET) && 
        (x <= target_p.x + 8*OFFSET))    
    {
        control_Writer();
        shared_m.score += 1;                // Update score
        shared_m.pos[index].x = NO_POS;     // Delete ball
        shared_m.pos[index].y = NO_POS;
        release_Writer();

        ret = 1;
    }
    return ret;
}

/* Check Wall collision */
int check_wall(int x, int y, int index)
{
    int	ret = 0;
    struct pos_t 	wall_p;

    // Get target position. Protected!
    control_Reader();
    wall_p.x = shared_m.pos_wall.x;     
    wall_p.y = shared_m.pos_wall.y;
    release_Reader();
    if ((x >= wall_p.x - RADIUS) && (x <= wall_p.x + WALL_W) && (y >= wall_p.y))
    {
        ret = 1;
    }
    return ret;
}

void update_wall()
{
    int score = 0;			    // Retrieve score
    int x_dir = 0;			    // Random direction position wall
    struct pos_t	old_wall_p;	// Old wall position
    struct pos_t	new_wall_p;	// New wall position

    control_Reader();
    score = shared_m.score;
    old_wall_p.x = shared_m.pos_wall.x;
    old_wall_p.y = shared_m.pos_wall.y;
    release_Reader();
    // Random generation wall position
    if (((old_wall_p.x % old_wall_p.y) % 2) == 0)
    {
        x_dir = 1;
    }
    else
    {
        x_dir = -1;
    }

    new_wall_p.y = old_wall_p.y - (score * OFFSET) - (rand()%10);
    new_wall_p.x = old_wall_p.x - (x_dir * score * OFFSET) - x_dir 
                                                            * (rand()%60 + 1);
            
    control_Writer();
    shared_m.pos_wall.y = new_wall_p.y;
    shared_m.pos_wall.x = new_wall_p.x;
    release_Writer();
}

void get_trajectory(struct postrail_t *pos)
{
    int j = 0;
    control_Reader();
    pos->x[0] = shared_m.trajectory.x[0];	// First trajectory point
    pos->y[0] = shared_m.trajectory.y[0];
    release_Reader();
    // Import trajectory to local
    while (pos->y[j] != NO_POS && pos->x[j] != NO_POS && j < SEMICFR)      
    {                                               
        j += 1;
        control_Reader();
        pos->x[j] = shared_m.trajectory.x[j];
        pos->y[j] = shared_m.trajectory.y[j];
        release_Reader();    
    }  
}

/* Check Deadline miss */
void check_Deadline()
{
    if (ptask_deadline_miss())
    {
        control_Writer();
        shared_m.ball_d += 1;
        release_Writer();
    }
}

/* Shot task */
ptask shot()
{
    struct postrail_t	local_t;      	// Trajectory local
    int index;                      	// Index of the current Shot task
    int i = 0;                      	// Counter printer trajectory
    int end = 0;                    	// If != 1, the shot task must end
    int	bord, targ, wall = 0;       	// Check collisions return
	int end_main = 0;		            // Check termination main
    int n_shots;

    control_Writer();
    shared_m.shots += 1;
    release_Writer();

    /* Retrieve the number of the shots */
    control_Reader();
    n_shots = shared_m.shots;
    release_Reader();

    index = ptask_get_index();
    printf("Generated ball %d!\n", index);
    
    /* Retrieve trajectory */
    get_trajectory(&local_t);       	

    // Draw ball position till Shot hit the target 
    // or goes outside game's border 
    while (end < 1)    
    {            
        end_main = check_End();  // Check termination
        /* Write Shot positions */
        control_Writer();
        shared_m.pos[index].x = local_t.x[i];     
        shared_m.pos[index].y = local_t.y[i];
        release_Writer();

        /* Collision Checking */
        bord = check_border(local_t.x[i], local_t.y[i], index);
        targ = check_target(local_t.x[i], local_t.y[i], index);
        if (i < SEMICFR)
        {
            wall = check_wall(local_t.x[i + 1], local_t.y[i + 1], index);
        }

        i += 1;
        if (targ == 1)      // Target shoted, update wall position 
        {
            update_wall();
        }
        if (wall == 1)      // Wall collision - Rebounce
        {
            rebounce_wall(&local_t,i);
            wall = 0;
        }
        end = bord + targ + end_main;

        check_Deadline();
        ptask_wait_for_period();
    }

    control_Writer();
    shared_m.pid[n_shots + 2] = -1;
    release_Writer();
}

