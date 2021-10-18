#include <stdio.h>
#include "brick.h"
#include <unistd.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>

#define MOTOR_RIGHT     OUTA
#define MOTOR_LEFT      OUTB
#define MOTOR_FORK      OUTC
#define SENSOR_TOUCH    IN1
#define SENSOR_GYRO     IN2
#define SENSOR_COLOR    IN3
#define SENSOR_DISTANCE IN4

#define MOTOR_BOTH      ( MOTOR_LEFT | MOTOR_RIGHT ) /* Bitvis ELLER ger att båda motorerna styrs samtidigt */

float CALIBRATION_DRIVE_DISTANCE = 2;
float CALIBRATION_TRIM = 0;
float CALIBRATION_TURN = 1/0.5625;

//Jesper Munkeby
int init(){
    if (! brick_init()) return ( 0 );
    if (! sensor_is_plugged(SENSOR_TOUCH, LEGO_EV3_TOUCH)){
        printf("Could not connect to SENSOR_TOUCH at socket IN1. Check if plugged in correctly");
        return(0);
    }
    if (! sensor_is_plugged(SENSOR_DISTANCE, LEGO_EV3_US)){
        printf("Could not connect to SENSOR_DISTANCE at socket IN4. Check if plugged in correctly");
        return(0);
    } 
    if (! sensor_is_plugged(SENSOR_COLOR, LEGO_EV3_COLOR)){
        printf("Could not connect to SENSOR_COLOR at socket IN3. Check if plugged in correctly");
        return(0);
    }
    if (! sensor_is_plugged(SENSOR_GYRO, LEGO_EV3_GYRO)){
        printf("Could not connect to SENSOR_GYRO at socket IN2. Check if plugged in correctly");
        return(0);
    }

    gyro_set_mode_gyro_g_and_a(SENSOR_GYRO);

    return(1);
}


//Jesper Munkeby
float get_distance(){ /*returns distance read from ultrasonic sensors in mm*/
    return sensor_get_value0(SENSOR_DISTANCE,0);
}

//Jesper Munkeby
bool get_touch(){ /*returns true if toch sensor is pressed, false if notm*/
    if(sensor_get_value0(SENSOR_TOUCH,0)){
        return true;
    } else {
        return false;
    }    
}

//Jesper Munkeby
float get_gyro(){ /*returns gyro reading in degrees*/
    return sensor_get_value0(SENSOR_GYRO,0);
}

//Jesper Munkeby
float get_gyro_speed(){ /*returns gyro reading of speed in degreed/second */
    return sensor_get_value1(SENSOR_GYRO,0);
}

//Jesper Munkeby
void drive(float dist){
	printf("Starts drive\n");
	printf("Resets motors\n");
    tacho_reset(MOTOR_BOTH);
    
    float speed = tacho_get_max_speed(MOTOR_LEFT,0)*0.2;
	
    tacho_set_position_sp(MOTOR_BOTH,dist*CALIBRATION_DRIVE_DISTANCE);

    tacho_set_speed_sp(MOTOR_BOTH, speed);

    tacho_run_to_rel_pos(MOTOR_BOTH);
	
	printf("Start motor\n");
    for(;tacho_is_running(MOTOR_BOTH);){}
	printf("End drive\n");
}

//Jesper Munkeby, Branko Iljic
void turn(float deg){
	printf("Starts turn\n");
	printf("Reset motors\n");
    tacho_reset(MOTOR_BOTH);

    float sgn = (deg < 0)? -1 : 1;
    float speed = tacho_get_max_speed(MOTOR_LEFT,0)*0.05;

    float gyro_start = get_gyro();

    tacho_set_stop_action_hold(MOTOR_BOTH);

    tacho_set_speed_sp(MOTOR_LEFT, sgn*speed);
    tacho_set_speed_sp(MOTOR_RIGHT, -sgn*speed);
	
	printf("Starts turning\n");
    tacho_run_forever(MOTOR_BOTH);

    for( ; sgn*get_gyro() < sgn*(gyro_start+deg) ; ){
    }
	printf("End turn\n");
}

//Adrian Kastrati
void drop_rel_pos(){
	printf("Starts drop_rel_pos \n");
	printf("Resets motors\n");
    tacho_reset(MOTOR_FORK);
	
    int max_speed_fork = tacho_get_max_speed( MOTOR_FORK, 0 ) * 0.6;    
    tacho_set_stop_action_hold(MOTOR_FORK);
 
        
    tacho_set_position_sp(MOTOR_FORK, 250);
    tacho_set_speed_sp(MOTOR_FORK, max_speed_fork);
    tacho_run_to_rel_pos(MOTOR_FORK);  
	
	printf("Sleep 1 second\n");
    sleep(1);
	
	printf("Drops...\n");
    tacho_set_position_sp(MOTOR_FORK, -250);    
    tacho_set_speed_sp(MOTOR_FORK, max_speed_fork);
    tacho_run_to_rel_pos(MOTOR_FORK);
	printf("End drop rel pos\n");
}

//Adrian Kastrati
void touch_align(){
	printf("Start touch align\n");
	printf("Resets motors\n");
    tacho_reset(MOTOR_BOTH);
    int max_speed = tacho_get_max_speed( MOTOR_LEFT, 0 ); 
    
    tacho_set_speed_sp(MOTOR_BOTH, max_speed * 0.3); 
	
	printf("Starts motors\n");
    tacho_run_forever(MOTOR_BOTH);

    while(!get_touch()){} 

	printf("Found touch, stop motors\n");
    tacho_stop(MOTOR_BOTH);
    
    tacho_set_speed_sp(MOTOR_LEFT, max_speed * 0.4); 
    tacho_set_speed_sp(MOTOR_RIGHT, max_speed * 0.1);
	
	printf("Run motors left faster\n");
    tacho_run_forever(MOTOR_BOTH);

	printf("Sleeps 2 seconds\n");
    sleep(2);
    
	printf("Run motor right faster\n");
    tacho_set_speed_sp(MOTOR_LEFT, max_speed * 0.1); 
    tacho_set_speed_sp(MOTOR_RIGHT, max_speed * 0.4);
    tacho_run_forever(MOTOR_BOTH);
	
	printf("Sleeps 2 seconds\n");
    sleep(2);

	printf("Run both motors same speed\n");
    tacho_set_speed_sp(MOTOR_BOTH, max_speed * 0.5); 
    tacho_run_forever(MOTOR_BOTH);
	
	printf("Sleep 1 seconds\n");
    sleep(1);

	printf("Stops motors\n");
    tacho_stop(MOTOR_BOTH);
	printf("End touch align\n");
}

//Adrian Kastrati, Branko Iljic
void find_wall(){ 
	printf("Start find wall\n");
	printf("Resets motors\n");
    tacho_reset(MOTOR_BOTH);

    float speed = tacho_get_max_speed(MOTOR_LEFT,0) * 0.05;

    float gyro_start = get_gyro();

    tacho_set_stop_action_hold(MOTOR_BOTH);

    tacho_set_speed_sp(MOTOR_LEFT, speed);
    tacho_set_speed_sp(MOTOR_RIGHT, -speed);
	
	printf("Turn motors\n");
    tacho_run_forever(MOTOR_BOTH);

    float closest_postition_gyro;
    float closest_distance_wall = get_distance();
    

    for( ; get_gyro() < (gyro_start + 360) ; ){
        
        float test_dist = get_distance();

        if(test_dist < closest_distance_wall){
            closest_postition_gyro = get_gyro();
            closest_distance_wall = test_dist;
			/*printf("Shortest distance: %f\nSmallest angle: %f\n", closest_distance_wall, closest_postition_gyro);*/
        }
    }
    
    int delta_gyro = closest_postition_gyro - gyro_start;

    if(delta_gyro > 180){
        delta_gyro = delta_gyro - 360;
		printf("Turns %f degrees counter clockwise\n", delta_gyro);
    }
	else
		printf("Turn %f degrees clockwise\n", delta_gyro);
	
    turn(delta_gyro);
	printf("Ends find wall\n");
}

//Jesper Munkeby
void follow_wall_pid(float dist){
	printf("Starts follow wall\n");
    float kp = 0.06;
    float ki = 0;
    float kd = 1;
	
	printf("Resets motor\n");
    tacho_reset(MOTOR_BOTH);

    float target_distance = get_distance();
    float sgn = (dist < 0)? -1 : 1;
    float fundamental_speed = tacho_get_max_speed(MOTOR_LEFT,0)*0.35*sgn;

	printf("Starts motors\n");
    tacho_set_speed_sp(MOTOR_BOTH, fundamental_speed);
    tacho_run_forever(MOTOR_BOTH);

    float proportional = 0;
    float integral = 0;
    float derivative = 0;

    float start_tacho = (tacho_get_position(MOTOR_LEFT, 0) + tacho_get_position(MOTOR_RIGHT, 0))/2;
    for (;;){
        
        float new_error = 0.01*(target_distance - get_distance());

        derivative = (new_error-proportional);
        proportional = new_error;
        integral = integral + new_error;
        

        float steer = proportional*kp + integral*ki + derivative*kd;
        steer = steer < 1 ? steer : 1;
        steer = steer > -1 ? steer : -1;

        tacho_set_speed_sp(MOTOR_LEFT, fundamental_speed + fundamental_speed * steer);
        tacho_set_speed_sp(MOTOR_RIGHT, fundamental_speed - fundamental_speed * steer);

        tacho_run_forever(MOTOR_BOTH);

        if(fabs((tacho_get_position(MOTOR_LEFT, 0) + tacho_get_position(MOTOR_RIGHT, 0))/2) > (start_tacho + fabs(dist*CALIBRATION_DRIVE_DISTANCE))){
            break;
        }

    }
    tacho_stop(MOTOR_BOTH);
	printf("Ends follow wall\n");
}

/*--------*/
/*Hela den följande delen itererades mycket på och vi skrev den tillsammans*/

void setup_orient(){
    find_wall();
    turn(-90);
    touch_align();
    drive(-500);
}

int HALL_WIDTH = 2000;

void mission_1(){
    setup_orient();
    turn(90);
    follow_wall_pid(2500);
    turn(-90);
    drive(150);
    drop_rel_pos();

    /*tillbaka*/
    drive(-150);
    turn(90);
    follow_wall_pid(-2500);
    turn(-90);
}

void mission_2(){
    setup_orient();
    turn(90);
    follow_wall_pid(-2500);
    turn(-90);
    drive(150);
    drop_rel_pos();

    /*tillbaka*/
    drive(-150);
    turn(90);
    follow_wall_pid(2500);
    turn(-90);
}

void mission_3(){
    setup_orient();

    drive(-(HALL_WIDTH-900));
    turn(180);
    touch_align();

    drive(-500);

    turn(90);
    follow_wall_pid(2500);

    turn(-90);
    drive(250);
    drop_rel_pos();

    /*tillbaka*/

    drive(-250);
    turn(90);
    follow_wall_pid(-2500);

    turn(-90);
    touch_align();
    
    drive(-(HALL_WIDTH-900));
    turn(180);
    touch_align();
    
    drive(-500);
    
} 

void mission_4(){
    setup_orient();

    drive(-(HALL_WIDTH-900));
    turn(180);
    touch_align();

    drive(-500);

    turn(90);
    follow_wall_pid(-2500);

    turn(-90);
    drive(150);
    drop_rel_pos();

    /*tillbaka*/

    drive(-150);
    turn(90);
    follow_wall_pid(2500);

    turn(-90);
    touch_align();

    drive(-(HALL_WIDTH-900));
    turn(180);
    touch_align();

    drive(-500);
}


int main(){
    if (!init()) return ( 1 );
    mission_1();
    mission_2();
    mission_3();
    mission_4();
}
