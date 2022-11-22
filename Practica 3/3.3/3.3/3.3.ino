#include <MPU9250_asukiaaa.h>
#include <Wire.h> 
#ifdef _ESP32_HAL_I2C_H_
#define SDA_PIN 21
#define SCL_PIN 22
#endif
 
MPU9250_asukiaaa mySensor;

void setup() {
while(!Serial);
 
Serial.begin(115200);
Serial.println("started");
 
#ifdef _ESP32_HAL_I2C_H_
// for esp32
Wire.begin(SDA_PIN, SCL_PIN); //sda, scl
#else
Wire.begin();
#endif
 
mySensor.setWire(&Wire);
 
mySensor.beginAccel();  // only the accelerometer, it will be easier

 
// you can set your own offset for mag values
// mySensor.magXOffset = -50;
// mySensor.magYOffset = -55;
// mySensor.magZOffset = -10;
}
 
void loop() {
mySensor.accelUpdate();
Serial.println(float(mySensor.accelX()));
Serial.println(float(mySensor.accelY()));
Serial.println(float(mySensor.accelZ()));
Serial.println(float(mySensor.accelSqrt()));

Serial.println(millis());
delay(500);
}
