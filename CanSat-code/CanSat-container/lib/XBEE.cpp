#pragma once
#include <SoftwareSerial.h>
#include <Wire.h>

#define RX_pin 12
#define TX_pin 13

namespace CanSat
{
    SoftwareSerial receiverSerial(RX_pin, TX_pin);
    String c = "";

        void Initialize()
        {
            Serial.begin(9600);
            receiverSerial.begin(9600);
            
        }

        void transmitData()
        {
            if (Serial.available())
            {
                receiverSerial.write(Serial.read());
            }
        }

        void receiveData()
        {
            if (receiverSerial.available() > 0)
            {
                c = receiverSerial.readString();
                Serial.println(c);
                receiverSerial.println(c);
            }
        }
}