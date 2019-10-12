# Cannon Project

Real Time Embedded System project - Ingegneria Enzo Ferrari

Project 23: Cannon. Simulate a cannon controlled by the user that must shot a 
ball to catch a target that moves slowly on the ground on the other side of the
wall. Height of the wall randomly generated when target catched 

Developed by Giovanni Bagnoli and Alessio Ruggi

## Installation dependencies
Project is based on graphic library `Allegro 4` and `ptask`
To install allegro under debian-distro: sudo apt‐get install liballegro4.4 liballegro4.4‐dev

`ptask` is a Periodic Real-Time Task interface facility to easily manage tasks.
Based on pthread lib

## Compile and Run

- `git clone` to clone project to local
- `make all` to remove all compiled files in `/build` directory, then build
- `make compile` to compile all C files and put compiled files in  `/build`
- `make run` to compile and run

Application should be executed as superuser to use ptask


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
Conditions are generally managed in a Readers / Writers way:
- Readers: many readers can read from the shared memory at the same time.
- Writers: when one writer is writing something, nobody could read access to the
 shared mem. 

Each time someone should write to `shared_mem`, before access a first function
`control_writer()` should be called to initialize private semaphore, after we 
could to shared memory in a protected way, then a `release_writer()` function 
will release private semaphore. 
Same for readers: `control_reader()` -> read from shared mem ->`release_reader()`

Tasks are executed following a FIFO SCHEDULING

## GamePlay

Game play is based on keyboard interactions. To play, in order you should:
1. Press `SPACE` to start the game.
2. Press `ENTER` to choose power shot intensity - from 1 to 10.
3. Press `ARROW KEY UP` or `ARROW KEY DOWN` to choose cannon degree rotation.
4. Press `ENTER` to shot the ball following trajectory.
5. If target will be catched by ball, wall position will be randomly changed.

Pressing `T` button, a shot trajectory, calculated at the specific intensity
and cannon degree choosen, will be shown.



