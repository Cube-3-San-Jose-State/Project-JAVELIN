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
            //Transmit data to the Reciever Xbee on the container
        }

        void getData()
        {
            return data;
        }

};