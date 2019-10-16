#include "manager.h"
#include "ptask.h"
#include "shot.h"
#include <stdlib.h>
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
        control_writer();
        shared_m.pos[index].x = NO_POS;     //Delete ball
        shared_m.pos[index].y = NO_POS;
        release_writer();

        ret = 1;
    }
    return ret;
}

/* Shot task */
ptask shot()
{
    struct postrail_t local_t;   // Trajectory local
    int index;                  // Index of the current Shot task
    int i, j = 0;               // tmp var
    int end = 0;                    // If == -1, the shot task must end
    int bord, targ, wall = 0;
    int score = 0;                  // Retrieve score
    int x_dir = 0;
    struct pos_t wall_p;               //wall position to update
    index = ptask_get_index();
    printf("Sono la pallina %d!\n", index);

    control_reader();
    local_t.x[0] = shared_m.trajectory.x[0];
    local_t.y[0] = shared_m.trajectory.y[0];
    release_reader();
    while(local_t.y[j] != NO_POS && local_t.x[j] != NO_POS && j < SEMICFR)      //Import trajectory to local
    {
        j += 1;
        control_reader();
        local_t.x[j] = shared_m.trajectory.x[j];
        local_t.y[j] = shared_m.trajectory.y[j];
        release_reader();
        // printf("Pallina traiettoria X:%f Y:%f\n",local_t[j].x, local_t[j].y);    
    }  

    control_writer();
    shared_m.shots += 1;
    //shared_m.pos[index].x = local_t[0].x;
    //shared_m.pos[index].y = local_t[0].y;
    release_writer();

    i = 0;
    while(end != 1)     //Draw ball position 
    {                   //till in border
        control_writer();
        shared_m.pos[index].x = local_t.x[i];     //Write ball position
        shared_m.pos[index].y = local_t.y[i];
        release_writer();
        // printf("Y:%d\n",local_t.y[i]);
        bord = check_border(local_t.x[i], local_t.y[i], index);
        targ = check_target(local_t.x[i], local_t.y[i], index);
        wall = check_wall(local_t.x[i], local_t.y[i], index);
        end = bord + wall + targ;
        
        i += 1;
        if(targ == 1)
        {
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
        /* Check Deadline miss */
        if(ptask_deadline_miss())
        {
            control_writer();
            shared_m.ball_d += 1;
            release_writer();
        }
        ptask_wait_for_period();
    }
    // printf("out i: %d\n", local_t.y[i]);
}

