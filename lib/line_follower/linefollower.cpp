#include "linefollower.h"

bool debug = true;
bool run_motor = true;
int i_sum = 0;
int d_error_prev = 0;

//
#define P_GAIN 0.3
#define P_LIMIT 100
#define D_GAIN 0.0007
#define D_LIMIT 50
#define I_GAIN 1
#define I_LIMIT 100
#define I_SUM_LIMIT 200

#define TRIM_KNOB PF6
#define GENERAL_KNOB PF7

#define RIGHT_SENSOR PF1
#define RIGHT_MAX 770.0
#define RIGHT_MIN 19.0

#define LEFT_SENSOR PF0
#define LEFT_MAX 620.0
#define LEFT_MIN 17.0

#define RIGHT_MOTOR 1
#define LEFT_MOTOR 0

#define NORMAL_SPEED 100

linefollower::linefollower(LiquidCrystal *passed_crystal, motorClass *passed_motor)
{
	LCD = passed_crystal;
	motor = passed_motor;
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
	float right_read = analogRead(RIGHT_SENSOR);
	float left_read = analogRead(LEFT_SENSOR);
	right_read = map(right_read, RIGHT_MIN, RIGHT_MAX, 0, 1000);
	left_read = map(left_read, LEFT_MIN, LEFT_MAX, 0, 1000);
	float p_error = (left_read - right_read) * P_GAIN;
	p_error = error_limit(p_error, P_LIMIT);
	i_sum = error_limit(i_sum + left_read - right_read, I_SUM_LIMIT);
	float i_error = error_limit(i_sum * I_GAIN, I_LIMIT);
	float d_error = (left_read - right_read) - d_error_prev;
	d_error = error_limit(d_error, D_LIMIT);

	float error = p_error + i_error + d_error + trim;

	if (run_motor)
	{
		motor->speed(RIGHT_MOTOR, (int)(NORMAL_SPEED + error));
		motor->speed(LEFT_MOTOR, (int)(NORMAL_SPEED - error));
	}
	if (debug)
	{
		Serial.print("RIGHT_SENSOR: " + String(right_read));
		Serial.print(" LEFT_SENSOR: " + String(left_read));
		Serial.print(" P_ERROR: " + String(p_error));
		Serial.print(" I_ERROR: " + String(i_error));
		Serial.print(" D_ERROR: " + String(d_error));
		Serial.print(" RIGHT_MOTOR: " + String((int)(NORMAL_SPEED + error)));
		Serial.print(" LEFT_MOTOR: " + String((int)(NORMAL_SPEED - error)));
		Serial.println(" TRIM: " + String(trim));
	}
	d_error_prev = left_read - right_read;
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
