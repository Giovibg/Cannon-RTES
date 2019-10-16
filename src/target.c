#include "ptask.h"
#include "manager.h"
#include "target.h"
/* Target Task */
ptask target()
{
    int right = 1;
    struct pos_t pos_target;
    pos_target.x = TARGET_X;
    pos_target.y = TARGET_Y;
    int wall_x;                 //X position Wall
    while(1)
    {
        control_reader();
        wall_x = shared_m.pos_wall.x;
        release_reader();
        if(right) 
        {
            pos_target.x += 2;
        }
        else
        {
            pos_target.x -= 2;
        }
        
        if(pos_target.x == MAX_TARGET_R)
        {
            right = 0;
        }

        if(pos_target.x <= wall_x + OFFSET_WALL)
        {
            right = 1;
        }
        control_writer();
        shared_m.pos_target.x = pos_target.x;
        shared_m.pos_target.y = pos_target.y;
        release_writer();
        /* Check Deadline miss */
        if(ptask_deadline_miss())
        {
            control_writer();
            shared_m.target_d += 1;
            release_writer();
        }
        ptask_wait_for_period();
    }
}