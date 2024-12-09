#include "leds.h"

const int pin2 = A2;
const int pin3 = A3;
const int pin4 = A4;
const int pin5 = A5;


void initializeLeds() {
  // see requirements for this function from leds.h
  pinMode(pin2, OUTPUT);
  pinMode(pin3, OUTPUT);
  pinMode(pin4, OUTPUT);
  pinMode(pin5, OUTPUT);
}

void setLed(byte ledNumber) {
  // see requirements for this function from leds.h
  clearAllLeds();
  switch (ledNumber) {
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
/*
  Lisätty setLeds funktio usean ledin samanaikaiseen käyttöön. T. Veli
*/
void setLeds(byte ledNumber)
{
  switch (ledNumber) {
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


void clearAllLeds() {
  // see requirements for this function from leds.h
  digitalWrite(pin2, LOW);
  digitalWrite(pin3, LOW);
  digitalWrite(pin4, LOW);
  digitalWrite(pin5, LOW);
  delay(100); // to distinguish same led turning on multiple times in a row
}

void setAllLeds() {
  // see requirements for this function from leds.h
  digitalWrite(pin2, HIGH);
  digitalWrite(pin3, HIGH);
  digitalWrite(pin4, HIGH);
  digitalWrite(pin5, HIGH);
}

/* 

  Show1 funktiota piti muokata vähän. T. Veli

*/
void show1() {
  //  vasemmalta oikealle

  for (int i = 0; i < 4; i++) {
    setLed(i);
    delay(100);
  }
  clearAllLeds(); 
  // oikealta vasemmalle

  for (int i = 3; i >= 0; i--) {
    setLed(i);
    delay(100);
  }
  clearAllLeds(); 
}


void show2() 
{
  // see requirements for this function from leds.h 
  for(int i = 0; i < 6; i++) 
  {
    setAllLeds();
    delay(100);
    clearAllLeds(); 
    delay(100);
  }
}
