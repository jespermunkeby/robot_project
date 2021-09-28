#include <stdio.h>
#include "brick.h"
#include <unistd.h>
#include <stdbool.h>

#define MOTOR_RIGHT     OUTA
#define MOTOR_LEFT      OUTB
#define MOTOR_FORK      OUTC
#define SENSOR_TOUCH    IN1
#define SENSOR_DISTANCE IN2
#define SENSOR_COLOR    IN3
#define SENSOR_GYRO     IN4
#define MOTOR_BOTH      ( MOTOR_LEFT | MOTOR_RIGHT ) /* Bitvis ELLER ger att båda motorerna styrs samtidigt */


float get_distance(){ //returns distance read from ultrasonic sensors in mm
    if(sensor_is_plugged(SENSOR_DISTANCE, LEGO_EV3_US)){
        return sensor_get_value0(SENSOR_DISTANCE,0);
    }

    printf("Could not connect to SENSOR_DISTANCE at socket IN2. Check if plugged in correctly");
    return 0.;
}

bool get_touch(){ //returns true if toch sensor is pressed, false if not

    if(sensor_is_plugged(SENSOR_TOUCH, LEGO_EV3_TOUCH)){
        if(sensor_get_value0(SENSOR_TOUCH,0)){
            return true;
        } else {
            return false;
        }
    }

    printf("Could not connect to SENSOR_TOUCH at socket IN1. Check if plugged in correctly");
    return 0.;
}

float get_gyro(){ //returns gyro reading in radians
    if(sensor_is_plugged(SENSOR_GYRO, LEGO_EV3_GYRO)){
        return sensor_get_value0(SENSOR_GYRO,0);
    }

    printf("Could not connect to SENSOR_GYRO at socket IN4. Check if plugged in correctly");
    return 0.;
}

void drive(float dist){}

void turn(){}

void touch_align(){}

void face_wall(){}

void follow_wall(){}

void drop(){}

int main(){
    if ( !brick_init()) return ( 1 ); 
    
    while(true){
        printf("distance: %f \n", get_distance());
        /* printf("gyro: %f \n", get_gyro());
        printf("touch: %d \n", int get_touch());*/
    }
}