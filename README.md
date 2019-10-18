# Cannon Project

Real Time Embedded System project - Ingegneria Enzo Ferrari

Project 23: Cannon. Simulate a cannon controlled by the user that must shot a   
ball to catch a target that moves slowly on the ground on the other side of the  
wall. Height of the wall randomly generated when target catched.

Developed by Giovanni Bagnoli and Alessio Ruggi

## Installation dependencies
Project is based on graphic library `Allegro 4` and `ptask`.
To install allegro under debian-distro: `sudo apt‐get install liballegro4.4 liballegro4.4‐dev`  

[ptask](https://github.com/glipari/ptask) is a Periodic Real-Time Task interface facility to easily manage tasks.
Based on pthread lib

## Compile and Run

- `git clone` to clone project to local.
- `make all` to remove all compiled files in `/build` directory, then build.
- `make compile` to compile all C files and put compiled files in  `/build`.
- `make run` to compile and run.

Application should be executed as superuser to use ptask.


## Tasks

- `N` ball tasks. Each ball shoted by the cannon is a task, that will follow a   
dedicated trajectory based on cannon angle and power.
- Power bar task. To decide how many power give to the ball at launch  
- Target task. Target should move slowly on the ground  
- Graphic task. To manage interaction of the screen every `PERIOD_G` milliseconds

## Tasks Interactions

Different tasks should interact by themself in a consistent way.
We have a shared memory struct(`shared_m`) were tasks can write or read data.
Consistency and mutual exclusion are guaranteed by mutex and private semaphores.
Conditions are generally managed in a Reader / Writer approach:
- **Readers**: many readers can read from the shared memory at the same time.
- **Writers**: when one writer is writing something, neither reader nor writer
could access the shared mem. 

Each time something should written to `shared_mem`, firstly we need to call  
`control_writer()` function to initialize private semaphore, after we  
could write to shared memory in a protected way, then a `release_writer()` function  
will release private semaphore.
 
Same for readers: `control_reader()` -> read from shared mem ->`release_reader()`

Tasks are executed following a FIFO SCHEDULING

## GamePlay

Game play is based on keyboard interactions. To play, in order you should:
1. Press `SPACE` to start the game.
2. Press `ENTER` to choose power shot intensity - from 1 to 10.
3. Press `ARROW KEY UP` or `ARROW KEY DOWN` to choose cannon degree rotation.
4. Press `ENTER` to shot the ball following trajectory.
<<<<<<< HEAD
5. If target will be catched by ball, wall position will be randomly changed, 
   height incremented.
=======
5. If target will be catched by ball, wall position will be randomly changed,
 height incremented
>>>>>>> develop
6. Press `ESC` to exit the game.


## Code Organization

Code has been organized in different source files:
- Main: Initialize ptask library, initialize allegro and keyboard interactions.
- Manager: Initialize shared memory(`mem_t_init`),  
writer protection(`control_writer`,`release_writer`),   
reader protection(`control_reader`,`release_reader`), initialize tasks param(`init_param`),  
intensity shot bar task(`ptask charge_cannon`), trajectory calculation(`trajectory_cannon`).
- Shot: Invoked after each shot to update ball position following relative trajectory.
- Graphic: All graphic functions. Each thing printed on screen is here.
- Target: `ptask_target` function makes target move slowly.

##Graphic Tasks Interaction
![txt](https://github.com/Giovibg/Cannon-RTES/tree/master/img/Task_Scheme.png)





