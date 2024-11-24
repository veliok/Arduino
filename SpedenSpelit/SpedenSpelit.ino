#include "display.h"
#include "buttons.h"
#include "leds.h"
#include "SpedenSpelit.h"


volatile int buttonNumber = -1;            // for buttons interrupt handler
volatile int pressCounter = 0;
volatile int interruptCounter = -1;
int randomNumbers[100];                    // stores random integers
volatile int userNumbers[100];             // stores button presses
volatile int gameScore = 0;
volatile int gameMode = 0;
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
      startTheGame();
    }
  }

  if(isRunning)
  {
    if(buttonNumber >= 0 && buttonNumber < 4)
    {
      checkGame(buttonNumber);
      Serial.print("LED in array: ");
      Serial.print(randomNumbers[pressCounter]);
      Serial.print(" - NUM in array: ");
      Serial.println(userNumbers[pressCounter]);
    }
  }
}

void initializeTimer(void)
{
  TCCR1A = 0;               
  TCCR1B = 0;               
  TCNT1 = 0;
  OCR1A = 46875;                          // Generates 3s timer cycle
  TCCR1B |= (1 << WGM12);                 // CTC-mode enabled
  TCCR1B |= (1 << CS12) | (1 << CS10);    // prescaler 1024
  TIMSK1 |= (1 << OCIE1A);                // enable timer compare
}

ISR(TIMER1_COMPA_vect)
{
  interruptCounter++;
  int ledToWrite = randomNumbers[interruptCounter];
  if(isRunning && interruptCounter != 0) // Only set leds if game is running and gameMode has been selected
  {
    if(gameMode == 0) // Normal game mode
    {
      setLed(ledToWrite);
    }

    if(gameMode == 1) // Faster game mode
    {
      setLed(ledToWrite);
      OCR1A = OCR1A - 1248;
      if(OCR1A < 3592) OCR1A = 3592;
    }

    if(gameMode == 2) // Inverted led game mode
    {
      for(int i = 0; i < 4; i ++)
      {
        if(i != ledToWrite) setLed(i);
      }
    }

    OCR1A = OCR1A - 624;   // Increase timer interrupt rate by ~40ms
    if(OCR1A < 3592) OCR1A = 3592; // lower limit for rate
  }
}

void checkGame(byte nbrOfButtonPush)
{
  pressCounter++;
  userNumbers[pressCounter] = nbrOfButtonPush;

  if(randomNumbers[pressCounter] != userNumbers[pressCounter]) // stop game if button press is wrong and display score
  {
    isRunning = false;
    stopTheGame();
  }

  if(interruptCounter == 100) stopTheGame(); // Stop game after 100 rounds
  gameScore++;
  showResult(gameScore);
  buttonNumber = -1;
}

void initializeGame()
{
  gameMode = buttonNumber;
  gameScore = 0;
  pressCounter = 0;
  isRunning = true;
  buttonNumber = -1;
  interruptCounter = 0;

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

void stopTheGame()
{
  Serial.println("Game ended");
  Serial.print("End Score: ");
  showResult(gameScore);
  TIMSK1 &= ~(1 << OCIE1A); // Disable timer
}

