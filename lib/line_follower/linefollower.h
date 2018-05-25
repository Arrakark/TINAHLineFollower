#pragma once
#include <Arduino.h>
#include <LiquidCrystal.h>
#include <motor.h>


class linefollower
{
	public:
		//consturctor
		linefollower(LiquidCrystal*, motorClass*);

		//member functions
		void setup();
		//void set_speed(int speed);
		void follow_line();
		//void stop_following();
		void set_trim();
	
	private:
		motorClass * motor;
		LiquidCrystal * LCD;
		int trim;
};



