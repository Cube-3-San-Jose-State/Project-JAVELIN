#include <Arduino.h>
#include "lib/PA1616S.hpp"
#include "lib/MPL3115A.hpp"
#include "lib/MPU6050.hpp"
#include "lib/XBEE.hpp"

PA1616S GPS(); //Pins?
MPL3115A barometer;
MPU6050 IMU;
XBEE xbee_receiver(12, 13);
XBEE xbee_transmitter(12, 13);

using namespace CanSat

void setup() {

}

void loop() {

}