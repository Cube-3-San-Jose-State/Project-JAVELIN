#include <Arduino.h>
#include "../lib/mission-control-handler.hpp"

using namespace CanSat;

MissionControlHandler mission_control_handler;

void setup() {
    Serial.begin(9600);

}

void loop() {
    Serial.println("Hello World");
    delay(1000);

}

//Rules Engine