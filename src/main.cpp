#include <phys253.h>
//#include <LiquidCrystal.h>

#define LEFT_SENSOR A0
#define RIGHT_SENSOR A1



void setup()
{
#include <phys253setup.txt>
  Serial.begin(9600);
  pinMode(PF0,INPUT);
  pinMode(PF1,INPUT);
  pinMode(PF2,INPUT);
  pinMode(PF3,INPUT);
  pinMode(PF4,INPUT);
  pinMode(PF5,INPUT);
  pinMode(PF6,INPUT);
  pinMode(PF7,INPUT);
}

void loop()
{
    Serial.print(analogRead(PF0));
    Serial.print(" ");
    Serial.print(analogRead(PF1));
    Serial.print(" ");
    Serial.print(analogRead(PF2));
    Serial.print(" ");
    Serial.print(analogRead(PF3));
    Serial.print(" ");
    Serial.print(analogRead(PF4));
    Serial.print(" ");
    Serial.print(analogRead(PF5));
    Serial.print(" ");
    Serial.print(analogRead(PF6));
    Serial.print(" ");
    Serial.print(analogRead(PF7));
    Serial.println(" ");
}
