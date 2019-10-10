#include "ptask.h"
#include "manager.h"

/* Target Task */
ptask target()
{
    int right = 1;
    struct pos_t pos_target;
    pos_target.x = TARGET_X;
    pos_target.y = TARGET_Y;

    while(1)
    {
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

        if(pos_target.x == MAX_TARGET_L)
        {
            right = 1;
        }

        control_writer();
        shared_m.pos_target.x = pos_target.x;
        shared_m.pos_target.y = pos_target.y;
        release_writer();

        //sleep(1 );
        for(int j=0;j<100000000;j++);
    }
}