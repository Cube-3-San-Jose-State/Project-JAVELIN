#pragma once
#include <map>
#include <Wire.h>

#define DEVICE_ADDRESS 0x60
#define CONTROL_REGISTER 0x26
#define DATA_REGISTER 0x13
#define STATUS 0x00
#define PRESSURE_MSB 0x01
#define TEMPERATURE_MSB 0x04
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
            float altitude = 0.0;
            float temperature = 0.0;
            byte readByte(byte address){
                Wire.beginTransmission(DEVICE_ADDRESS);
                Wire.write(address);
                Wire.endTransmission(false);
                Wire.requestFrom(DEVICE_ADDRESS, 1);
                return Wire.read();
            }
            void writeByte(byte address, byte value){
                Wire.beginTransmission(DEVICE_ADDRESS);
                Wire.write(address);
                Wire.write(value);
                Wire.endTransmission(true);
            }

            void toggleOneShot(){ //clears settings, allowing for faster reading times
                byte currentSetting = readByte(CONTROL_REGISTER);
                currentSetting &= ~(1<<1);
                writeByte(CONTROL_REGISTER, currentSetting);

                currentSetting = readByte(CONTROL_REGISTER);
                currentSetting |= (1<<1);
                writeByte(CONTROL_REGISTER, currentSetting);
            }

        public:
            MPL3115A(int sda, int scl){
                Wire.setSDA(sda);
                Wire.setSCL(scl);
            };

            void Initialize(){
                Wire.begin();

                byte altimeterMode = readByte(CONTROL_REGISTER);
                altimeterMode |= (1<<7);
                writeByte(CONTROL_REGISTER, altimeterMode);
            };

            void Update(){
                toggleOneShot(); //toggle on
                // Update Altimeter
                Wire.beginTransmission(DEVICE_ADDRESS);
                Wire.write(PRESSURE_MSB);
                Wire.endTransmission(false);
                Wire.requestFrom(DEVICE_ADDRESS, 3);
                
                int counter = 0;
                byte altitude_msb, altitude_csb, altitude_lsb;
                altitude_msb = Wire.read();
                altitude_csb = Wire.read();
                altitude_lsb = Wire.read();
                
                toggleOneShot(); //toggle off

                float tempcsb = (altitude_lsb >> 4) / 16.0;
                altitude = (float)( (altitude_msb << 8) | altitude_csb ) + tempcsb;

                toggleOneShot(); //toggle on

                Wire.beginTransmission(DEVICE_ADDRESS);
                Wire.write(TEMPERATURE_MSB);
                Wire.endTransmission(false);
                Wire.requestFrom(DEVICE_ADDRESS, 2);

                byte temperature_msb, temperature_lsb;
                temperature_msb = Wire.read();
                temperature_lsb = Wire.read();

                float templsb = (temperature_lsb >> 4 ) / 16.0;
                temperature = (float)(temperature_msb + templsb);
            }

            float GetAltitude(){
                return altitude;
            }

            float GetTemperature(){
                return temperature;
            }
    };
}