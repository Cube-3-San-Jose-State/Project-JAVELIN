#include <Arduino.h>

// using namespace CanSat; // This is not needed for this demo but is needed for the CanSat driver code for your devices
// run platformio run to compile and platformio run --target upload to upload to your board

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("Hello World");
  delay(1000);
}