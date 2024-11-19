#include "display.h"

const int serialInput = 8;  // Input pin for data
const int outEnable = 9;    // LOW enables output
const int latchClock = 10;  // HIGH outputs new values
const int shiftClock = 11;  // HIGH value shifts all values forward one
const int reset = 12;       // Sets all bits to 0 if LOW


void initializeDisplay(void)
{
    pinMode(serialInput, OUTPUT);
    pinMode(outEnable, OUTPUT);
    pinMode(latchClock, OUTPUT);
    pinMode(shiftClock, OUTPUT);
    pinMode(reset, OUTPUT);

    digitalWrite(serialInput, LOW);
    digitalWrite(outEnable, LOW);
    digitalWrite(latchClock, LOW);
    digitalWrite(shiftClock, LOW);
    digitalWrite(reset, HIGH);
}


void writeByte(uint8_t bits,bool last)
{
    
}


void writeHighAndLowNumber(uint8_t tens,uint8_t ones)
{
   
}

void showResult(byte number)
{
   
}

