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
int gameMode = 0;
volatile bool isRunning = false;
volatile bool timerInterrupt = false;


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
      if(gameMode == 3) multiButtonCheck(buttonNumber); // Handle mode 3 separately for multiple button presses
      else checkGame(buttonNumber);                     // Normal processing
      
      Serial.print("LED in array: ");
      Serial.print(randomNumbers[pressCounter]);
      Serial.print(" - NUM in array: ");
      Serial.println(userNumbers[pressCounter]);
    }
    
    if(timerInterrupt)
    {
      interruptCounter++;
      int ledToWrite = randomNumbers[interruptCounter];

      if(isRunning && interruptCounter != 0) // Only set LEDs if game is running and gameMode has been selected
      {
        if(gameMode == 0 || gameMode == 1) // Normal and faster game mode
        {
          setLed(ledToWrite);
        }

        if(gameMode == 2) // Inverted LED game mode
        {
          for(int i = 0; i < 4; i ++)
          {
            if(i != ledToWrite) setLed(i);
          }
        }
        /*
          gameMode 3 uses bitmasking to evaluate button presses and LEDs, 
          this ensures that the order of button presses won't matter.
        */
        if(gameMode == 3)
        {
          for(int i = 0; i < 4; i++)
          {
            if(ledToWrite & (1 << 1)) setLed(i);
          }
        }

        if(gameMode == 1) OCR1A = OCR1A - 624;    // Increase timer interrupt rate by ~40ms for faster game
        else OCR1A = OCR1A - 1248;                // Normal rate
        if(OCR1A < 3592) OCR1A = 3592;            // Limit for rate
      }
      timerInterrupt = false;
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

  if(interruptCounter == 100) stopTheGame(); // Stop game after 100 rounds
  gameScore++;
  showResult(gameScore);
  buttonNumber = -1;
}

void multiButtonCheck(byte nbrOfButtonPush)
{
  userNumbers[pressCounter] |= (1 << nbrOfButtonPush);

  if(userNumbers[pressCounter] == randomNumbers[pressCounter])
  {
    pressCounter++;
    if(interruptCounter == 100) stopTheGame();
    gameScore++;
    showResult(gameScore);
    buttonNumber = -1;
  }
  else
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
