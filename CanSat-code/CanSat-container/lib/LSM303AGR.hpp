#include <Wire.h>
#include <math.h>

#define MAG_ADDRESS 0x1E
#define MAG_CFG_REG_A 0x60
#define MAG_CFG_REG_B 0x61
#define MAG_CFG_REG_C 0x62
#define MAG_OUTX_L_REG 0x68

namespace CanSat
{
    class LSM303AGR
    {
    private:
        struct magneticField
        {
            float x;
            float y;
            float z;
        };

    public:
        magneticField magField;

        LSM303AGR() {}

        void initialize()
        {
            Wire.begin();

            Wire.beginTransmission(MAG_ADDRESS);
            Wire.write(MAG_CFG_REG_A);
            Wire.write(0x03); // Set magnetometer data rate to 10 Hz
            Wire.endTransmission();

            Wire.beginTransmission(MAG_ADDRESS);
            Wire.write(MAG_CFG_REG_B);
            Wire.write(0x20); // Set magnetometer full scale to +/- 49 gauss
            Wire.endTransmission();

            Wire.beginTransmission(MAG_ADDRESS);
            Wire.write(MAG_CFG_REG_C);
            Wire.write(0x00); // Set magnetometer to continuous conversion mode
            Wire.endTransmission();
        }

        void update()
        {
            Wire.beginTransmission(MAG_ADDRESS);
            Wire.write(MAG_OUTX_L_REG | 0x80); // Set MSB to 1 for auto-increment
            Wire.endTransmission();
            Wire.requestFrom(MAG_ADDRESS, 6);

            int16_t x = (Wire.read() | (Wire.read() << 8));
            int16_t y = (Wire.read() | (Wire.read() << 8));
            int16_t z = (Wire.read() | (Wire.read() << 8));

            magField.x = (float)x * (49.0 / 32768.0);
            magField.y = (float)y * (49.0 / 32768.0);
            magField.z = (float)z * (49.0 / 32768.0);
        }

        magneticField getMagneticField()
        {
            return magField;
        }

        float getHeading()
        {
            float heading = atan2(magField.y, magField.x) * 180.0 / M_PI;
            if (heading < 0)
            {
                heading += 360;
            }
            return heading;
        }
    };
}
