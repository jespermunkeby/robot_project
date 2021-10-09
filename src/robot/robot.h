#ifndef robot_h_
#define robot_h_

#include <stdio.h>
#include "brick.h"
#include <unistd.h>
#include <stdbool.h>
#include <math.h>

int init();
float get_distance();
bool get_touch();
float get_gyro();
void drive(float dist);
void turn(float deg);
void touch_align();
void find_wall();
void follow_wall_pid(float dist);
void drop();

#endif
