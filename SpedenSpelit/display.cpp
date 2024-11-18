#include "display.h"

const int serialInput = 8;  // Input pin for data
const int outEnable = 9;    // LOW enables output
const int latchClock = 10;  // HIGH outputs new values
const int shiftClock = 11;  // HIGH value shifts all values forward one
const int reset = 12;       // Sets all bits to 0 if LOW

const byte digitToSegment[] = 
{
    0b00111111, // 0
    0b00000110, // 1
    0b01011011, // 2
    0b01001111, // 3
    0b01100110, // 4
    0b01101101, // 5
    0b01111101, // 6
    0b00000111, // 7
    0b01111111, // 8
    0b01101111  // 9
}

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
    for(int i = 8; i > 0; i--)
    {
        if(bits & (1 << i)) // bitwise AND and left shift
        {
            digitalWrite(serialInput, HIGH);
        }
        else
        {
            digitalWrite(serialInput, LOW);
        }

        digitalWrite(shiftClock, HIGH);
        digitalWrite(shiftClock, LOW);
    }

    if(last)
    {
        digitalWrite(latchClock, HIGH);
        digitalWrite(latchClock, LOW);
    }
}


void writeHighAndLowNumber(uint8_t tens,uint8_t ones)
{
    uint8_t tensSegment = digitToSegment[tens];
    uint8_t onesSegment = digitToSegment[ones];

    writeByte(tensSegment, false);
    writeByte(onesSegment, true);
}

void showResult(byte number)
{
    if(number > 99)
    {
        number = 99;
    }

    uint8_t tens = number / 10;
    uint8_t ones = number % 10;

    writeHighAndLowNumber(tens, ones);
}

