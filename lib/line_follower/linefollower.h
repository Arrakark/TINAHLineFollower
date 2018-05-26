#pragma once
#include <Arduino.h>
#include <LiquidCrystal.h>
#include <motor.h>

#define TRIM_KNOB PF6

class linefollower
{
  public:
	//consturctor
	linefollower(LiquidCrystal *, motorClass *);

	//member functions
	void setup();
	//void set_speed(int speed);
	void follow_line();
	//void stop_following();
	void set_trim();

	void write_to_LCD(String, String);

  private:
	motorClass *motor;
	LiquidCrystal *LCD;
	int trim;
};
