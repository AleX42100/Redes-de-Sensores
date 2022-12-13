// Import required libraries
#include "WiFi.h"
#include "time.h"
#include "ArduinoJson.h"
#include <ESP32_FTPClient.h>
#include <WiFiClient.h> 


struct tm timeinfo; //to get the time

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 3600;   //in Spain, we are at GMT+1 (+3600sec = 1h)
const int   daylightOffset_sec = 0;

// Replace with your network credentials
const char* ssid = "RSense";
const char* password = "";

char ftp_server[] = "155.210.150.77";
char ftp_user[]   = "rsense";
char ftp_pass[]   = "rsense";

// you can pass a FTP timeout and debbug mode on the last 2 arguments
ESP32_FTPClient ftp (ftp_server,ftp_user,ftp_pass, 5000, 2);


String printLocalTime() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time 1");
  }

  char timeStringBuff [35];
  strftime(timeStringBuff, sizeof(timeStringBuff), "%d %b %S", &timeinfo);
  String asString(timeStringBuff);
  return timeStringBuff;
}

void setTime(int yr, int month, int mday, int hr, int minute, int sec, int isDst) {
  struct tm tm;

  tm.tm_year = yr - 1900;   // Set date
  tm.tm_mon = month - 1;
  tm.tm_mday = mday;
  tm.tm_hour = hr;      // Set time
  tm.tm_min = minute;
  tm.tm_sec = sec;
  tm.tm_isdst = isDst;  // 1 or 0
  time_t t = mktime(&tm);
  Serial.printf("Setting time: %s", asctime(&tm));
  struct timeval now = { .tv_sec = t };
  settimeofday(&now, NULL);
}


void setup() {
  Serial.begin(115200);  // Serial port for debugging purposes
 
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  Serial.println(WiFi.localIP());  // Print ESP32 Local IP Address
  ftp.OpenConnection();



  //Get the time from NTP server:
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    return;
  }

  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);


}

void loop() {
  
  String stringJson;
  DynamicJsonDocument doc(1024);  //configure the Json document
  doc["Time"] = printLocalTime();
  doc["Sensor"] = "Temperature";
  doc["Value"]   = random(0, 20); //generation of a random plausible temperature value
  doc["Unit"]   = "°C";   // or °F if you are a little weird
  serializeJson(doc, stringJson); //debug
  
  char charJson[200];
  stringJson.toCharArray(charJson, 200);    //conversion to char in order to ba able to write on the FTP server

  String docName1 = "876794";
  String docName2 = docName1+"_";
  String docName = docName2+printLocalTime()+".json";
  Serial.println(docName);   //debug
  
  char charName[200];
  docName.toCharArray(charName, 200);    //conversion to char in order to ba able to write on the FTP server


  
  ftp.ChangeWorkDir("/rsense/876794");
  ftp.InitFile("Type A");
  ftp.NewFile(charName);
  ftp.Write(charJson);
  ftp.CloseFile();
  delay(10000); //every 10sec, a new line is generated

  //ftp.CloseConnection();


}
