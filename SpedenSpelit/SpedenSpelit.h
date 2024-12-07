#ifndef SPEDENSPELIT_H
#define SPEDENSPELIT_H
#include <arduino.h>
#include <avr/io.h>
#include <avr/interrupt.h>


enum GameMode {
  SINGLE = 0,
  FASTER = 1,
  INVERSE = 2,
  MULTI = 3
};

/*
  initializeTimer() subroutine intializes Arduino Timer1 module to
  give interrupts at rate 1Hz
  
*/
void initializeTimer(void);
// Intoduce TIMER1_COMPA_vect Interrupt SeRvice (ISR) function for timer.
ISR(TIMER1_COMPA_vect);

/*
  initializeGame() subroutine is used to initialize all variables
  needed to store random numbers and player button push data.
  This function is called from startTheGame() function.
  
*/
void initializeGame(void);

/*
  checkGame() subroutine is used to check the status
  of the Game after each player button press.
  
  If the latest player button press is wrong, the game stops
  and if the latest press was right, game display is incremented
  by 1.
  
  Parameters
  byte lastButtonPress of the player 0 or 1 or 2 or 3
  
*/
void checkGame(byte);

/*
  multiButtonCheck() subroutine is used to check the status
  of game in gameMode 3, where player has to press multiple
  buttons.

  Parameters
  Byte lastButtonPress of the player 0 or 1 or 2 or 3
*/
void multiButtonCheck(byte);

/*
  startTheGame() subroutine calls InitializeGame()
  function and enables Timer1 interrupts to start
  the Game.
*/

void startTheGame(void);
void stopTheGame(void);
void interruptHandler();
void gameNormal(int);
void gameInverse(int);

/*
  MultiButton game mode uses boolean arrays to track 
  the LEDs and button presses.
*/
void gameMultiButton(int);
void resetMultiButton();


#endif
