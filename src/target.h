#include "ptask.h"

//-------------------------------------------------------------
// FUNCTIONS
//-------------------------------------------------------------

/* 
 * Decide movement: left or right 
 * if right == 1, move right; else, left.
 * struct pos_t *pos_target -> struct that represent X and Y Target's position
 */
void check_LRShift(struct pos_t *pos_target, int *right);

/* 
 * Target Task. Target moves between wall position and right playground border  
 */
ptask target();