#include "ptask.h"

/* Shot task */
ptask shot();
/*Check shot out of borders */
int check_border(int x, int y, int index);
/* Check ball hit target */
int check_target(int x, int y, int index);
/* Check wall collision */
int check_wall(int x, int y, int index);
