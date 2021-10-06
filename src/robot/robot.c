#include <stdio.h>
#include "brick.h"
#include <unistd.h>
#include <stdbool.h>
#include <math.h>

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
float CALIBRATION_TURN = 1/0.5625;

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

float get_gyro(){ //returns gyro reading in degrees
    return sensor_get_value0(SENSOR_GYRO,0);
}

void drive(float dist){
    tacho_reset(MOTOR_BOTH);
    
    float speed = tacho_get_max_speed(MOTOR_LEFT,0)*0.5;


    tacho_set_ramp_up_sp(MOTOR_BOTH, 3000);
    tacho_set_ramp_down_sp(MOTOR_BOTH, 3000);

    tacho_set_position_sp(MOTOR_BOTH,dist*CALIBRATION_DRIVE_DISTANCE);

    tacho_set_speed_sp(MOTOR_BOTH, speed);

    tacho_run_to_rel_pos(MOTOR_BOTH);
    
    sleep(6);

    for(;tacho_is_running(MOTOR_BOTH);){}
}

void turn_gyro(float deg){
    tacho_reset(MOTOR_BOTH);

    float sgn = (deg < 0)? -1 : 1;
    float speed = tacho_get_max_speed(MOTOR_LEFT,0)*0.03;

    float gyro_start = get_gyro();

    tacho_set_stop_action_hold(MOTOR_BOTH);

    tacho_set_speed_sp(MOTOR_LEFT, sgn*speed);
    tacho_set_speed_sp(MOTOR_RIGHT, -sgn*speed);

    tacho_run_forever(MOTOR_BOTH);

    for( ; sgn*get_gyro() < sgn*(gyro_start+deg) ; ){}
}

<<<<<<< HEAD
void touch_align(){
    int max_speed = tacho_get_max_speed( MOTOR_LEFT, 0 ); 
    
    tacho_set_speed_sp(MOTOR_BOTH, max_speed * 0.5); 

    tacho_run_forever(MOTOR_BOTH);
=======
void turn_gyro(float angle){
	float sgn = (angle < 0 ? 1 : -1);
	float first_pos_gyro = get_gyro();
	int turn_speed = 0.1 * tacho_get_max_speed( MOTOR_LEFT, 0 );
	
	tacho_set_speed_sp(MOTOR_RIGHT, -sgn * turn_speed);
	tacho_set_speed_sp(MOTOR_LEFT, sgn * turn_speed);
	
	tacho_run_forever(MOTOR_BOTH);
	
	for(;sgn * fabs(get_gyro()) <= sgn * first_pos_gyro;){}
	
	tacho_stop( MOTOR_BOTH );
}
>>>>>>> 5de616f4c568ab81dc236295c40ead713a09aff5

    while(!get_touch()){
    } 

    tacho_stop(MOTOR_BOTH);
    
    tacho_set_speed_sp(MOTOR_LEFT, max_speed * 0.6); 
    tacho_set_speed_sp(MOTOR_RIGHT, max_speed * 0.4);

    tacho_run_forever(MOTOR_BOTH);
    
    sleep(2);

    //tacho_stop(MOTOR_BOTH);

    tacho_set_speed_sp(MOTOR_LEFT, max_speed * 0.4); 
    tacho_set_speed_sp(MOTOR_RIGHT, max_speed * 0.6);
    tacho_run_forever(MOTOR_BOTH);

    sleep(2);

    tacho_set_speed_sp(MOTOR_BOTH, max_speed * 0.5); 
    tacho_run_forever(MOTOR_BOTH);

    sleep(1);

    tacho_stop(MOTOR_BOTH);
}


void face_wall(){}

void follow_wall(){
    tacho_reset(MOTOR_BOTH);

    float goal_distance = get_distance();
    float fundamental_speed = tacho_get_max_speed(MOTOR_LEFT,0)*0.2;

    tacho_set_ramp_up_sp(MOTOR_BOTH, 5000);
    tacho_set_ramp_down_sp(MOTOR_BOTH, 5000);

    tacho_set_speed_sp(MOTOR_BOTH, fundamental_speed);
    tacho_run_forever(MOTOR_BOTH);

    for (;;){
        float current_distance = get_distance();
        float steer = powf((current_distance - goal_distance)*0.03, 2); 
        steer = (steer < 1) ? steer : 1; /*clamp below or eq to 1*/
        float steer_amount = 0.3;

        if(current_distance < goal_distance){
            printf("RIGHT\n");
            tacho_set_speed_sp(MOTOR_LEFT, fundamental_speed + fundamental_speed * steer_amount * steer);
            tacho_set_speed_sp(MOTOR_RIGHT, fundamental_speed - fundamental_speed * steer_amount * steer);
        } else {
            printf("LEFT\n");
            tacho_set_speed_sp(MOTOR_RIGHT, fundamental_speed + steer_amount*fundamental_speed*steer_amount*steer);
            tacho_set_speed_sp(MOTOR_LEFT, fundamental_speed - steer_amount*fundamental_speed*steer_amount*steer);
        }

        tacho_run_forever(MOTOR_BOTH);
        printf("%f\n\n", steer);
    }
}

void follow_wall_pid(float kp, float ki, float kd){
    tacho_reset(MOTOR_BOTH);

    float target_distance = get_distance();
    float fundamental_speed = tacho_get_max_speed(MOTOR_LEFT,0)*0.2;

    tacho_set_speed_sp(MOTOR_BOTH, fundamental_speed);
    tacho_run_forever(MOTOR_BOTH);

    float proportional = 0;
    float integral = 0;
    float derivative = 0;

    for (;;){
        
        float new_error = (get_distance() - target_distance);

        proportional = (new_error)*kp;
        integral = (integral+new_error)*ki;
        derivative = (proportional - new_error)*kd;

        float steer = proportional*kp + integral*ki + derivative*kd;
        steer = steer < 1 ? steer : 1;
        steer = steer > -1 ? steer : -1;

        tacho_set_speed_sp(MOTOR_LEFT, fundamental_speed + fundamental_speed * steer);
        tacho_set_speed_sp(MOTOR_RIGHT, fundamental_speed - fundamental_speed * steer);

        tacho_run_forever(MOTOR_BOTH);
        printf("p: %f\n", proportional);
        printf("i: %f\n", integral);
        printf("d: %f\n", derivative);
        printf("steer: %f\n\n", steer);
    }
}

void drop(){}

int main(){
    if (!init()) return ( 1 ); 

    follow_wall_pid(0.05,0.05,0.05);

}