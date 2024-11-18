#include "buttons.h"

extern volatile int buttonNumber;
volatile unsigned long lastInterrupt = 0;
const unsigned long debounce = 50;


void initButtonsAndButtonInterrupts(void)
{
  for(byte pin = firstPin; pin <= lastPin; pin++)
  {
    pinMode(pin, INPUT_PULLUP);
  }
  pinMode(6, INPUT_PULLUP);

  PCICR |= (1 << PCIE2);
  PCMSK2 |= (1 << PCINT18) | (1 << PCINT19) | (1 << PCINT20) | (1 << PCINT21) | (1 << PCINT22);
}

ISR(PCINT2_vect) {
  unsigned long interrupt = millis();
  if(interrupt - lastInterrupt > debounce)
  {
    if(digitalRead(6) == LOW)
    {
      buttonNumber = 4;
      return;
    }
    for(byte pin = firstPin; pin <= lastPin; pin++)
    {
      if(digitalRead(pin) == LOW) {
        buttonNumber = pin - firstPin;
        return;
      }
    }
    lastInterrupt = interrupt;
  }
}
