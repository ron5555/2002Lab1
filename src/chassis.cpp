#include <Romi32U4.h>
#include "chassis.h"

float RomiChassis::SpeedLeft(void){
  int lcounts;
  lcounts = count_left - prev_count_left;
    
    return (1000 * lcounts * C_wheel / (N_wheel*interval)); //[mm/s]
}

float RomiChassis::SpeedRight(void)
{
    int rcounts;
    rcounts = count_right - prev_count_right;
    
    return (1000 * rcounts * C_wheel/(N_wheel*interval)); //[mm/s]
}

void RomiChassis::UpdateEffortDriveWheels(int left, int right)
{ 
    motors.setEfforts(left,right);
}

void RomiChassis::UpdateEffortDriveWheelsPI(int target_speed_left, int target_speed_right)
{
    E_left = target_speed_left - SpeedLeft();
    E_right = target_speed_right - SpeedRight();
    E_lsum = E_lsum + E_left;
    E_rsum = E_rsum + E_right;

    float u_left = Kp*E_left + Ki*E_lsum;
    float u_right = Kp*E_right + Ki*E_rsum;;

    motors.setEfforts(u_left,u_right);

    SerialPlotter(last_update, SpeedLeft(), SpeedRight(), E_left, E_right, u_left, u_right);
}

void RomiChassis::SerialPlotter(float a, float b, float c, float d, float e, float f, float g)
{
    Serial.print(a);
    Serial.print(" ");
    Serial.print(b);
    Serial.print(" ");
    Serial.print(c); 
    Serial.print(" ");
    Serial.print(d);
    Serial.print(" ");
    Serial.print(e);
    Serial.print(" ");
    Serial.print(f);
    Serial.print(" ");
    Serial.println(g);
}

void RomiChassis::MotorControl(void)
{
  uint32_t now = millis();
  if(now - last_update >= interval)
  {    
    prev_count_left = count_left;
    prev_count_right = count_right;
    count_left = encoders.getCountsLeft();
    count_right = encoders.getCountsRight();
    previous_time = millis();
    UpdateEffortDriveWheelsPI(target_left, target_right);
    last_update = now;
  }
}

void RomiChassis::StartDriving(float left, float right, uint32_t duration)
{
  target_left = left; target_right = right;
  start_time = millis();
  last_update = start_time;
  end_time = start_time + duration; //fails at rollover
  E_left = 0;
  E_right = 0;
}

bool RomiChassis::CheckDriveComplete(void)
{
  return millis() >= end_time;
}

void RomiChassis::Stop(void)
{
  target_left = target_right = 0;
  motors.setEfforts(0, 0);
}