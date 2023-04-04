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
  Serial.print(", ");
  Serial.println(compass.GetMagneticField().z);

  Serial.print("Heading: ");
  Serial.println(compass.GetHeading());
  delay(1000);

}