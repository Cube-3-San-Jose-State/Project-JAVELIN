#include <Wire.h>
#include <map>

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
class MPL3115A
{
    private:
        std::map<string, int> data;
        float pressure;
        float altitude;
        float temperature;
        
        void requestAltimeterFromWire()
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
                data["status"] = Wire.read();
                data["altitude_msb1"] = Wire.read();
                data["altitude_msb"] = Wire.read(); // msb = most significant byte
                data["altitude_lsb"] = Wire.read(); // lsb = least significant byte
                data["temperature_msb"] = Wire.read();
                data["temperature_lsb"] = Wire.read();
            }
        }

        void requestBarometerFromWire()
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
                data["status"] = Wire.read();
                data["pressure_msb1"] = Wire.read();
                data["pressure_msb"] = Wire.read();
                data["pressure_lsb"] = Wire.read();
            }

        }


    public:
        void initialize()
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

        void update() {
            requestAltimeterFromWire();
            requestBarometerFromWire();

            //converting data to 20-bits
            altitude  = (((long)(data["altitude_msb1"] * (long)65536) + (data["altitude_msb"]*256) + (data["altitude_lsb"] & 0xF0))/256.0);
            temperature = (((data["temperature_msb"] * 256)+(data["temperature_lsb"])) / 256.0) * 1.8 + 32; //in fahrenheit
            pressure = (((long)data["pressure_msb1"]*(long)65536) + (data["pressure_msb"]*256) + (data["pressure_lsb"] & 0xF0)) / 64000;
        }
        
        float getPressure()
        {
            return pressure;
        }

        float getAltitude()
        {
            return altitude;
        }

        float getTemperature()
        {
            return temperature;
        }
};