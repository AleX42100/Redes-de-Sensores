/* 
 *  This code collects the values of gyroscope in the MPU9250 I2C sensor
 *  It Sends a .json file over the network to my computer every 2sec with 10 points per second
 *  Each time a file is sent, the green LED blinks
 *  Also, the results based on the average of the gyroscope is displayed on the embedded screen
*/

#include <MPU9250_asukiaaa.h>
#include <Wire.h>
#include <WiFi.h>
#include "ArduinoJson.h"    //for FTP server
#include <ESP32_FTPClient.h>
#include <WiFiClient.h>
#include <Adafruit_GFX.h>   //for OLED screen
#include <Adafruit_SSD1331.h>
#include <SPI.h>


#define MPU_ADDRESS 0x68  // I2C address of the MPU-6050
#ifdef _ESP32_HAL_I2C_H_
#define SDA_PIN 21
#define SCL_PIN 22
#endif

#define sclk 14   // pins for OLED 
#define mosi 13
#define miso 12
#define cs   15
#define rst  4
#define dc   16


// OLED color definitions RGB545 format (5,6,5)
#define  BLACK          0x0000
#define BLUE            0x001F
#define RED             0xE800
#define GREEN           0x07E0
#define CYAN            0x07FF
#define MAGENTA         0xF81F
#define YELLOW          0xFFE0
#define WHITE           0xFFFF
#define ORANGE          0xFBE0


 int NUMBER_ACQ = 20;
int INTERVAL_ACQ = 100;
int PIN_LED = 5;

// Time section:
struct tm timeinfo; //to get the time
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 3600;   //in Spain, we are at GMT+1 (+3600sec = 1h)
const int   daylightOffset_sec = 0;

// FTP Config:
char ftp_server[] = "192.168.0.150";   //check ip adress via the Mac Terminal : 'ifconfig -a'
char ftp_user[]   = "ESP32";
char ftp_pass[]   = "1TRe100";
ESP32_FTPClient ftp (ftp_server, ftp_user, ftp_pass, 5000, 2);

// WiFi network name and password:

/*  // in my appartment:
const char* networkName = "Tenda_DF9160_5G_EXT2.4G";
const char* networkPswd = "soulteam204";
*/
// At the University:
const char* networkName = "RSense";
const char* networkPswd = "";

Adafruit_SSD1331 display = Adafruit_SSD1331(cs, dc, mosi, sclk, rst); // OLED declaration

MPU9250_asukiaaa mySensor;  //MPU declaration
float tabData[50];  //table of 10 values of acceleration
int i = 0;  // id of the column

float aX, aY, aZ, sqrtAccel;
float gX, gY, gZ, magAbs;
float totalAvg = 0.0;


//-------------Functions related to the Time-----------------

String printLocalTime() { //to display the actual time
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time :(");
  }

  char timeStringBuff [35];
  strftime(timeStringBuff, sizeof(timeStringBuff), "%b %d %H %M %S", &timeinfo);  //Month DD HH SS
  String asString(timeStringBuff);
  return timeStringBuff;
}

void setTime(int yr, int month, int mday, int hr, int minute, int sec, int isDst) // to set the time 
{
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

//------------------Config FIlters MPU------------------------

void setupMpu9250Registers() {

  // Configure low pass filter
  Wire.beginTransmission(MPU_ADDRESS); // Start communication with MPU
  Wire.write(0x1A);                    // Request the CONFIG register
  Wire.write(0x03);                    // Set Digital Low Pass Filter about ~43Hz
  Wire.endTransmission();              // End the transmission
}

//-------------------------WIFI--------------------------

void connectToWiFi(const char * ssid, const char * pwd)
{

  Serial.println("Connecting to WiFi network: " + String(ssid));

  WiFi.begin(ssid, pwd);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

//-------------------Sensor Update-----------------------
void sensorUpdate() //we will be using only the gyroscope at the end
{
  //mySensor.accelUpdate();
  mySensor.gyroUpdate();  //update the values of the gyroscope

  //aX = mySensor.accelX(); aY = mySensor.accelY(); aZ = mySensor.accelZ();

  //sqrtAccel = mySensor.accelSqrt();

  gX = mySensor.gyroX(); gY = mySensor.gyroY(); gZ = mySensor.gyroZ();

  magAbs = sqrt(pow(gX, 2) + pow(gY, 2) + pow(gZ, 2));  //square root of the quadratic sum of the 3 axis

}


//--------------Json File Generation-------------

void jsonFileGen()

{
  String stringJson;
  char charJson[2048];
  DynamicJsonDocument doc(2048);  //configure the Json document
  String docName1 = "MPU_";
  //String docName2 = docName1+"_";
  String docName = docName1 + printLocalTime() + ".json";
  Serial.println(docName);   //debug

  char charName[30];
  docName.toCharArray(charName, 30);    //conversion to char in order to ba able to write on the FTP server
  ftp.InitFile("Type A");
  ftp.NewFile(charName);
}

//---------------Display results on the oled screen--------------------

void oledScreenResults(float totalAvg)
{
  Serial.println(totalAvg);
  if (totalAvg < 50) // Standing, deduced from Excel graphs
  {
    Serial.println("Standing Still");
    display.fillScreen(GREEN);
    display.print("Standing");
  }
  else if (totalAvg > 50 && totalAvg < 150) // Walking,  deduced from Excel graphs
  {
    Serial.println("Walking");
    display.fillScreen(ORANGE);
    display.print("Walking");
  }
  else if (totalAvg > 150) // Running like a champ! Deduced from Excel graphs
  {
    Serial.println("Running");
    display.fillScreen(RED);
    display.print("Running");
  }
}

//==========================SETUP==============================
void setup() {

  setupMpu9250Registers();
  while (!Serial);

  Serial.begin(115200);
  display.begin();

  // Connect to Wi-Fi
  connectToWiFi(networkName, networkPswd);
  ftp.OpenConnection(); //enable FTP

  //Get the time from NTP server:
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    return;
  }
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  pinMode(PIN_LED, OUTPUT);   // green embedded LED
  digitalWrite(PIN_LED, HIGH);    //HIGH : LED OFF/LOW : LED ON

  // I2C protocol starting procedure
#ifdef _ESP32_HAL_I2C_H_
  Wire.begin(SDA_PIN, SCL_PIN); //sda, scl
#else
  Wire.begin();
#endif

  mySensor.setWire(&Wire);
  mySensor.beginAccel(ACC_FULL_SCALE_16_G);
  //mySensor.beginMag();
  mySensor.beginGyro(GYRO_FULL_SCALE_2000_DPS);
}

//=============================LOOP================================
void loop() {
  
  String stringJson;
  char charJson[2048];
  DynamicJsonDocument doc(2048);  //configure the Json document
  jsonFileGen();  //creation of a new Json file with the correct naming

  for (int i = 0; i < NUMBER_ACQ; i++) // each file : 20 samples, taken each 100ms, for a 2s sampling time
  {
    if (i == 1)
    {
      digitalWrite(PIN_LED, LOW); //LED ON to see each time when we are in the loop
    }

    sensorUpdate();   //update the sensors, get the values on the 3 axis for gyro and accel (not used here)

    Serial.print("sqrtGyro: ");
    Serial.println(magAbs);

    // JSON to FTP server part
    doc["id"] = i;  //n° of the line
    
    // doc["Time"] = printLocalTime();  //used before, it's harder to open properly with Excel after..
    //  doc["Sensor"] = "Mag";
    
    doc["Value"]   = magAbs; //value we want to send (may vary depending on the selected sensor)

    serializeJson(doc, stringJson);
    stringJson = stringJson.substring(0, stringJson.length() - 1); //delete the } at the end
    stringJson.concat('\n');
    stringJson.toCharArray(charJson, 2048);    //conversion to char to ba able to write on the FTP server

    Serial.print(stringJson); //debug
    
    stringJson.toCharArray(charJson, 2048);    //conversion to char to ba able to write on the FTP server


    totalAvg = totalAvg + magAbs; //sum of all the values 
    delay (INTERVAL_ACQ); //every sec, a new line is generated
  }

  display.setCursor(0, 32); //original position of the cursor 
  display.setTextColor(WHITE);  // to be seen properly with all the 3 colors
  display.setTextSize(2); // big enough


  totalAvg = totalAvg / NUMBER_ACQ; // average calculation
  oledScreenResults(totalAvg);  // display the result based on the average value

  digitalWrite(PIN_LED, HIGH);  //LED OFF : we are going to send a file

    ftp.Write(charJson);  //writing to the file
    ftp.CloseFile();  //close the file, we are done!
  

}
