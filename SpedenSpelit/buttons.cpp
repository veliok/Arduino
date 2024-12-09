#include "buttons.h"

const unsigned long debounceDelay = 100;
volatile unsigned long lastInterruptTime[NUM_BUTTONS] = {0};
volatile bool buttonState[NUM_BUTTONS] = {0};
extern volatile int buttonNumber;                        // Napin numero pelilogiikalle. T. Veli


void initButtonsAndButtonInterrupts(void)
{
  for (byte pin = firstPin; pin <= lastPin; pin++) {
    pinMode (pin, INPUT_PULLUP);
  }

  PCICR |= (1 << PCIE2);
  PCMSK2 |= 0b01111100;
}

ISR(PCINT2_vect) {
  unsigned long currentTime = millis();
  for (byte pin = firstPin; pin <= lastPin; pin++) {
    byte index = pin - firstPin;
      if (digitalRead(pin) == LOW) {
        if (currentTime - lastInterruptTime[index] > debounceDelay) {
          lastInterruptTime[index] = currentTime;
            if (!buttonState[index]) {
              buttonState[index] = true;
              Serial.print("Nappia ");
              Serial.print(index);
              Serial.println(" painettu");
              buttonNumber = index;         // Napin numero pelilogiikalle. T. Veli
            }
        }
      } else {
      buttonState[index] = false;
      }
  }
}
