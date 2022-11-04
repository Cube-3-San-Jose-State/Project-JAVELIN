#pragma once
#include <Wire.h>
namespace CanSat
{
    class MiniSpyCamera
    {
        /*Connect the red wire to 3.7V-5VDC and the black wire to ground. Then use the white wire to control the camera. When touched to ground for under about 100ms, the camera will take a photo and stick it in a PHOTO folder on the SD card, numbered from PIC000.jpg up to PIC999.jpg.*/
        private:
            int signal_pin = 0x00;
            double voltage;
            double ground;
            
            
        void Initialize()
        {
            Wire.begin();
            Wire.beginTransmission(signal_pin);
            Wire.endTransmission(true);
        }

        void ReadMPL() {
            Wire.beginTransmission(signal_pin);
        }
        
        int GetSignalPin()
        {
            return signal_pin;
        }

        double GetVoltage()
        {
            return voltage;
        }

        double GetGround()
        {
            return ground;
        }
    };
}