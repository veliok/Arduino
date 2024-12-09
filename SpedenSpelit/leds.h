#ifndef LEDS_H
#define LEDS_H
#include <arduino.h>

void initializeLeds();
void setLed(byte ledNumber);
void setLeds(byte ledNumber);
void clearAllLeds(void);
void setAllLeds(void);
void show1();   // show1 ja show2 puuttui .h tiedostosta. T. Veli
void show2();


#endif