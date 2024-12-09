#include "display.h"
/*
  Tällä kytkennällä kommentoidut osat olivat turhia. T. Veli
  https://europe1.discourse-cdn.com/arduino/original/4X/d/a/5/da5d3161ac7532db20b9950c4c153683cc12824e.png
*/
//const int resetPin = 12;
const int shiftClockPin = 11;
const int latchClockPin = 10;
//const int outEnablePin = 9;
const int serialInputPin = 8;

void initializeDisplay(void)
{
  //pinMode(resetPin, OUTPUT);
  pinMode(shiftClockPin, OUTPUT);
  pinMode(latchClockPin, OUTPUT);
  //pinMode(outEnablePin, OUTPUT);
  pinMode(serialInputPin, OUTPUT);

  //digitalWrite(resetPin, HIGH); 
  //delay(10);
  //digitalWrite(resetPin, LOW);
  //digitalWrite(outEnablePin, LOW); 
  digitalWrite(latchClockPin, LOW);
  digitalWrite(shiftClockPin, LOW);
}

void writeByte(uint8_t bits, bool last)
{
  for (int i = 7; i >= 0; i--) 
  {
    digitalWrite(shiftClockPin, LOW);
    digitalWrite(serialInputPin, (bits >> i) & 0x01); 
    digitalWrite(shiftClockPin, HIGH); 
  }

  if (last)
  {
    digitalWrite(latchClockPin, HIGH); 
    delayMicroseconds(10);
    digitalWrite(latchClockPin, LOW);
  }
}

void writeNumberToThreeSegments(uint8_t hundreds, uint8_t tens, uint8_t ones)
{
  const uint8_t segmentDigits[] = {
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
  };

  if (hundreds > 9 || tens > 9 || ones > 9) return; // Välistä puuttui ||. T. Veli

  writeByte(segmentDigits[hundreds], false); 
  writeByte(segmentDigits[tens], false);
  writeByte(segmentDigits[ones], true);
}

void showResult(byte number)
{
  if (number > 999) return; 

  uint8_t hundreds = number / 100;
  uint8_t tens = (number / 10) % 10;
  uint8_t ones = number % 10;
  writeNumberToThreeSegments(hundreds, tens, ones);
}
