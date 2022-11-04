#include <WiFi.h>
#include "time.h"
/* change ssid and password according to yours WiFi*/
const char* ssid     = "RSense";
const char* password = "";
String cmd;
bool test=false;

struct tm timeinfo; //to get the time


const char* ntpServer = "pool.ntp.org"; // Get the time part:
const long  gmtOffset_sec = 3600;   //in Spain, we are at GMT+1 (+3600sec = 1h)
const int   daylightOffset_sec = 0;

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

  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
}

void loop()
{
  /* Use WiFiClient class to create TCP connections */
  WiFiClient client;

  if (!client.connect(host, port)) {
    Serial.println("connection failed");
    return;
  }

  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    return;
  }

  if (Serial.available() > 0) // Si communication sur port Série
  {
    cmd = Serial.readStringUntil(0x0a); // On récupère la chaine
    Serial.setTimeout(50);  // temps d'attente de la donnée
    Serial.print(cmd);
  }
    if (cmd.indexOf("stop") >= 0) //if stop is written in the console, we stop sending the time
    {

      client.stop();
      test=false;
    }
    if (cmd.indexOf("start") >= 0 || test==true)
    {
      Serial.println(" : sending the time each second!");
   test=true;
   client.println(&timeinfo, "%a, %b %d %Y %H:%M:%S");
   client.stop();
   delay(1000);
    }

  }

  
