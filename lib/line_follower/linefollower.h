#pragma once
#include <Arduino.h>
#include <LiquidCrystal.h>
#include <motor.h>
#include <CmdMessenger.h>

#define TRIM_KNOB PF6

class linefollower
{
public:
	//consturctor
	linefollower(LiquidCrystal *, motorClass *);
	//member functions
	void setup();
	void follow_line();
	void set_trim();
	void write_to_LCD(String, String);
	void evaluate_commands();
private:
	int error_limit(int, int);
	static void setgain();
	static void setlimit();
	static void setsensor();
	static void setspeed();
	static void setdebug();
	static void returnPID();
	static void toggledebug();
	static void OnUnknownCommand();
	motorClass *motor;
	LiquidCrystal *LCD;
	int trim;
	unsigned long time;
};
