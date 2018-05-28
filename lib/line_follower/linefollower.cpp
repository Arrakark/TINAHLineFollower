#include "linefollower.h"

bool debug = true;
int leftSensorPrev = 0;
int rightSensorPrev = 0;
int leftSensorCurr = 0;
int rightSensorCurr = 0;

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
	LCD->write(line1.c_str());
	LCD->setCursor(0, 1);
	LCD->write(line2.c_str());
}

void linefollower::follow_line()
{
	leftSensorCurr = analogRead(PF0);
	rightSensorCurr = analogRead(PF1);
	
	int difference = leftSensorCurr - rightSensorCurr;
	difference = map(difference, -200, 200, 255, -255);
	int motorright = 255 + trim - difference;
	int motorleft = 255 - trim + difference;
	motor->speed(0, motorright);
	motor->speed(1, motorleft);

	if(debug){
		Serial.print("trim: " + String(trim));
		Serial.println(" difference: " + String(difference));
		//Serial.println("mr: " + String(motorright));
		//Serial.println("ml: " + String(motorleft));
	}

}

void linefollower::set_trim()
{
	int readvalue = analogRead(PF6);
	trim = map(readvalue, 0, 1024, 50, -50);
}
