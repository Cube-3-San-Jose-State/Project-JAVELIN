#include <Arduino.h>
#include "../lib/PA1616S.hpp"

PA1616S gps;
void setup() {
	Serial.begin(9600);
	while (!Serial) delay(10);
	gps.initialize();
}

void loop() {
	gps.update();

	Serial.print("Longitude: ");
	Serial.println(gps.getLongitude());
	Serial.print("Latitude: ");
	Serial.println(gps.getLatitude());
}