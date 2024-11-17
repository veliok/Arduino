#include <Arduino_FreeRTOS.h>

int value = 0;

void Read(void *pvParameters) {
  for(;;) {
    if (Serial.available() > 0) {
      int val = Serial.parseInt();
      value += val;           
      Serial.print("Lisätty arvo: ");
      Serial.println(value);
    }
  }
}

void Decrement(void *pvParameters) {
  for(;;) {
     if (value > 0) {
      value--;
      Serial.print("Muuttujan arvo: ");
      Serial.println(value);
    }
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

void setup() {
  Serial.begin(9600);

  xTaskCreate(Read, "Read", 128, NULL, 1, NULL);
  xTaskCreate(Decrement, "Decrement", 128, NULL, 1 , NULL);

}

void loop() {

}
