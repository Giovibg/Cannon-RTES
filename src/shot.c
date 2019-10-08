#include "manager.h"
#include "ptask.h"

/* Shot task */
ptask shot()
{
    int x = XWIN/2;
    int y = YWIN/2;

    int index;                  // Index of the current Shot task

    index = ptask_get_index();

    printf("Sono la pallina %d!\n", index);

    control_writer();
    shared_m.shots += 1;
    shared_m.pos[index].x = x;
    shared_m.pos[index].y = y;
    release_writer();

    while(1)
    {
        control_writer();
        shared_m.pos[index].x += 1;
        shared_m.pos[index].y += 1;
        release_writer();
        ptask_wait_for_period();
    }
}