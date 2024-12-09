#ifndef DISPLAY_H
#define DISPLAY_H
#include <arduino.h>

void initializeDisplay(void);
void showResult(byte result);
void writeNumberToThreeSegments(uint8_t hundreds, uint8_t tens, uint8_t ones);
void writeByte(uint8_t bits, bool last);

#endif
