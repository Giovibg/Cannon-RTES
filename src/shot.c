#include "manager.h"
#include "ptask.h"

/* Shot task */
ptask shot()
{
    printf("Sono una pallina!\n");

    control_writer();
    shared_m.shots += 1;
    release_writer();

    while(1)
    {
        ptask_wait_for_period();
    }
}