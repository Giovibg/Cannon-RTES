#include "manager.h"
#include "ptask.h"
#include "shot.h"
#include <stdlib.h>

/* Wall collision rebounce */
void rebounce_wall(struct postrail_t *reb, int i)
{
    int j;
    int wall_x;
    int dist_xwallx;
    int shift = 1;
    control_reader();
    wall_x = shared_m.pos_wall.x;
    release_reader();
    j = i;
    
    while(reb->x[j] != NO_POS)
    {
        dist_xwallx = reb->x[j] - wall_x;
        reb->x[j] = (wall_x - dist_xwallx);
        reb->y[j] += (shift * 0.05); 
        j += 1;
        shift += 1;
    }
}
/*Check shot out of borders */
int check_border(int x, int y, int index) 
{
    int ret = 0;

    if(x >= XWIN - PAD - RADIUS  ||
        y <= PAD + RADIUS || 
        y >= YWIN - PAD - RADIUS)
    {
        control_writer();
        shared_m.pos[index].x = NO_POS;     //Delete ball
        shared_m.pos[index].y = NO_POS;
        release_writer();
        ret = 1;
    }
    return ret;
}

/* Check ball hit target */
int check_target(int x, int y, int index)
{
    int ret = 0;
    struct pos_t target_p;
    control_reader();
    target_p.x = shared_m.pos_target.x;     //Get target position. Protected!
    target_p.y = shared_m.pos_target.y;
    release_reader();
    if (x >= (target_p.x - RADIUS) && 
        y >= (target_p.y - OFFSET) && (x <= target_p.x + 8*OFFSET))    //Condition ball hit target
    {
        control_writer();
        shared_m.score += 1;                //Update score
        shared_m.pos[index].x = NO_POS;     //Delete ball
        shared_m.pos[index].y = NO_POS;
        release_writer();
        

        ret = 1;
    }
    return ret;
}
/* Check Wall collision */
int check_wall(int x, int y, int index)
{
    int ret = 0;
    struct pos_t wall_p;
    control_reader();
    wall_p.x = shared_m.pos_wall.x;     //Get target position. Protected!
    wall_p.y = shared_m.pos_wall.y;
    release_reader();
    if((x >= wall_p.x - RADIUS) && (x <= wall_p.x + WALL_W) && (y >= wall_p.y))
    {

        /*control_writer();
        shared_m.pos[index].x = NO_POS;     //Delete ball
        shared_m.pos[index].y = NO_POS;
        release_writer();
        */

        ret = 1;
    }
    return ret;
}

void update_wall()
{
    int score = 0;                      // Retrieve score
    int x_dir = 0;
    struct pos_t wall_p;                //wall position to update
    control_reader();
    score = shared_m.score;
    wall_p.x = shared_m.pos_wall.x;
    wall_p.y = shared_m.pos_wall.y;
    release_reader();

    if (((wall_p.x % wall_p.y) % 2) == 0)
    {
        x_dir = 1;
    }
    else
    {
        x_dir = -1;
    }
            
    control_writer();
    shared_m.pos_wall.y = wall_p.y - score * OFFSET - (rand()%10);
    shared_m.pos_wall.x = wall_p.x - x_dir * score * OFFSET - x_dir * (rand()%60 + 1);
    release_writer();
}

void get_trajectory(struct postrail_t *pos)
{
    int j = 0;
    control_reader();
    pos->x[0] = shared_m.trajectory.x[0]; //First trajectory point
    pos->y[0] = shared_m.trajectory.y[0];
    release_reader();
    //Import trajectory to local
    while(pos->y[j] != NO_POS && pos->x[j] != NO_POS && j < SEMICFR)      
    {                                               
        j += 1;
        control_reader();
        pos->x[j] = shared_m.trajectory.x[j];
        pos->y[j] = shared_m.trajectory.y[j];
        release_reader();    
    }  
}

/* Shot task */
ptask shot()
{
    struct postrail_t local_t;      // Trajectory local
    int index;                      // Index of the current Shot task
    int i = 0;                      // Counter printer trajectory
    int end = 0;                    // If == -1, the shot task must end
    int bord, targ, wall = 0;       // Check collisions return
    index = ptask_get_index();
    printf("Sono la pallina %d!\n", index);

    get_trajectory(&local_t);       /* Retrieve trajectory */

    control_writer();
    shared_m.shots += 1;
    //shared_m.pos[index].x = local_t[0].x;
    //shared_m.pos[index].y = local_t[0].y;
    release_writer();

    i = 0;
    while(end != 1)         //Draw ball position till in border
    {                   
        control_writer();
        shared_m.pos[index].x = local_t.x[i];     //Write ball position
        shared_m.pos[index].y = local_t.y[i];
        release_writer();
        // Collision Checking
        bord = check_border(local_t.x[i], local_t.y[i], index);
        targ = check_target(local_t.x[i], local_t.y[i], index);
        wall = check_wall(local_t.x[i], local_t.y[i], index);
        
        i += 1;
        if(targ == 1)       // Target shoted, update wall position 
        {
            update_wall();
        }
        if(wall == 1)       //Wall collision - Rebounce
        {
            rebounce_wall(&local_t,i);
            wall = 0;
        }
        end = bord + targ;

        /* Check Deadline miss */
        if(ptask_deadline_miss())
        {
            control_writer();
            shared_m.ball_d += 1;
            release_writer();
        }
        ptask_wait_for_period();
    }
}

