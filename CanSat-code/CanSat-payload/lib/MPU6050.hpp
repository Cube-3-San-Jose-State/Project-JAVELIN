#pragma once
#include <Wire.h>

namespace drivware
{

    class MPU6050
    {
    private:
        double temperature;
        const int device_address = 0x68;

        struct accel
        {
            double x;
            double y;
            double z;
        };
        struct gyro
        {
            double x;
            double y;
            double z;
        };

    public:
        accel accelerometer;
        gyro gyroscope;

        void Initialize()
        {
            Wire.begin();
            Wire.beginTransmission(device_address);
            Wire.write(0x6B);
            Wire.write(0);
            Wire.endTransmission(true);
        }

        void ReadMPU()
        {
            Wire.beginTransmission(device_address);
            Wire.write(0x3B);
            Wire.endTransmission(false);
            Wire.requestFrom(device_address, 14, true);

            accelerometer.x = Wire.read() << 8 | Wire.read();
            accelerometer.y = Wire.read() << 8 | Wire.read();
            accelerometer.z = Wire.read() << 8 | Wire.read();

            temperature = Wire.read() << 8 | Wire.read();

            gyroscope.x = Wire.read() << 8 | Wire.read();
            gyroscope.y = Wire.read() << 8 | Wire.read();
            gyroscope.z = Wire.read() << 8 | Wire.read();
        }

        accel GetAccel()
        {
            return accelerometer;
        }

        gyro GetGyro()
        {
            return gyroscope;
        }

        double GetTemp()
        {
            return temperature;
        }
    };
}