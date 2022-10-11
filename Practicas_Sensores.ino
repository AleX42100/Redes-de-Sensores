//--------------------1°:---------------------------

/*

  int Pot = 12; // pin connected to the potentiometer
  int valPot = 0;

  void setup() {
  pinMode (Pot, INPUT); // the previously declared pin is an input
  Serial.begin(115200); // standard speed
  }

  void loop() {
  valPot = analogRead(Pot); // reading the analog value
  Serial.print("Raw Potentiometer value: ");
  Serial.println(valPot); //displaying it in the console..
  delay(1000);  //.. each second
  }
*/

//--------------------2°:----------------------------


/*

  #include <Adafruit_GFX.h>
  #include <Adafruit_SSD1331.h>
  #include <SPI.h>  // necessary libraries to make the OLED screen work

  #define sclk 14
  #define mosi 13
  #define miso 12
  #define cs   15
  #define rst  4
  #define dc   16  // pins according to the ESP32

  // Color definitions
  #define  BLACK          0x0000
  #define BLUE            0x001F
  #define RED             0xF800
  #define GREEN           0x07E0
  #define CYAN            0x07FF
  #define MAGENTA         0xF81F
  #define YELLOW          0xFFE0
  #define WHITE           0xFFFF

  // declaration of the screen :
  Adafruit_SSD1331 display = Adafruit_SSD1331(cs, dc, mosi, sclk, rst); 

  // interruption mechanism
  volatile int interruptCounter;
  int totalInterruptCounter;

  hw_timer_t * timer = NULL;
  portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

  void IRAM_ATTR onTimer() {
  portENTER_CRITICAL_ISR(&timerMux);
  interruptCounter++;
  portEXIT_CRITICAL_ISR(&timerMux);

  }

  int Pot = 12;
  int valPot = 0;

  void setup() {
  pinMode (Pot, INPUT);
  Serial.begin(115200);
  
  //  Testing phase of the screen
  display.begin();
  display.fillScreen(RED);
  delay(100);
  display.fillScreen(GREEN);
  delay(100);
  display.fillScreen(BLUE);
  delay(100);
  display.fillScreen(BLACK);

  //  REMINDER --> OTHER OLED FUNCTIONS :
  //  display.setTextSize(2);
  //  display.setTextColor(RED);
  //  display.fillRect(10,40, 75, 20, RED);
  //  display.setCursor(20,47);
  //  display.setTextSize(1);
  //  display.print("Bonjour la France");
  //  display.drawRect(0,0,96,64,WHITE);

  // interrupt mechansim :
  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, 10000000, true);
  timerAlarmEnable(timer);

  }

  void loop() {

  if (interruptCounter > 0) {

    portENTER_CRITICAL(&timerMux);
    interruptCounter--;
    portEXIT_CRITICAL(&timerMux);

    totalInterruptCounter++;

    Serial.print("An interrupt as occurred. Total number: ");
    Serial.println(totalInterruptCounter);

    valPot = analogRead(Pot); // reading the analog value
    Serial.print("Raw Potentiometer value: ");
    Serial.println(valPot); //displaying it in the console..

    display.fillScreen(BLACK);
    display.setCursor(20,5);
    display.setTextColor(WHITE);
    display.setTextSize(2);
    display.print(valPot);  //display the pot value on hte oled in white, black background

  }
  }
*/

//---------------------3° :-----------------------------

const int PWM = 27;  //chosen pin for PWM output
int Pot = 12;
int valPot = 0;

// Parameters channel 0 of PWM
const int freq = 5000; // X Hz
const int canalPWM = 0;
const int resolution = 12; //  12 bits resolution (4096 for Pot)

void setup(){

    pinMode (Pot, INPUT); // the previously declared pin is an input
    Serial.begin(115200); // standard speed
    ledcSetup(PWM, freq, resolution); // function to generate a PWM signal
    ledcAttachPin(PWM, canalPWM);     // links channel 0 to output
}

void loop(){
  valPot = analogRead(Pot); // reading the analog value
  Serial.print("Raw Potentiometer value: ");
  Serial.println(valPot); //displaying it in the console..
  
  delay(1000);  //.. each second
  // Duty cycle configuration, according to Pot position
  ledcWrite(canalPWM, valPot);
}
