#include <WiFi.h>
#include <MPU9250_asukiaaa.h>
#include <Wire.h>
#ifdef _ESP32_HAL_I2C_H_
#define SDA_PIN 21
#define SCL_PIN 22
#endif

MPU9250_asukiaaa mySensor;

/* change ssid and password according to yours WiFi*/
const char* ssid     = "RSense";
const char* password = "";

/*
   This is the IP address of your PC
   [Wins: use ipconfig command, Linux/MAC: use ifconfig command]
*/
const char* host = "192.168.0.114";
const int port = 23;

void setup()
{
  Serial.begin(115200);
  Serial.print("Connecting to ");
  Serial.println(ssid);
  /* connect to your WiFi */
  WiFi.begin(ssid, password);
  /* wait until ESP32 connect to WiFi*/
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected with IP address: ");
  Serial.println(WiFi.localIP());
  
#ifdef _ESP32_HAL_I2C_H_
  // for esp32
  Wire.begin(SDA_PIN, SCL_PIN); //sda, scl
#else
  Wire.begin();
#endif

mySensor.setWire(&Wire);
 
mySensor.beginAccel();  // only the accelerometer, it will be easier

}
void loop()
{
  /* Use WiFiClient class to create TCP connections */
  WiFiClient client;
      if (!client.connect(host, port)) {
        Serial.println("connection failed");
        return;
    }

mySensor.accelUpdate();
client.print("Acceleration X : ");
client.println(String(mySensor.accelX()));
client.print("Acceleration Y : ");
client.println(String(mySensor.accelY()));
client.print("Acceleration Z : ");
client.println(String(mySensor.accelZ()));
//client.stop();
delay(1000);
  
}
