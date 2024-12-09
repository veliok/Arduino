#ifndef BUTTONS_H
#define BUTTONS_H
#include <arduino.h>
#include <avr/io.h>
#include <avr/interrupt.h>


const byte firstPin = 2; // First PinChangeInterrupt on D-bus
const byte lastPin =  5; // Last PinChangeInterrupt on D-bus

#define NUM_BUTTONS (lastPin - firstPin + 1)

extern const unsigned long debounceDelay; // debounce delay

extern volatile unsigned long lastInterruptTime[]; // store last interrupt times for debounce handling
extern volatile bool buttonState[];

void initButtonsAndButtonInterrupts(void);


// Intoduce PCINT2_vect Interrupt SeRvice (ISR) function for Pin Change Interrupt.
ISR(PCINT2_vect); 
#endif