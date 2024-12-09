#include "display.h"
#include "buttons.h"
#include "leds.h"
#include "SpedenSpelit.h"

const int TIMER_MIN = 3800;                // lower limit for timer

volatile int buttonNumber = -1;            // button pressed
volatile int pressCounter = 0;
volatile int interruptCounter = -1;
int randomNumbers[100];
volatile int userNumbers[100];             // stores button presses
volatile int gameScore = 0;
int gameMode = 0;
volatile bool isRunning = false;
volatile bool timerInterrupt = false;

// Arrays and variables for MULTI mode
bool ledState[4] = {false,false,false,false};
bool buttonPressed[4] = {false,false,false,false};
int requiredButtons = 0;


void setup()
{
  Serial.begin(9600);
  initButtonsAndButtonInterrupts();
  initializeDisplay();
  initializeLeds();
}

void loop()
{
  if(buttonNumber >= 0 && buttonNumber < 4)
  {
    if(!isRunning) startTheGame();
    else if(isRunning)
    {
      if(gameMode == MULTI) multiButtonCheck(buttonNumber);   // Handle multimode separately for multiple button presses at once
      else checkGame(buttonNumber);
    }
  }
    
  if(isRunning && timerInterrupt)
  {
    interruptHandler();
  }
}

void initializeTimer(void)
{
  TCCR1A = 0;               
  TCCR1B = 0;               
  TCNT1 = 0;
  OCR1A = 31250;                          // Generates 2s timer cycle
  TCCR1B |= (1 << WGM12);                 // CTC-mode enabled
  TCCR1B |= (1 << CS12) | (1 << CS10);    // prescaler 1024
  TIMSK1 |= (1 << OCIE1A);                // enable timer compare
}

ISR(TIMER1_COMPA_vect)
{
  timerInterrupt = true;
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
  else gameScore++;

  showResult(gameScore);
  buttonNumber = -1;
}

void multiButtonCheck(byte nbrOfButtonPush)
{
  if(!ledState[nbrOfButtonPush]) // Check if wrong button is pressed
  {
    isRunning = false;
    stopTheGame();
  }

  buttonPressed[nbrOfButtonPush] = true;

  //  Check if all required buttons are pressed
  bool allPressed = true;
  for(int i = 0; i < 4; i++)
  {
    if(ledState[i] && !buttonPressed[i])
    {
      allPressed = false;
      break;
    }
  }

  if(allPressed)
  {
    gameScore++;
    showResult(gameScore);
    resetMultiButton();
  }
  
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

  if(gameMode == MULTI)
  {
    resetMultiButton();
  }

  for(int i = 0; i < 100; i++)
  {
    randomNumbers[i] = random(0, 4); // Single LED mode
  }
}

void startTheGame()
{
  initializeGame();
  showResult(gameScore);
  show1();
  delay(2000);
  initializeTimer();
}

void stopTheGame()
{
  show2();
  showResult(gameScore);
  TIMSK1 &= ~(1 << OCIE1A); // Disable timer at the end of round
}

void interruptHandler()
{
  interruptCounter++;
  if(interruptCounter == 99) 
  {
    interruptCounter = 1;
    pressCounter = 1;
  }

  int ledToWrite = randomNumbers[interruptCounter];

  switch(gameMode)
  {
    case SINGLE:
    case FASTER:
      gameNormal(ledToWrite);
      break;
    case INVERSE:
      gameInverse(ledToWrite);
      break;
    case MULTI:
      gameMultiButton();
      break;
  }

  if(interruptCounter % 10 == 0 && OCR1A > TIMER_MIN) // Adjust speed
  {
      if(gameMode == FASTER) OCR1A = OCR1A - 2248;    // faster game
      else OCR1A = OCR1A - 1248;                      // normal game
  }
  timerInterrupt = false;
}

void gameNormal(int ledToWrite)
{
  setLed(ledToWrite);
}

void gameInverse(int ledToWrite)
{
  clearAllLeds();
  for(int i = 0; i < 4; i ++)
  {
    if(i != ledToWrite)
    { 
      setLeds(i);
    }
  }
}

void gameMultiButton()
{
  clearAllLeds();
  resetMultiButton();

  requiredButtons = random(1, 5);
  for(int i = 0; i < requiredButtons; i++)
  {
    int ledToWrite = random(0, 4);
    ledState[ledToWrite] = true;
    setLeds(ledToWrite);
  }
}

void resetMultiButton()
{
  for(int i = 0; i < 4; i++)  // Reset state
  {
    ledState[i] = false;
    buttonPressed[i] = false;
  }
}
