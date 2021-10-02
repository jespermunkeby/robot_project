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

float CALIBRATION_DRIVE_DISTANCE = 2;
float CALIBRATION_TRIM = 0;
float CALIBRATION_TURN = 1;

int init(){
    if (! brick_init()) return ( 0 );
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

void drive(float dist){
    tacho_set_position_sp(MOTOR_BOTH,dist*CALIBRATION_DRIVE_DISTANCE);
    tacho_set_speed_sp(MOTOR_BOTH, tacho_get_max_speed(MOTOR_LEFT,0)*0.5);
    tacho_run_to_rel_pos(MOTOR_BOTH);
}

void await_drive(){
    
}

void turn_gyro(){}


void turn_tacho(float deg){
    tacho_set_position_sp(MOTOR_LEFT,deg*CALIBRATION_TURN);
    tacho_set_position_sp(MOTOR_RIGHT,-deg*CALIBRATION_TURN);
    tacho_set_speed_sp(MOTOR_BOTH, tacho_get_max_speed(MOTOR_LEFT,0)*0.2);
    tacho_run_to_rel_pos(MOTOR_BOTH);
}

void touch_align(){}


void face_wall(){
    int smallest_element_index(float arr[], int length){
        int index = 0;
        int i;
        for( i = 1; i < length; i++){
            if(arr[i] < arr[index])
                index = i;
        }
        return index;
    }

}

void follow_wall(){

}

void drop(){}

int main(){
    if (!init()) return ( 1 ); 
    turn_tacho(360*10);
    sleep(60);
}