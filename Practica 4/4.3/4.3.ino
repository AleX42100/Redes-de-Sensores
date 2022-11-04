#include <WiFi.h>
/* change ssid and password according to yours WiFi*/
const char* ssid     = "RSense";
const char* password = "";
String cmd;

/*
 * This is the IP address of your PC
 * [Wins: use ipconfig command, Linux/MAC: use ifconfig command]
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
}
void loop()
{
    /* Use WiFiClient class to create TCP connections */
    WiFiClient client;
    
    if (!client.connect(host, port)) {
        Serial.println("connection failed");
        return;
    }
    /* This will send the data to the server */
    if (Serial.available() > 0) // If communication via Serial:
  {
    cmd = Serial.readStringUntil(0x0a); // We get the string chain
    Serial.setTimeout(50);
    
    client.println(cmd);  //
    client.stop();
    }
}
