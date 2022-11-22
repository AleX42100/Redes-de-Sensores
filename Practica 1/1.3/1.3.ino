
// Alexandre ORIOL
// Practicas Redes de Sensores, Universidad de Zaragoza, 2022

const int PWM = 27;  //chosen pin for PWM output
int Pot = 12;
int valPot = 0;
int LED = 5;

// Parameters channel 0 of PWM
const int freq = 5000; // X Hz
const int canalPWM = 0;
const int resolution = 12; //  12 bits resolution (4096 for Pot)

void setup() {

  pinMode (Pot, INPUT); // the previously declared pin is an input
  pinMode (LED, OUTPUT); // the previously declared pin is an output
  Serial.begin(115200); // standard serial speed
  ledcSetup(canalPWM, freq, resolution); // function to generate a PWM signal
  ledcAttachPin(PWM, canalPWM);     // links channel 0 to output
}

void loop() {
  valPot = analogRead(Pot); // reading the analog value
  digitalWrite(LED, HIGH);
  Serial.print("Raw Potentiometer value: ");
  Serial.println(valPot); //displaying it in the console..

  delay(50);  //.. each second
  // Duty cycle configuration, according to Pot position
  ledcWrite(canalPWM, valPot);
  digitalWrite(LED, LOW);
  delay(50);  // in order to see the LED
}
