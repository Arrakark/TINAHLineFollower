#include <phys253.h>
#include "linefollower.h"
#include <Timer.h>

linefollower follower(&LCD, &motor);

Timer main_timer;
Timer secondary_timer;

void main_tick()
{
  follower.follow_line();
  follower.set_trim();
}

void secondary_tick()
{
  follower.evaluate_commands();
}

void setup(void)
{
#include <phys253setup.txt>
  Serial.begin(9600);
  main_timer.every(250, main_tick);
  secondary_timer.every(100, secondary_tick);
}

void loop(void)
{
  main_timer.update();
  secondary_timer.update();
}