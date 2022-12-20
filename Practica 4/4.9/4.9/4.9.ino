// In ths code, I can't understand why the potntiometer value is always 0, 
// It seems to work though.. trying with random value
#include "config.h"

int valPot = 11;
int Pot = 12;
int number = 0;

// set up the 'Potentiometer' feed
AdafruitIO_Feed *feed = io.feed("potentiometer");

void setup() {

  // start the serial connection
  Serial.begin(115200);
  pinMode (Pot, INPUT); // the previously declared pin is an input

  
  // wait for serial monitor to open
  while(! Serial);
  
  Serial.print("Connecting to Adafruit IO");

  // connect to io.adafruit.com
  io.connect();

  // wait for a connection
  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  // we are connected
  Serial.println();
  Serial.println(io.statusText());

}

void loop() {

  // io.run(); is required for all sketches.
  // it should always be present at the top of your loop
  // function. it keeps the client connected to
  // io.adafruit.com, and processes any incoming data.
  //pinMode (Pot, INPUT); // the previously declared pin is an input
  //valPot = analogRead(Pot); // reading the analog value
  //Serial.println(valPot);
  io.run();

  number = random(2,20);
  
  Serial.print("sending -> ");
  //Serial.println(valPot); // doesn't work :(
  Serial.println(number);
  feed->save(number);

  // Adafruit IO is rate limited for publishing, so a delay is required in
  // between feed->save events. In this example, we will wait three seconds
  // (1000 milliseconds == 1 second) during each loop.
  delay(3000);

}
