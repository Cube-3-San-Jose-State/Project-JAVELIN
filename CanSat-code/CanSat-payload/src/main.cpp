#include <Arduino.h>
#include <Wire.h>
#include "../lib/PA1616S.hpp"
#include "../lib/MPL3115A.hpp"

PA1616S gps;
MPL3115A altimeter;

void setup() {
	Serial.begin(9600);
	while (!Serial) delay(10);
	Serial.println("gps initializing...");
	gps.initialize();
	Serial.println("gps initialized!");
	
	Serial.println("altimeter initializing...");
	altimeter.initialize();
	Serial.println("altimeter initialized!");
}

void loop() {
	delay(500);
	gps.update();
	
	Serial.print("Longitude: ");
	Serial.println(gps.getLongitude(), 6);
	Serial.print("Latitude: ");
	Serial.println(gps.getLatitude(), 6);

	altimeter.update();
	Serial.print("temp: ");
	Serial.print(altimeter.getTemperature());
	Serial.println(" F");
	Serial.print("altitude: ");
	Serial.print(altimeter.getAltitude());
	Serial.println(" m");
	Serial.print("pressure: ");
	Serial.print(altimeter.getPressure());
	Serial.println(" kPa");
	Serial.println("---");


}