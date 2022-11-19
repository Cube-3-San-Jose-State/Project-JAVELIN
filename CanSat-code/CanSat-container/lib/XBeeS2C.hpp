#pragma once
#include <Wire.h>

class XBeeS2C
{
    private:
        int device_address;
        string data;
        int channel;

    public:
        XBeeS2C(int device_address_, int channel_)
        {
            device_address = device_address_;
            channel = channel_;
        }

        void Intialize()
        {
            Wire.begin();
            Wire.beginTransmission(device_address);
            Wire.endTransmission(true);
        }

        void TransmitData(string data)
        {
            Serial.print('<'); 	//Starting symbol
 	        Serial.print(data);
	        Serial.println('>'); //Ending symbol
        }

        void recieveData()
        {
            bool started = false;
            bool ended = false;
            while(Serial.available() > 0)
            {
                recived = Serial.read();
                
                if (recieved == '<')
                {
                    started = true;
                }

                if (recieved == '>')
                {
                    ended = true;
                }
            }

            if (started == true && ended == true)
            {
                Write(device_address, recieved);
                data = recieved;
            }
        }

        string getData()
        {
            return data;
        }

};