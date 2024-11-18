#include "leds.h"

const int pin2 = A2;
const int pin3 = A3;
const int pin4 = A4;
const int pin5 = A5;

void initializeLeds()
{
  pinMode(pin2, OUTPUT);
  pinMode(pin3, OUTPUT);
  pinMode(pin4, OUTPUT);
  pinMode(pin5, OUTPUT);
}

void setLed(byte ledNumber)
{
  switch(ledNumber) 
  {
  case 0:
    digitalWrite(pin2, HIGH);
    break;
  case 1:
    digitalWrite(pin3, HIGH);
    break;
  case 2:
    digitalWrite(pin4, HIGH);
    break;
  case 3:
    digitalWrite(pin5, HIGH);
    break;
  default:
    digitalWrite(pin2, LOW);
    digitalWrite(pin3, LOW);
    digitalWrite(pin4, LOW);
    digitalWrite(pin5, LOW);
  }
}

void clearAllLeds()
{
  digitalWrite(pin2, LOW);
  digitalWrite(pin3, LOW);
  digitalWrite(pin4, LOW);
  digitalWrite(pin5, LOW);
}

void setAllLeds()
{
  digitalWrite(pin2, HIGH);
  digitalWrite(pin3, HIGH);
  digitalWrite(pin4, HIGH);
  digitalWrite(pin5, HIGH);
}


void show1()
{
// see requirements for this function from leds.h
}

void show2(int rounds)
{
// see requirements for this function from leds.h  
}
