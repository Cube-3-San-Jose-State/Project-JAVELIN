#include <Arduino.h>
#include "../include/mission-control-handler.hpp"
#include "../include/deploy-payload.hpp"
#include "../include/container-dto.hpp"

using namespace CanSat;

MissionControlHandler mission_control_handler;
Container_Data container_data;
PayloadDeployment payload_deployment(9, 1000);

void setup() {
    Serial.begin(9600);

}

void loop() {
    container_data = payload_deployment.Deploy(container_data);
    Serial.println("Hello World");
    delay(1000);

}

//Rules Engine