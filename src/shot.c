#include "manager.h"
#include "ptask.h"

/*Check shot out of borders */
void check_border(struct pos_t *pos) 
{
    if(pos->x == XWIN - PAD - RADIUS || 
        pos->y == PAD - RADIUS || 
        pos->y == YWIN - PAD - RADIUS)
    {
        pos->x = NO_POS;
        pos->y = NO_POS;
    }
}

/* Check ball hit target */
void check_target(struct pos_t *pos)
{
    struct pos_t target_p;
    control_reader();
    target_p = shared_m.pos_target;     //Get target position. Protected!
    release_reader();
    if (pos->x >= (target_p.x - OFFSET) && 
        pos->y >= (target_p.y - OFFSET))    //Condition ball hit target
    {
        control_writer();
        shared_m.score += 1;    //Update score
        release_writer();
        pos->x = NO_POS;
        pos->y = NO_POS;
    }
}

/* Shot task */
ptask shot()
{
    struct pos_t local_p;                     // Ball local position 
    struct postrail_t local_t[XWIN * YWIN];   // Trajectory local
    int j = 0;
    
    while(shared_m.trajectory[j].x != NO_POS)
    {
        control_reader();
        local_t[j].x = shared_m.trajectory[j].x;
        local_t[j].y = shared_m.trajectory[j].y;
        release_reader();
        printf("Pallina traiettoria X:%f Y:%f\n",local_t[j].x, local_t[j].y);
        j += 1;
        
    }
    
    int index;                  // Index of the current Shot task
    int i = 0;
    index = ptask_get_index();

    printf("Sono la pallina %d!\n", index);

    control_writer();
    shared_m.shots += 1;
    shared_m.pos[index].x = local_t[0].x;
    shared_m.pos[index].y = local_t[0].y;
    release_writer();

    while(local_p.x != NO_POS || local_p.y != NO_POS)   //Draw ball position 
    {                                                   //till in border
        local_p.x = local_t[i].x;
        local_p.y = local_t[i].y;
        
        check_border(&local_p);
        check_target(&local_p);
        control_writer();
        shared_m.pos[index].x = local_p.x; 
        shared_m.pos[index].y = local_p.y;
        release_writer();
        i += 1;
        
        ptask_wait_for_period();
    }
}

