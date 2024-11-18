#include "display.h"
#include "buttons.h"
#include "leds.h"
#include "SpedenSpelit.h"


volatile int buttonNumber = -1;           // for buttons interrupt handler
volatile int pressCounter = 0;             // counter for button presses
volatile bool newTimerInterrupt = false;  // for timer interrupt handler
volatile int interruptCounter = 0;        // counter for interrupts
volatile int randomNumbers[100];          // stores random integers
volatile int buttonPush[100];             // stores button presses
volatile int score = 0;
volatile bool running = false;


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
    if(buttonNumber == 4) 
    {
      startTheGame();
    }
    // check the game if 0<=buttonNumber<4
    if(running)
    {
      if(buttonNumber >= 0 && buttonNumber < 4) 
      {
        pressCounter++;
        checkGame(buttonNumber);
      }
    }
  }
  if(running)
  {
    if(newTimerInterrupt == true)
    {
      setLed(randomNumbers[interruptCounter]);    // interrupt turns new led on
      newTimerInterrupt = false;
    }
  }
}

void initializeTimer(void)
{
  TCNT
  OCR1A = 15624;                          // Generates 1hz timer cycle (16MHz/1024-1 = 15624)
  TCCR1B |= (1 << WGM12);                 // OCR1A CTC-mode enabled
  TCCR1B |= (1 << CS12) | (1 << CS10);    // prescaler 1024
  TIMSK1 |= (1 << OCIE1A);                // Clear Timer on Compare Match witch OCR1A
}

ISR(TIMER1_COMPA_vect)
{
  interruptCounter++;
  if(interruptCounter % 10 == 0) 
  {
    OCR1A = OCR1A - 1562;   // Increase timer interrupt rate after 10 interrupts MODIFY IF TOO FAST/SLOW
  }

  newTimerInterrupt = true;
}

void checkGame(byte nbrOfButtonPush)
{
  buttonPush[pressCounter] = nbrOfButtonPush;
  if(randomNumbers[pressCounter] != buttonPush[pressCounter]) // stop game if button press is wrong and display score
  {
    running = false;
  }
  else  // game continues
  {
    score++;
  }
  showResult(score);  // display score
}

void initializeGame()
{
  score = 0;
  pressCounter = 0;
  interruptCounter = 0;
  running = true;

  for(int i = 0; i < 100; i++)    // fill array with 100 random numbers from 0-3
  {
    randomNumbers[i] = random(0, 4);
  }
}

void startTheGame()
{
  initializeGame();
}

