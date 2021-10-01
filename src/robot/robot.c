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

int init(){
    if (! brick_init()) return ( 1 );
    if (! sensor_is_plugged(SENSOR_TOUCH, LEGO_EV3_TOUCH)){
        printf("Could not connect to SENSOR_TOUCH at socket IN1. Check if plugged in correctly");
        return(0);
    }
    if (! sensor_is_plugged(SENSOR_DISTANCE, LEGO_EV3_US)){
        printf("Could not connect to SENSOR_DISTANCE at socket IN2. Check if plugged in correctly");
        return(0);
    } 
    if (! sensor_is_plugged(SENSOR_COLOR, LEGO_EV3_COLOR)){
        printf("Could not connect to SENSOR_COLOR at socket IN3. Check if plugged in correctly");
        return(0);
    }
    if (! sensor_is_plugged(SENSOR_GYRO, LEGO_EV3_GYRO)){
        printf("Could not connect to SENSOR_GYRO at socket IN4. Check if plugged in correctly");
        return(0);
    }

    return(1);

}


float get_distance(){ //returns distance read from ultrasonic sensors in mm
    return sensor_get_value0(SENSOR_DISTANCE,0);
}

bool get_touch(){ //returns true if toch sensor is pressed, false if not
    if(sensor_get_value0(SENSOR_TOUCH,0)){
        return true;
    } else {
        return false;
    }    
}

float get_gyro(){ //returns gyro reading in radians
    return sensor_get_value0(SENSOR_GYRO,0);
}

void drive(float dist){}

void turn(){}

void touch_align(){}

void face_wall(){

}

void follow_wall(){

}

void drop(){}

int main(){
    if (!init()) return ( 1 ); 
    
    while(true){
        printf("\e[1;1H\e[2J");
        printf("distance: %f \n", get_distance());
        printf("gyro: %f \n", get_gyro());
        printf("touch: %d \n", get_touch());
    }
}