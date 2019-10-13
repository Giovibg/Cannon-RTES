#ifndef MAIN_H
#define MAIN_H

//-------------------------------------------------------------
// GLOBAL CONSTANTS
//-------------------------------------------------------------
// Period manager task
#define PERIOD_M 10
// Priority manager task
#define PRIO_M 10
// Max value for power shot
#define MAX_SHOT 10
// PIGRECO
#define PIGRECO 3.1415926535

//-------------------------------------------------------------
// FUNCTIONS
//-------------------------------------------------------------
// Set if trajectory should be updated. Protected!
// Bool can be 0 or 1
// 0 -> Don't update trajectory
// 1 -> Update trajectory
void set_UpdateTrajectory(int bool);

// Update Cannon Degree. Protected!
void update_CannonDegree(int cannon_degree);

// Phase where the user can set the Cannon angle
// int shot_pwr -> It's the power of the shot
int set_CannonAngle(int shot_pwr);

// End the Charge cannon phase and return the power of the shot
int get_CannonPwr();

// Create the cannon task and set the shared memory variable to the correct value
int set_CannonPwr();

#endif