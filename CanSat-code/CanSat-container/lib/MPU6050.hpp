#include <Wire.h>
#include <map>
#define MPU_ADDRESS 0x68
#define PWR_MGMT_1 0x6B
#define ACCEL_XOUT_H 0x3B
/*
    Code largely based on Arduino user JohnChi's code.
    References: https://invensense.tdk.com/wp-content/uploads/2015/02/MPU-6000-Datasheet1.pdf
    MPU6050 - Accelerometer:
        Constructor: MPL3115A accelerometer(int device_address);
        Public functions:
        void Initialize() - Sets up for wire transmissions
        void Update() - updates data so that get functions return updated values
        accel GetAccelerometer() returns accel map { {'x': int16_t}, {'y': int16_t}, {'z': int_16t} }
        gyro GetGyroscope() returns gyro map { {'x': int16_t}, {'y': int16_t}, {'z': int_16t} }
*/

/**
 * @brief Code for IMU, gathering accelerometer and gyroscope data.
 * 
 * Constructor: MPL3115A accelerometer(int device_address);
 * Public functions:
 * void Initialize() - Sets up for wire transmissions
 * void Update() - updates data so that get functions return updated values
 * accel GetAccelerometer() returns accel map { {'x': int16_t}, {'y': int16_t}, {'z': int_16t} }
 * gyro GetGyroscope() returns gyro map { {'x': int16_t}, {'y': int16_t}, {'z': int_16t} }
 * 
 * Reference/Credit: https://invensense.tdk.com/wp-content/uploads/2015/02/MPU-6000-Datasheet1.pdf, JohnChi on github.
 * 
 */
namespace CanSat{
    class MPU6050
    {
        private:
            struct accelerometer 
            {
                int16_t x;
                int16_t y;
                int16_t z;
            };
            struct gyroscope 
            {
                int16_t x;
                int16_t y;
                int16_t z;
            };
            struct attitude
            {
                float roll;
                float pitch;
            };

            int16_t temp_holder;
            
        public:
            accelerometer accel;
            gyroscope gyro;
            attitude atti;

            MPU6050(int sda, int scl)
            {
                Wire.setSCL(scl);
                Wire.setSDA(sda);
            }

            void Initialize()
            {
                Wire.begin();
                Wire.beginTransmission(MPU_ADDRESS);
                Wire.write(PWR_MGMT_1); // PWR_MGMT_1 register
                Wire.write(0x00); // set to zero (wakes up the MPU-6050)
                Wire.endTransmission(true);
            }

            void Update()
            {
                Wire.beginTransmission(MPU_ADDRESS);
                Wire.write(ACCEL_XOUT_H); // starting with register 0x3B (ACCEL_XOUT_H)
                Wire.endTransmission(false);
                Wire.requestFrom(MPU_ADDRESS, 14, true); // request a total of 14 registers
                accel.x = Wire.read() << 8 | Wire.read(); // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
                accel.y = Wire.read() << 8 | Wire.read(); // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
                accel.z = Wire.read() << 8 | Wire.read(); // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
                temp_holder = Wire.read() << 8 | Wire.read(); // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
                gyro.x = Wire.read() << 8 | Wire.read(); // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
                gyro.y = Wire.read() << 8 | Wire.read(); // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
                gyro.z = Wire.read() << 8 | Wire.read(); // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
            }

            accelerometer GetAccelerometer()
            {
                return accel;
            }

            gyroscope  GetGyroscope()
            {
                return gyro;
            }

            attitude GetAttitude(){
                atti.roll = atan2(accel.y, accel.z);
                atti.pitch = atan2(-accel.x, sqrt(accel.y*accel.y + accel.z*accel.z));
                return atti;
            }
    };
}