#include <Arduino.h>
#include "../lib/MAX17048.hpp"
Adafruit_MAX17048 fuelguage;
void setup() {
  Serial.begin(9600);
  while (!Serial) delay (10);
  fuelguage.quickStart();
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print(fuelguage.cellPercent());
  Serial.print(fuelguage.cellVoltage());
}