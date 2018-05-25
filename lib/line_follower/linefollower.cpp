#include "linefollower.h"


linefollower::linefollower(LiquidCrystal * passed_crystal, motorClass * passed_motor)
{
	LCD = passed_crystal;
	motor = passed_motor;
}

void linefollower::setup()
{
	LCD->clear();
	LCD->home();
	LCD->setCursor(0, 0);
	LCD->write("Line Follower");
	LCD->setCursor(0, 1);
	LCD->write("Setup!");
  	delay(1000);
}

void linefollower::follow_line()
{
	motor->speed(0, 255 + trim);
	motor->speed(1, 255 - trim);
}

void linefollower::set_trim()
{
	int readvalue = analogRead(PF6);
	trim = map(readvalue, 0, 1024, 50, -50);
}
