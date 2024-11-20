#include "display.h"
#include "buttons.h"
#include "leds.h"
#include "SpedenSpelit.h"


volatile int buttonNumber = -1;           // for buttons interrupt handler
volatile int pressCounter = 0;             // counter for button presses
int randomNumbers[100];          // stores random integers
volatile int userNumbers[100];             // stores button presses
volatile int gameScore = 0;
int gameMode = 0;
volatile bool isRunning = false;


void setup()
{
  initButtonsAndButtonInterrupts();
  initializeDisplay();
  initializeLeds();
}

void loop()
{
  if(!isRunning)
  {
    if(buttonNumber >= 0 && buttonNumber < 4)
    {
      gameMode = buttonNumber;
      isRunning = true;
      buttonNumber = -1;
    }
  }

  if(isRunning)
  {
    if(buttonNumber >= 0 && buttonNumber < 4)
    {
      checkGame();
      buttonNumber = -1;
      pressCounter++;
    }
  }
}

void initializeTimer(void)
{
  OCR1A = 15624;                          // Generates 1hz timer cycle (16MHz/1024-1 = 15624)
  TCCR1B |= (1 << WGM12);                 // CTC-mode enabled
  TCCR1B |= (1 << CS12) | (1 << CS10);    // prescaler 1024
  TIMSK1 |= (1 << OCIE1A);                // enable timer compare
}

ISR(TIMER1_COMPA_vect)
{
  OCR1A -= 1562;   // Increase timer interrupt rate after 10 interrupts MODIFY IF TOO FAST/SLOW
  if(OCR1A < 1000) OCR1A = 1000; // lower limit for rate
}

void checkGame(byte nbrOfButtonPush)
{
  if(randomNumbers[pressCounter] != userNumbers[pressCounter]) // stop game if button press is wrong and display score
  {
    isRunning = false;
    Serial.println("Game ended");
  }
  else gameScore++;
  //showResult(gameScore);  // display score
}

void initializeGame()
{
  gameScore = 0;
  pressCounter = 0;
  isRunning = true;
  buttonNumber = -1;

  for(int i = 0; i < 100; i++)    // fill array with 100 random numbers from 0-3
  {
    randomNumbers[i] = random(0, 4);
  }
  Serial.println("Game initialized");
}

void startTheGame()
{
  initializeGame();
  initializeTimer();
  showResult(gameScore);
}

