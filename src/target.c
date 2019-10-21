#include "ptask.h"
#include "manager.h"
#include "target.h"

/* Target Task */
ptask target()
{
    int right = 1;                  // Target direction
    int wall_x;                     // X position Wall
    int end = 0;

    struct pos_t    pos_target;     // Target's X Y
    
    pos_target.x = TARGET_X;
    pos_target.y = TARGET_Y;
    
    while (!end)
    {
        end = check_End();
        control_Reader();
        wall_x = shared_m.pos_wall.x;
        release_Reader();
        if (right){
            pos_target.x += 2;
        }
        else{
            pos_target.x -= 2;
        }
        
        if (pos_target.x == MAX_TARGET_R){
            right = 0;
        }
        /* Target will move till wall position */
        if (pos_target.x <= wall_x + OFFSET_WALL){
            right = 1;
        }

        control_Writer(); 
        shared_m.pos_target.x = pos_target.x;
        shared_m.pos_target.y = pos_target.y;
        release_Writer();
        
        /* Check Deadline miss */
        if (ptask_deadline_miss())
        {
            control_Writer();
            shared_m.target_d += 1;
            release_Writer();
        }
        ptask_wait_for_period();
    }
}
