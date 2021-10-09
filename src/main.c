#include "robot.h"

void setup_orient(){
    find_wall();
    turn(-90);
    touch_align();
    drive(-400);
}

//TODO: Change missions to new delivery routes

void mission_1(){
    find_wall();
    turn(-90);
    touch_align();

    drive(-200);
    turn(90);
    
    follow_wall_pid(-2500);
    drop();
    
    /*tillbaka*/
    follow_wall_pid(2500);
    turn(90);
    drive(300);
    
}
void mission_2(){
    find_wall();
    turn(-90);
    touch_align();
    
    drive(-200);
    turn(90);

    follow_wall_pid(2500);
    drop();
    
    /*tillbaka*/
    follow_wall_pid(-2500);
    turn(-90);
    drive(300);   
}

void mission_3(){
    find_wall();
    turn(-90);
    touch_align();
    
    drive(-2500);
    turn(180);
    touch_align();

    drive(-200);
    turn(90);

    follow_wall_pid(2500);
    drop();
    
    /*tillbaka*/
   follow_wall_pid(2500);
    find_wall();
    turn(90);
    
    touch_align();
    drive(-2500);
    
}

void mission_4(){
    find_wall();
    turn(-90);
    touch_align();
    
    drive(-2500);
    turn(180);
    touch_align();

    drive(-200);
    turn(-90);

    follow_wall_pid(-2500);
    drop();
    
    /*tillbaka*/
    follow_wall_pid(-2500);
    find_wall();
    turn(-90);
    
    touch_align();
    drive(-2500);
}

int main(){
    if (!init()) return ( 1 ); 
    mission_1();
    mission_2();
}
