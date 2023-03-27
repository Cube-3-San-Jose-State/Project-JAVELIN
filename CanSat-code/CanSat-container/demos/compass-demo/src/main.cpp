/**************************************************************************/
/*!
    @file     Adafruit_MPL3115A2.cpp
    @author   K.Townsend (Adafruit Industries)
    @license  BSD (see license.txt)

    Example for the MPL3115A2 barometric pressure sensor

    This is a library for the Adafruit MPL3115A2 breakout
    ----> https://www.adafruit.com/products/1893

    Adafruit invests time and resources providing this open source code,
    please support Adafruit and open-source hardware by purchasing
    products from Adafruit!

    @section  HISTORY

    v1.0 - First release
*/
/**************************************************************************/

#include "../../../lib/LSM303AGR.hpp"

using namespace CanSat;

LSM303AGR compass(25, 24);

void setup() {
  Serial.begin(9600);
  compass.Initialize();
}

void loop() {
  compass.Update();
  Serial.print("Magnetometer: ");
  Serial.print(compass.GetMagneticField().x);
  Serial.print(", ");
  Serial.print(compass.GetMagneticField().y);

}