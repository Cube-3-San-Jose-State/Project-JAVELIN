#pragma once
#include <Arduino.h>
#include <Wire.h>

namespace CanSat
{
    class MPL3115A2
    {
        private:
            int device_address = 0x60;
            double pressure;
            double altitude;
            double temperature;
            
            
        void Initialize()
        {
            Wire.begin();
            Wire.beginTransmission(device_address);
            Wire.write(0x60);
            Wire.write(0);
            Wire.endTransmission(true);
        }

        void ReadMPL() {
            Wire.beginTransmission(device_address);
            Wire.endTransmission(false);
            Wire.requestFrom(device_address,16,true);
            pressure = Wire.read() << 8 | Wire.read();
            altitude = Wire.read() << 8 | Wire.read();
            temperature = Wire.read() << 8 | Wire.read();
        }
        
        double GetPressure()
        {
            return pressure;
        }

        double GetAltitude()
        {
            return altitude;
        }

        double GetTemp()
        {
            return temperature;
        }
    };
}