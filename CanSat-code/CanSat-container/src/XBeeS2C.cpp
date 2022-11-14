#pragma once
#include <Wire.h>

class XBeeS2C
{
    private:
        int device_address;
        string data;
        int channel;

    public:
        void Intialize()
        {
            Wire.begin();
            Wire.beginTransmission(device_address);
            //Write.write(
            Wire.endTransmission(true); 
        }
        void readData()
        {

        }
        void getData()
        {
            return data;
        }
        void getChannel()
        {
            return channel;
        }
};