#include <Arduino.h>
#include <Romi32U4.h>
#include "chassis.h"

RomiChassis chassis;

enum ROBOT_STATE {ROBOT_IDLE, ROBOT_DRIVING};
ROBOT_STATE robot_state = ROBOT_IDLE;

Romi32U4ButtonA buttonA;

void setup() {
    Serial.begin(115200);
    //while(!buttonA.getSingleDebouncedRelease());
    /*
    while(!chassis.CheckDriveComplete()){
      chassis.UpdateEffortDriveWheels(100, 100);
      chassis.MotorControl();
      Serial.print(millis());
      Serial.print(" ");
      Serial.print(chassis.SpeedLeft());
      Serial.print(" ");
      Serial.println(chassis.SpeedRight());
      //chassis.SerialPlotter(millis(), chassis.SpeedLeft(), chassis.SpeedRight(), 0, 0, 0, 0);
      delay(50);
      
    }
    */
    
}

void loop() 
{
  switch(robot_state)
  {
    case ROBOT_IDLE:
      if(buttonA.getSingleDebouncedRelease()) 
      {
        chassis.StartDriving(50, 50, 10000); //contains your program that the robot executes when pushbutton A is pressed
        robot_state = ROBOT_DRIVING;
      }
      break;

    case ROBOT_DRIVING:
      chassis.MotorControl();
      if(chassis.CheckDriveComplete()) 
      {
        chassis.Stop();
        robot_state = ROBOT_IDLE;
      }
      if(buttonA.getSingleDebouncedRelease()) 
      {
        chassis.Stop();
        robot_state = ROBOT_IDLE;
      }
  }
}