#include <phys253.h>
#include "linefollower.h"

linefollower follower(&LCD, &motor);

void setup(void)
{
#include <phys253setup.txt>
  Serial.begin(9600);
  //follower.setup();
}

void loop(void)
{
  //follower.follow_line();
  //follower.set_trim();
  motor.speed(0,255);
  motor.speed(1,255);
  //Serial.print(analogRead(PF0));
  //Serial.print(" ");
  //Serial.println(analogRead(PF1));
}
