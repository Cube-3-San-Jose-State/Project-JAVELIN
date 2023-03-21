#include <Wire.h>
#include <math.h>

#define MAG_ADDRESS 0x1E
#define MAG_CFG_REG_A 0x60
#define MAG_CFG_REG_C 0x62
#define MAG_OUTX_L_REG 0x68

namespace CanSat
{
    class LSM303AGR
    {
    private:
        struct MagneticField
        {
            float x;
            float y;
            float z;
        };

    public:
        MagneticField magField;

        LSM303AGR(int sda, int scl) {
            Wire2.setSDA(sda);
            Wire2.setSCL(scl);
        }

        void Initialize()
        {
            Wire2.begin();

            Wire2.beginTransmission(MAG_ADDRESS);
            Wire2.write(MAG_CFG_REG_A);
            Wire2.write(0x00); // Set magnetometer data rate to 10 Hz
            Wire2.endTransmission();

            Wire2.beginTransmission(MAG_ADDRESS);
            Wire2.write(MAG_CFG_REG_C);
            Wire2.write(0x00); // Set magnetometer to continuous conversion mode
            Wire2.endTransmission();
        }

        void Update()
        {
            Wire2.beginTransmission(MAG_ADDRESS);
            Wire2.write(MAG_OUTX_L_REG | 0x80); // Set MSB to 1 for auto-increment
            Wire2.endTransmission();
            Wire2.requestFrom(MAG_ADDRESS, 6);

            int16_t x = (Wire2.read() | (Wire2.read() << 8));
            int16_t y = (Wire2.read() | (Wire2.read() << 8));
            int16_t z = (Wire2.read() | (Wire2.read() << 8));

            magField.x = (float)x * (49.0 / 32768.0);
            magField.y = (float)y * (49.0 / 32768.0);
            magField.z = (float)z * (49.0 / 32768.0);
        }

        MagneticField GetMagneticField()
        {
            return magField;
        }

        float GetHeading()
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