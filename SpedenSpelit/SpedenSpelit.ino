#include "display.h"
#include "buttons.h"
#include "leds.h"
#include "SpedenSpelit.h"

// Use these 2 volatile variables for communicating between
// loop() function and interrupt handlers
volatile int buttonNumber = -1;           // for buttons interrupt handler
volatile bool newTimerInterrupt = false;  // for timer interrupt handler
volatile int randomNumbers[150];


void setup()
{
  initButtonsAndButtonInterrupts();
  initializeDisplay();
  initializeLeds();
}

void loop()
{
  if(buttonNumber>=0)
  {
    // start the game if buttonNumber == 4
    if(buttonNumber == 4) {
      startTheGame();
    }
    // check the game if 0<=buttonNumber<4
    if(buttonNumber >= 0 && buttonNumber < 4) {
      checkGame();
    }
  }

  if(newTimerInterrupt == true)
  {
    // new random number must be generated
    // and corresponding let must be activated
    setLed(randomNumber);
    newTimerInterrupt = false;
  }
}

void initializeTimer(void)
{
	
}
ISR(TIMER1_COMPA_vect)
{
  /*
  Communicate to loop() that it's time to make new random number.
  Increase timer interrupt rate after 10 interrupts.
  */
  newTimerInterrupt = true;
  
}


void checkGame(byte nbrOfButtonPush)
{
	if()
}


void initializeGame()
{
	
}

void startTheGame()
{
  initializeGame();
}

