// Same function as 1.5, but using RTOS

#include <Arduino.h>
#include <MPU9250_asukiaaa.h>
#include <Wire.h>
#ifdef _ESP32_HAL_I2C_H_
#define SDA_PIN 21
#define SCL_PIN 22
#endif

MPU9250_asukiaaa mySensor;    //MPU sensor

int LED = 5;
float tabData[10];  //table of 10 values of acceleration
int i = 0;  // id of the column

void setup() {

  while (!Serial);

  Serial.begin(115200);
  Serial.println("started");
  pinMode(5, OUTPUT);   // green embedded LED
  digitalWrite(5, HIGH);    //HIGH : LED OFF/LOW : LED ON

#ifdef _ESP32_HAL_I2C_H_
  Wire.begin(SDA_PIN, SCL_PIN); //sda, scl
#else
  Wire.begin();
#endif

  mySensor.setWire(&Wire);
  mySensor.beginAccel();  // only the accelerometer

  xTaskCreate(fillTable, "fill and print the accel values", 10000, NULL, 1, NULL);

}

void loop() {
}



void fillTable( void * pvParameters)
{
  while (1)
  {
    for ( i = 0; i < 10; i++) //we want 10 values, one every 100ms for a total duration of 1s
    {
      mySensor.accelUpdate();
      tabData[i] = float(mySensor.accelSqrt());
      vTaskDelay( 100 / portTICK_PERIOD_MS );
      if (i == 1)     digitalWrite(5, HIGH); //to get the correct timing at the end
    }

    digitalWrite(5, LOW);
    Serial.println("New Dataset: ");

    for (i = 0; i < 10; i++) // printing the full table
    {
      Serial.print("i : "); Serial.print(i); Serial.print(" --> "); Serial.println(tabData[i]);
    }
  }
}
