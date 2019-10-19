#include "ptask.h"

/* Shot task */
ptask shot();
/* Wall collision rebounce */
void rebounce_wall(struct postrail_t *reb, int i);
/*Check shot out of borders */
int check_border(int x, int y, int index);
/* Check ball hit target */
int check_target(int x, int y, int index);
/* Check wall collision */
int check_wall(int x, int y, int index);
/* Update wall position */
void update_wall();
/* Retrieve trajectory */
void get_trajectory(struct postrail_t *pos);
/* Check Deadline miss */
void check_deadline();
