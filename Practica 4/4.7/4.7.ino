// Import required libraries
#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include "SPIFFS.h"
#include "time.h"

struct tm timeinfo; //to get the time

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 3600;   //in Spain, we are at GMT+1 (+3600sec = 1h)
const int   daylightOffset_sec = 0;


// Replace with your network credentials
const char* ssid = "RSense";
const char* password = "";


// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

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

String printLocalTime() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time 1");
  }

  char timeStringBuff [35];
  strftime(timeStringBuff, sizeof(timeStringBuff), "%a, %B %d %Y %H:%M:%S", &timeinfo);
  String asString(timeStringBuff);
  return timeStringBuff;
}

// Replaces placeholder with the time
String processTime(String vartime) {
  String strtime;

  if (vartime == "TIME") {
    printLocalTime();
    strtime =  printLocalTime();
    Serial.println(strtime);
    return strtime;
  }
}

void setup() {
  
  Serial.begin(115200);  // Serial port for debugging purposes

  // Initialize SPIFFS
  if (!SPIFFS.begin(true)) {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  Serial.println(WiFi.localIP());  // Print ESP32 Local IP Address


  //Get the time from NTP server:
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    return;
  }
  
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/index.html",  String(), false, processTime);
    printLocalTime();
  });

  // Route to load style.css file
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/style.css", "text/css");
  });


  // Route to set GPIO to TIME
  server.on("/time", HTTP_GET, [](AsyncWebServerRequest * request) {
    setTime(2020, 01, 01, 0, 0, 0, 0);  //function to reset the time to the 1st of January, 2020 (random date)
    request->send(SPIFFS, "/index.html", String(), false, processTime);
  });

  // Start server
  server.begin();

}

void loop() {
  delay(1000);
  Serial.println(printLocalTime()); //debugging purposes
}
