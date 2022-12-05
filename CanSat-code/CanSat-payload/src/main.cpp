#include <Arduino.h>
#include "lib/MiniSpyCamera.hpp"
#include "lib/MPL3115A.hpp"
#include "lib/MPU6050.hpp"
#include "lib/XBEE.hpp"

MiniSpyCamera camera(2);
MPL3115A barometer;
MPU6050 IMU;
XBEE xbee_transmitter(12, 13);

void setup() {

}

void loop() {

}