#include <phys253.h>
//#include "linefollower.h"

//linefollower follower(&LCD, &motor);

void setup(void)
{
#include <phys253setup.txt>
Serial.begin(9600);
}

void loop(void)
{
  motor.speed(0,map(analogRead(PF6),0,1024,-255,255));
  RCServo0.write(map(analogRead(PF7),0,1024,0,180));
  //follower.follow_line();
  //follower.set_trim();
}
