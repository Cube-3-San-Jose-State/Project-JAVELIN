#include <Arduino.h>
#include "E:\CanSat-2022\CanSat-code\CanSat-container\lib\XBeeS2C.hpp"

int led = 13; 
int recieved = 0;

XBeeS2C xbee_radio_transmit(0x68, 0x01);

int i;

void setup() {
	
  Serial.begin(9600);
  pinMode(led, OUTPUT);
  xbee_radio_transmit.Intialize();
  
  //radioPowerUp();
  //setVolume(30);
}

void loop() {

  xbee_radio_transmit.TransmitData("Hello World");

  while(Serial.available() > 0)
  {
    recived = Serial.read();
    // space issue?
  }

  if (!recieved.empty())
  {
    digitalWrite(led, HIGH);
    delay(2000);
    digitalWrite(led, LOW);
  }
}