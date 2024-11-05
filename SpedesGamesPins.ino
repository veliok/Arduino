bool gameState = false;
int score = 0;
int randomPin = 0;
volatile int playerPin = 0;
const int pins[] = {2,3,4,5};


void setup() {
  Serial.begin(9600);

  for(int pin : pins) {
    pinMode(pin, INPUT_PULLUP);
  }

  PCICR = B00000100;
  PCMSK2 = B00111100; // Digital pins 2-5
  interrupts();
}

void loop() {
  Serial.println("Start game by sending 's' to serial monitor");
  while(Serial.available() == 0) {  } // Odotetaan käyttäjää
  
  if(int incomingByte = Serial.read() == 115) {
    gameState = true;
    while(gameState == true) {
      game();
    }
  }
}

void game() {
  randomPin = random(2, 6);
  Serial.println(randomPin);
  delay(2000);

  if(playerPin != randomPin) {
      Serial.print("Score: ");
      Serial.println(score);
      score = 0;
      gameState = false;
    }

  if(playerPin == randomPin) {
    score++;
  }
  playerPin = 0;
}

ISR(PCINT2_vect) {
  for(int i = 2; i < 6; i++) {
    if(digitalRead(i) == LOW) {
      playerPin = i;
    }
  }
}
