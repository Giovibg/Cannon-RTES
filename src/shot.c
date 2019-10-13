#include "manager.h"
#include "ptask.h"

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
    target_p = shared_m.pos_target;     //Get target position. Protected!
    release_reader();
    if (x >= (target_p.x - OFFSET) && 
        y >= (target_p.y - OFFSET))    //Condition ball hit target
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

/* Shot task */
ptask shot()
{
    struct postrail_t local_t;   // Trajectory local
    int index;                  // Index of the current Shot task
    int i, j = 0;               // tmp var
    int end = 0;                    // If == -1, the shot task must end
    int bord, targ, wall = 0;

    index = ptask_get_index();
    printf("Sono la pallina %d!\n", index);

    control_reader();
    local_t.x[0] = shared_m.trajectory.x[0];
    local_t.y[0] = shared_m.trajectory.y[0];
    release_reader();
    while(local_t.x[j] != NO_POS)      //Import trajectory to local
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

        end = bord + wall + targ;
        
        i += 1;
        
        ptask_wait_for_period();
    }
    // printf("out i: %d\n", local_t.y[i]);
}

