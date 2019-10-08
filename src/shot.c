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
    struct pos_t local_p;      // Ball local position 
    local_p.x = XWIN/2;
    local_p.y = YWIN/2;

    int index;                  // Index of the current Shot task

    index = ptask_get_index();

    printf("Sono la pallina %d!\n", index);

    control_writer();
    shared_m.shots += 1;
    shared_m.pos[index].x = local_p.x;
    shared_m.pos[index].y = local_p.y;
    release_writer();

    while(local_p.x != NO_POS || local_p.y != NO_POS)   //Draw ball position 
    {                                           //till in border
        local_p.x += 1;
        local_p.y += 1;
        
        check_border(&local_p);
        check_target(&local_p);
        control_writer();
        shared_m.pos[index].x = local_p.x; 
        shared_m.pos[index].y = local_p.y;
        release_writer();

        
        ptask_wait_for_period();
    }

    
    

}

