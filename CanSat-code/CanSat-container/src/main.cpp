#include <Arduino.h>

int led = 13; 
int recieved = 0;

int i;

void setup() {
	
  Serial.begin(9600);
  pinMode(led, OUTPUT);
  
  //radioPowerUp();
  //setVolume(30);
}

void loop() {

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