const int interruptPin2 = 2;
const int interruptPin3 = 3;
volatile int counter = 0;

volatile unsigned long last_interrupt = 0;
const unsigned long debounce = 500;


void setup() {
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);

  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(interruptPin2), incrementOne, FALLING);
  attachInterrupt(digitalPinToInterrupt(interruptPin3), incrementTen, FALLING);
}

void loop() {
  delay(5000);
  Serial.println(counter);
  counter = 0;
}

void incrementOne() {
  unsigned long interrupt = millis();
  if(interrupt - last_interrupt > debounce) {
    counter += 1;
    last_interrupt = interrupt;
  }
}

void incrementTen() {
  unsigned long interrupt = millis();
   if(interrupt - last_interrupt > debounce) {
    counter += 10;
    last_interrupt = interrupt;
  }
}


