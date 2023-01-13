#pragma once
#include <map>
#include <Wire.h>

#define DEVICE_ADDRESS 0x60
#define CONTROL_REGISTER 0x26
#define DATA_REGISTER 0x13
#define SELECT_DATA_REGISTER 0x00

#define DATA_READY 0x07
#define ALTIMETER_MODE 0xB9
#define BAROMETER_MODE 0x39

/*
    Addresses found is from datasheet: https://cdn.sparkfun.com/datasheets/Sensors/Pressure/MPL3115A2.pdf
        Addresses found in Section 7
    Largely based on code by Ryker1990: https://github.com/ControlEverythingCommunity/MPL3115A2/blob/master/Arduino/MPL3115A2.ino
*/

/*
    MPL3115A - Altimeter/Barometer:
        Public functions:
        initialize() - Sets up for wire transmissions
        update() - updates pressure, altitude, and temperature
        getPressure()
        getAltitude()
        getTemperature()
*/
namespace CanSat{
    class MPL3115A
    {
        private:
            struct barometer_data{
                byte status;
                byte altitude_msb1;
                byte altitude_msb;
                byte altitude_lsb;
                byte temperature_msb;
                byte temperature_lsb;
                byte pressure_msb1;
                byte pressure_msb;
                byte pressure_lsb;
                
            };
            barometer_data barometer_data;
            float pressure;
            float altitude;
            float temperature;
            
            void RequestAltimeterFromWire()
            {
                Wire.beginTransmission(DEVICE_ADDRESS);
                Wire.write(CONTROL_REGISTER);
                Wire.write(ALTIMETER_MODE);
                Wire.endTransmission();

                Wire.beginTransmission(DEVICE_ADDRESS);
                Wire.write(SELECT_DATA_REGISTER);
                Wire.endTransmission();

                Wire.requestFrom(DEVICE_ADDRESS, 6);
                if(Wire.available() == 6)
                {
                    barometer_data.status = Wire.read();
                    barometer_data.altitude_msb1 = Wire.read();
                    barometer_data.altitude_msb = Wire.read(); // msb = most significant byte
                    barometer_data.altitude_lsb = Wire.read(); // lsb = least significant byte
                    barometer_data.temperature_msb  = Wire.read();
                    barometer_data.temperature_lsb = Wire.read();
                }
            }

            void RequestBarometerFromWire()
            {   
                Wire.beginTransmission(DEVICE_ADDRESS);
                Wire.write(CONTROL_REGISTER);
                Wire.write(BAROMETER_MODE);
                Wire.endTransmission();

                Wire.beginTransmission(DEVICE_ADDRESS);
                Wire.write(SELECT_DATA_REGISTER);
                Wire.endTransmission();

                Wire.requestFrom(DEVICE_ADDRESS, 4);
                if (Wire.available() == 4)
                {
                    barometer_data.status = Wire.read();
                    barometer_data.pressure_msb1 = Wire.read();
                    barometer_data.pressure_msb = Wire.read();
                    barometer_data.pressure_lsb = Wire.read();
                }

            }


        public:
            MPL3115A(int sda, int scl){
                Wire.setSDA(sda);
                Wire.setSCL(scl);
            } 

            void Initialize()
            {
                Wire.begin();

                Wire.beginTransmission(DEVICE_ADDRESS);
                Wire.write(CONTROL_REGISTER);
                Wire.write(ALTIMETER_MODE);
                Wire.endTransmission();

                Wire.beginTransmission(DEVICE_ADDRESS);
                Wire.write(CONTROL_REGISTER);
                Wire.write(DATA_READY);
                Wire.endTransmission();
            }

            void Update() {
                RequestAltimeterFromWire();
                RequestBarometerFromWire();

                //converting data to 20-bits 
                int tempAlt = (((long)(barometer_data.altitude_msb1 * (long)65536) + (barometer_data.altitude_msb * 256) + (barometer_data.altitude_lsb & 0xF0)) / 16);
                altitude = tempAlt / 16.0;
                temperature = (((barometer_data.temperature_msb * 256)+(barometer_data.temperature_lsb)) / 256.0) * 1.8 + 32; //in fahrenheit
                pressure = (((long)barometer_data.pressure_msb1*(long)65536) + (barometer_data.pressure_msb*256) + (barometer_data.pressure_lsb & 0xF0)) / 64000;
            }
            
            float GetPressure()
            {
                return pressure;
            }

            float GetAltitude()
            {
                return altitude;
            }

            float GetTemperature()
            {
                return temperature;
            }
    };
}