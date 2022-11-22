//a completer, code 1.5 pas encore fait tah l'époque

#include <Arduino.h>
int LED = 5;

void setup() {

  Serial.begin(112500);
  delay(1000);

  xTaskCreate(TaskBlink1, "Blink each 200ms", 10000, NULL, 1, NULL);
  xTaskCreate(HelloWorld, "Hello World!", 10000, NULL, 1, NULL);

}

void loop() {
  delay(1000);
}



void HelloWorld( void * pvParameters)
{
  while (1)
  {
    vTaskDelay( 1000 / portTICK_PERIOD_MS );
    Serial.println("Hello World!");
  }
}


void TaskBlink1(void *pvParameters)
{
  pinMode(5, OUTPUT);
  while (1)
  {
    digitalWrite(5, HIGH);
    vTaskDelay( 200 / portTICK_PERIOD_MS );
    digitalWrite(5, LOW);
    vTaskDelay( 200 / portTICK_PERIOD_MS );
  }
}
