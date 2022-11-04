#include <WiFi.h>
#include "time.h"

const char* ssid       = "RSense";   //Name of the Router's network
const char* password   = "";  //RSense does not have a password

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 3600;   //in Spain, we are at GMT+1 (+3600sec = 1h)
const int   daylightOffset_sec = 0;  

void setup()
{
  Serial.begin(115200);
  
  //connect to WiFi
  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");  // loading animation
  }
  Serial.println("CONNECTED to WIFI");
  
  //init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  printLocalTime();

  //disconnect WiFi : we already have what we need
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
}

void loop()
{
  delay(1000);
  printLocalTime();
}
void printLocalTime()
{
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println(&timeinfo, "%a, %b %d %Y %H:%M:%S");
/*
%A  Full weekday name Saturday
%a  Abbreviated weekday name  Sat
%B  Full month name January
%b or %h  Abbreviated month name  Jan
%D  Short MM/DD/YY date 09/12/07
%d  Day of the month (01-31)  12
%F  Short YYYY-MM-DD date 2007-09-12
%H  The hour in 24-hour format (00-23)  13
%I  The hour in 12-hour format (01-12)  08
%j  Day of the year (001-366) 78
%Y  Year  2021
%y  Last two digits of the year (00-99) 21
%m  Month as a decimal number (01-12) 02
%M  Minute (00-59)  12
%p  AM or PM  AM
%r  12-hour clock time  05:12:32 am
%R  24-hour time HH: MM 13:22
%S  Second (00-59)  32
%T  Time format HH: MM: SS  09:12:01
*/
}
