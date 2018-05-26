#include "linefollower.h"

linefollower::linefollower(LiquidCrystal *passed_crystal, motorClass *passed_motor)
{
	LCD = passed_crystal;
	motor = passed_motor;
}

void linefollower::setup()
{
	write_to_LCD("LINE FOLLOWER", "SETUP");
	delay(1000);
	write_to_LCD("USE KNOB 6", "TO ADJUST TRIM");
	delay(1000);
	while (!digitalRead(TRIM_KNOB))
	{
		write_to_LCD("CURRENT TRIM", String(trim));
		follow_line();
		set_trim();
	}
	write_to_LCD("ADJUST LEFT", "SENSOR");
	for (int i = 40; i < 42; i++)
	{
		write_to_LCD(String("LSENS: ") + String(), String("SETPNT:") + String());
	}
}

void linefollower::write_to_LCD(String line1, String line2)
{
	LCD->clear();
	LCD->home();
	LCD->write(line1);
	LCD->setCursor(0, 1);
	LCD->write(line2);
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
