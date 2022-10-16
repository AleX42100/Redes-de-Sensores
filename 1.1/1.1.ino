// Alexandre ORIOL
// Practicas Redes de Sensores, Universidad de Zaragoza, 2022


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
  Serial.print("Voltage: ");
  Serial.print((3.3 * valPot) / 4096);
  Serial.println(" V");
  delay(1000);  //.. each second
}
