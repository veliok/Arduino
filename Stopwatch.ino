volatile int tenths = 0;
volatile int seconds = 0;
volatile int minutes = 0;
volatile bool running = false;
int lastPrint = 0;

const int startPin = 2;
const int stopPin = 3;

void setup() {
  Serial.begin(9600);
  pinMode(startPin, INPUT_PULLUP);
  pinMode(stopPin, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(startPin), startWatch, FALLING);
  attachInterrupt(digitalPinToInterrupt(stopPin), stopWatch, FALLING);

  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;

  OCR1A = 1562;
  TCCR1B |= (1 << WGM12);
  TCCR1B |= (1 << CS12) | (1 << CS10);
  TIMSK1 |= (1 << OCIE1A);
}

void loop() {
  if(running && seconds != lastPrint) {
    Serial.print(minutes);
    Serial.print(":");
    Serial.print(seconds);
    Serial.print(":");
    Serial.println(tenths);
    
    lastPrint = seconds;
  }
}

void startWatch() {
  if(!running) {
    running = true;
    tenths = 0;
    seconds = 0;
    minutes = 0;
    lastPrint = 0;
  }
}

void stopWatch() {
  if(running) {
    running = false;
    Serial.print(minutes);
    Serial.print(":");
    Serial.print(seconds);
    Serial.print(":");
    Serial.println(tenths);
    
    tenths = 0;
    seconds = 0;
    minutes = 0;
    lastPrint = 0;
  }
}

ISR(TIMER1_COMPA_vect) {
  if(running) {
    tenths++;

    if(tenths >= 10) {
      tenths = 0;
      seconds++;
      
      if(seconds >= 60) {
        seconds = 0;
        minutes++;
      }
    }
  }
}
