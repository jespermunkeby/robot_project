#include "robot.h"

void mission_1(){
    find_wall();
    turn_gyro(-90);
    touch_align();

    drive(-150);
    turn_gyro(90);
    
    follow_wall_pid(-2500); //ska använda follow_wall_pd()
    drop();
}
void mission_2(){
    find_wall();
    turn_gyro(-90);
    touch_align();
    
    drive(-150);
    turn_gyro(90);

    follow_wall_pid(2500); // ska använda follow_wall_pd()
    drop();
}

void mission_3(){
    find_wall();
    turn_gyro(-90);
    touch_align();
    
    drive(-150);
    turn_gyro(180);
    touch_align();

    drive(-150);
    turn_gyro(90);

    follow_wall_pid(2500); // ska använda follow_wall_pd()
    drop();
}

void mission_4(){
    find_wall();
    turn_gyro(-90);
    touch_align();
    
    drive(-150);
    turn_gyro(180);
    touch_align();

    drive(-150);
    turn_gyro(-90);

    follow_wall_pid(-2500); // ska använda follow_wall_pd()
    drop();
}

int main{
    if (!init()) return ( 1 ); 
}
