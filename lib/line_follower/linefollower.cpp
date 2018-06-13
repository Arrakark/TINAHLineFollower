#include "linefollower.h"

bool debug = true;
bool run_motor = true;
int i_sum = 0;
int d_error_prev = 0;

CmdMessenger cmdMessenger = CmdMessenger(Serial);

enum
{
	GAIN = 1,		 //GAIN,
	LIMIT = 2,		 //LIMIT,
	SENSOR = 3,		 //SENSOR,
	speed = 4,		 //speed,
	getPID = 5,		 //getPID,
	enabledebug = 6, //enabledebug,
	kError = 7,		 //kError,
};

//
float P_GAIN = 0.1;
float P_LIMIT = 100;
float D_GAIN = 0;
float D_LIMIT = 0;
float I_GAIN = 0;
float I_LIMIT = 0;

#define TRIM_KNOB PF6
#define GENERAL_KNOB PF7

#define RIGHT_SENSOR PF1
float RIGHT_MAX = 793.0;
float RIGHT_MIN = 0.0;

#define LEFT_SENSOR PF0
float LEFT_MAX = 752.0;
float LEFT_MIN = 0.0;

#define RIGHT_MOTOR 1
#define LEFT_MOTOR 0

float NORMAL_SPEED = 255;

linefollower::linefollower(LiquidCrystal *passed_crystal, motorClass *passed_motor)
{
	LCD = passed_crystal;
	motor = passed_motor;

	cmdMessenger.attach(OnUnknownCommand);
	cmdMessenger.attach(GAIN, setgain);
	cmdMessenger.attach(LIMIT, setlimit);
	cmdMessenger.attach(SENSOR, setsensor);
	cmdMessenger.attach(speed, setspeed);
	cmdMessenger.attach(getPID, returnPID);
	cmdMessenger.attach(enabledebug, toggledebug);
	time = 0;
}

void linefollower::setup()
{

	float right_read = analogRead(RIGHT_SENSOR);
	float left_read = analogRead(LEFT_SENSOR);
	if (debug)
	{
		Serial.print("RIGHT_SENSOR: " + String(right_read));
		Serial.println(" LEFT_SENSOR: " + String(left_read));
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
	set_trim();
	//Read the sensors
	float right_read = analogRead(RIGHT_SENSOR);
	float left_read = analogRead(LEFT_SENSOR);
	//Map the sensor values
	right_read = map(right_read, RIGHT_MIN, RIGHT_MAX, 0, 1000);
	left_read = map(left_read, LEFT_MIN, LEFT_MAX, 0, 1000);
	//Apply proportional gain
	float p_error = (left_read - right_read) * P_GAIN;
	p_error = error_limit(p_error, P_LIMIT);
	//Apply/calculate integral
	i_sum = error_limit(i_sum + (left_read - right_read) * (float)(millis() - time), I_LIMIT);
	float i_error = i_sum * I_GAIN;
	//Apply/calculate derivitive
	float d_error = (left_read - right_read) / (float)(millis() - time) - d_error_prev;
	d_error = error_limit(d_error, D_LIMIT);

	float error = p_error + i_error + d_error + trim;

	if (run_motor)
	{
		if (NORMAL_SPEED + error < 0)
		{
			motor->speed(RIGHT_MOTOR, (int)(NORMAL_SPEED + error));
		}
		else
		{
			motor->speed(RIGHT_MOTOR, (int)(NORMAL_SPEED + error));
		}
		if (NORMAL_SPEED - error < 0)
		{
			motor->speed(LEFT_MOTOR, (int)(NORMAL_SPEED - error));
		}
		else
		{
			motor->speed(LEFT_MOTOR, (int)(NORMAL_SPEED - error));
		}
	}
	if (debug)
	{
		Serial.print("L_S: " + String(left_read));
		Serial.print(" R_S: " + String(right_read));
		Serial.print(" P_E: " + String(p_error));
		Serial.print(" I_E: " + String(i_error));
		Serial.print(" D_E: " + String(d_error));
		Serial.print(" R_M: " + String((int)(NORMAL_SPEED + error)));
		Serial.print(" L_M: " + String((int)(NORMAL_SPEED - error)));
		Serial.println(" TRIM: " + String(trim));
	}
	d_error_prev = left_read - right_read;
	time = millis();
}

void linefollower::evaluate_commands(){
	cmdMessenger.feedinSerialData();
}

int linefollower::error_limit(int value, int limit)
{
	if (value > limit)
	{
		return limit;
	}
	if (value < -limit)
	{
		return -limit;
	}
	return value;
}

void linefollower::set_trim()
{
	int readvalue = analogRead(TRIM_KNOB);
	trim = map(readvalue, 0, 1024, 50, -50);
}

void linefollower::setgain()
{
	char type = cmdMessenger.readCharArg();
	if (type == 'P')
	{
		P_GAIN = cmdMessenger.readFloatArg();
	}
	if (type == 'I')
	{
		I_GAIN = cmdMessenger.readFloatArg();
	}
	if (type == 'D')
	{
		D_GAIN = cmdMessenger.readFloatArg();
	}
}
void linefollower::setlimit()
{
	char type = cmdMessenger.readCharArg();
	if (type == 'P')
	{
		P_LIMIT = cmdMessenger.readFloatArg();
	}
	if (type == 'I')
	{
		I_LIMIT = cmdMessenger.readFloatArg();
	}
	if (type == 'D')
	{
		D_LIMIT = cmdMessenger.readFloatArg();
	}
}
void linefollower::setsensor()
{
	char dir = cmdMessenger.readCharArg();
	if (dir == 'R')
	{
		char maxmin = cmdMessenger.readCharArg();
		if (maxmin == 'X')
		{
			RIGHT_MAX = cmdMessenger.readInt16Arg();
		}
		if (maxmin == 'N')
		{
			RIGHT_MIN = cmdMessenger.readInt16Arg();
		}
	}
	if (dir == 'L')
	{
		char maxmin = cmdMessenger.readCharArg();
		if (maxmin == 'X')
		{
			LEFT_MAX = cmdMessenger.readInt16Arg();
		}
		if (maxmin == 'N')
		{
			LEFT_MIN = cmdMessenger.readInt16Arg();
		}
	}
}
void linefollower::setspeed()
{
	NORMAL_SPEED = cmdMessenger.readInt16Arg();
}
void linefollower::OnUnknownCommand()
{
	cmdMessenger.sendCmd(kError, "Command without attached callback");
}
void linefollower::toggledebug()
{
	debug = !debug;
}
void linefollower::returnPID()
{
	Serial.print("PGAIN: ");
	Serial.print(P_GAIN);
	Serial.print(" IGAIN: ");
	Serial.print(I_GAIN);
	Serial.print(" DGAIN: ");
	Serial.print(D_GAIN);
	Serial.print(" PLIMIT: ");
	Serial.print(P_LIMIT);
	Serial.print(" ILIMIT: ");
	Serial.print(I_LIMIT);
	Serial.print(" DLIMIT: ");
	Serial.println(D_LIMIT);
}