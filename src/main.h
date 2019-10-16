#ifndef MAIN_H
#define MAIN_H

//-------------------------------------------------------------
// GLOBAL CONSTANTS
//-------------------------------------------------------------

// Max value for power shot
#define MAX_SHOT 10

//-------------------------------------------------------------
// FUNCTIONS
//-------------------------------------------------------------

/* Set if trajectory should be updated. Protected!
 * int bool can be 0 or 1
 * 0 -> Don't update trajectory
 * 1 -> Update trajectory
 */
void set_UpdateTrajectory(int bool);

/*
 * Update Cannon Degree. Protected!
 * int cannon_degree -> the degree of the cannon
 */
void update_CannonDegree(int cannon_degree);

/* Phase where the user can set the Cannon angle
 * int shot_pwr -> It's the power of the shot
 */
int set_CannonAngle(int shot_pwr);

/*
 * End the Charge cannon phase and return the power of the shot
 */
int get_CannonPwr();

/* 
 * Create the cannon task and set the shared memory variable to the correct value
 */
int set_CannonPwr();

#endif