#include "robot.h"

void mission_1(){
    find_wall();
    turn_gyro(-90);
    touch_align();

    drive(-200);
    turn_gyro(90);
    
    follow_wall_pid(0.05,0,0.5,-2500); //ska använda follow_wall_pd()
    drop();
    
   //tillbaka
    follow_wall_pid(0.05,0,0.5,2500);
    turn(90);
    drive(300);
    
}
void mission_2(){
    find_wall();
    turn_gyro(-90);
    touch_align();
    
    drive(-200);
    turn_gyro(90);

    follow_wall_pid(0.05,0,0.5,2500); // ska använda follow_wall_pd()
    drop();
    
    //tillbaka    
    follow_wall_pid(0.05,0,0.5,-2500);
    turn(-90);
    drive(300);   
}

void mission_3(){
    find_wall();
    turn_gyro(-90);
    touch_align();
    
    drive(-2500);
    turn_gyro(180);
    touch_align();

    drive(-200);
    turn_gyro(90);

    follow_wall_pid(0.05,0,0.5,2500); // ska använda follow_wall_pd()
    drop();
    
    //tillbaka
   follow_wall_pid(0.05,0,0.5,2500);
    find_wall();
    turn_gyro(90);
    
    touch_align();
    drive(-2500);
    
}

void mission_4(){
    find_wall();
    turn_gyro(-90);
    touch_align();
    
    drive(-2500);
    turn_gyro(180);
    touch_align();

    drive(-200);
    turn_gyro(-90);

    follow_wall_pid(0.05,0,0.5,-2500); // ska använda follow_wall_pd()
    drop();
    
    //tillbaka
    follow_wall_pid(0.05,0,0.5,-2500);
    find_wall();
    turn_gyro(-90);
    
    touch_align();
    drive(-2500);
}

int main{
    if (!init()) return ( 1 ); 
    
}
