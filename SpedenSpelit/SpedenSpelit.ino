#include "display.h"
#include "buttons.h"
#include "leds.h"
#include "SpedenSpelit.h"

const int TIMER_MIN = 3800;

volatile int buttonNumber = -1;            // for buttons interrupt handler
volatile int pressCounter = 0;
volatile int interruptCounter = -1;
int randomNumbers[100];                    // stores random integers
volatile int userNumbers[100];             // stores button presses
volatile int gameScore = 0;
int gameMode = 0;
volatile bool isRunning = false;
volatile bool timerInterrupt = false;


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
      if(gameMode == 3) multiButtonCheck(buttonNumber);   // Handle mode 3 separately for multiple button presses at once
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

  if(interruptCounter == 99) 
  {
    interruptCounter = 1;
    pressCounter = 1;
  }
  gameScore++;
  showResult(gameScore);
  buttonNumber = -1;
}

void multiButtonCheck(byte nbrOfButtonPush)
{
  userNumbers[pressCounter] |= (1 << nbrOfButtonPush);

  Serial.print("userNumbers[");
  Serial.print(pressCounter);
  Serial.print("]: ");
  Serial.println(userNumbers[pressCounter], BIN);

  Serial.print("randomNumbers[");
  Serial.print(pressCounter);
  Serial.print("]: ");
  Serial.println(randomNumbers[pressCounter], BIN);

  if(userNumbers[pressCounter] == randomNumbers[pressCounter])
  {
    pressCounter++;
    gameScore++;
    showResult(gameScore);
    userNumbers[pressCounter] = 0;
    buttonNumber = -1;
  }
  else if(userNumbers[pressCounter] > randomNumbers[pressCounter])
  {
    isRunning = false;
    stopTheGame();
  }
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
    if(gameMode == 3)
    {
      int numberOfLeds = random(1, 4);

      for(int j = 0; j < numberOfLeds; j++)
      {
        randomNumbers[i] |= (1 << random(0, 4));
      }
    }
    else randomNumbers[i] = random(0, 4);
    Serial.print("randomNumbers[");
    Serial.print(i);
    Serial.print("]: ");
    Serial.println(randomNumbers[i], BIN);
  }
  
  Serial.println("Game initialized");
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
  Serial.println("Game ended");
  show2();
  showResult(gameScore);
  TIMSK1 &= ~(1 << OCIE1A); // Disable timer
}

void interruptHandler()
{
  interruptCounter++;
  int ledToWrite = randomNumbers[interruptCounter];

  switch(gameMode)
  {
    case 0:
    case 1:
      gameMode01(ledToWrite);
      break;
    case 2:
      gameMode2(ledToWrite);
      break;
    case 3:
      gameMode3(ledToWrite);
      break;
  }

  if(interruptCounter % 10 == 0 && OCR1A > TIMER_MIN)
  {
      if(gameMode == 1) OCR1A = OCR1A - 2248; // faster game
      else OCR1A = OCR1A - 1248;              // normal game
  }
  timerInterrupt = false;
}


void gameMode01(int ledToWrite)
{
  setLed(ledToWrite);
  Serial.println(ledToWrite);
}

void gameMode2(int ledToWrite)
{
  clearAllLeds();
  for(int i = 0; i < 4; i ++)
  {
    if(i != ledToWrite)
    { 
      setLeds(i);
      Serial.print("i: ");
      Serial.println(i);
      Serial.print("ledToWrite: ");
      Serial.println(ledToWrite);
    }
  }
}
/*
  gameMode 3 uses bitmasking to evaluate button presses and LEDs, 
  this ensures that the order of button presses won't matter.
*/
void gameMode3(int ledToWrite)
{
  clearAllLeds();

  for(int i = 0; i < 4; i++)
  {
    if(ledToWrite & (1 << i)) setLeds(i);
  }
  Serial.print("ledToWrite: ");
  Serial.println(ledToWrite, BIN);
}
