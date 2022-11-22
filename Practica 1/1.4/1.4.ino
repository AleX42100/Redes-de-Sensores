// Alexandre ORIOL
// Practicas Redes de Sensores, Universidad de Zaragoza, 2022

String cmd;
const int PWM = 27;  //chosen pin for PWM output
int arg1 = 0, nb_arg = 0;
int valPot = 0;
int Pot = 12;

// Parameters channel 0 of PWM
const int freq = 5000; // X Hz
const int canalPWM = 0;
const int resolution = 12; //  12 bits resolution (4096 for Pot)


int customTime = 1000000;

volatile int interruptCounter;
int totalInterruptCounter;

hw_timer_t * timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

void IRAM_ATTR onTimer() {
  portENTER_CRITICAL_ISR(&timerMux);
  interruptCounter++;
  portEXIT_CRITICAL_ISR(&timerMux);
  valPot = analogRead(Pot); // reading the analog value

}


void readADC()
{
  if (cmd.indexOf("ADC") >= 0)  //si la commande rentrée commence par ADC
  {
    if (nb_arg == 1)  //si il y a qqch entre parenthèses
    {
      customTime = 1000000 * arg1;

      while (Serial.available() == 0 && arg1 != 0)
      {
        timerAlarmWrite(timer, customTime, true);
        timerAlarmEnable(timer);
        if (interruptCounter > 0)
        {
          portENTER_CRITICAL(&timerMux);
          interruptCounter--;
          portEXIT_CRITICAL(&timerMux);

          Serial.print("Raw Potentiometer value: ");
          Serial.println(analogRead(Pot)); //displaying Pot in the console
        }
      }
    }

    else if (nb_arg == 0)
    {
      timerWrite(timer, 0);
      valPot = analogRead(Pot); // reading the analog value
      Serial.print("Raw Potentiometer value: ");
      Serial.println(valPot); //displaying it in the console
    }


    if (cmd.indexOf("PWM") >= 0)
    {
      if (nb_arg == 1)
      {
        int duty = map(arg1, 0, 9, 0, 4095);
        ledcWrite(canalPWM, duty);
      }

    }
  }
}
void setup() {
  Serial.begin(115200);
  ledcSetup(canalPWM, freq, resolution); // function to generate a PWM signal
  ledcAttachPin(PWM, canalPWM);     // links channel 0 to output
  // interrupt mechansim :
  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &onTimer, true);
}


void loop()
{
  if (Serial.available() > 0) // Si communication sur port Série
  {
    // int arg1 = 0, arg2 = 0, nb_arg = 0;
    cmd = Serial.readStringUntil(0x0a); // On récupère la chaine
    Serial.setTimeout(50);  // temps d'attente de la donnée

    if (cmd.indexOf("(") >= 0) // On récupère 0 ou 1 argument
    {
      if (cmd.indexOf("(") == cmd.lastIndexOf("("))
      {
        arg1 = cmd.substring(cmd.indexOf("(") + 1).toInt(); //valeur de ce paramètre
        nb_arg = 1; //il y a un paramètre entre parenthèses après la commande
        interruptCounter = 0; //to avoid multiple reads when going from ADC(0) to acquiring some data
      }
    }

    Serial.print("Commande saisie: "); Serial.println(cmd);   //debug, but useful
    readADC();

  }
}
