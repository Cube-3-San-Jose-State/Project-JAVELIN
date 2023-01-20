#include <Wire.h>

#define MPL3115A2_ADDRESS 0x60

// Define MPL3115A2 registers
enum mpl3115a_regs
{
    STATUS = 0x00,
    OUT_P_MSB = 0x01,
    OUT_P_CSB = 0x02,
    OUT_P_LSB = 0x03,
    OUT_T_MSB = 0x04,
    OUT_T_LSB = 0x05,
    DR_STATUS = 0x06,
    OUT_P_DELTA_MSB = 0x07,
    OUT_P_DELTA_CSB = 0x08,
    OUT_P_DELTA_LSB = 0x09,
    OUT_T_DELTA_MSB = 0x0A,
    OUT_T_DELTA_LSB = 0x0B,
    WHO_AM_I = 0x0C,
    F_STATUS = 0x0D,
    F_DATA = 0x0E,
    F_SETUP = 0x0F,
    TIME_DLY = 0x10,
    SYSMOD = 0x11,
    INT_SOURCE = 0x12,
    PT_DATA_CFG = 0x13,
    BAR_IN_MSB = 0x14,
    BAR_IN_LSB = 0x15,
    P_TGT_MSB = 0x16,
    P_TGT_LSB = 0x17,
    T_TGT = 0x18,
    P_WND_MSB = 0x19,
    P_WND_LSB = 0x1A,
    T_WND = 0x1B,
    P_MIN_MSB = 0x1C,
    P_MIN_CSB = 0x1D,
    P_MIN_LSB = 0x1E,
    T_MIN_MSB = 0x1F,
    T_MIN_LSB = 0x20,
    P_MAX_MSB = 0x21,
    P_MAX_CSB = 0x22,
    P_MAX_LSB = 0x23,
    T_MAX_MSB = 0x24,
    T_MAX_LSB = 0x25,
    CTRL_REG1 = 0x26,
    CTRL_REG2 = 0x27,
    CTRL_REG3 = 0x28,
    CTRL_REG4 = 0x29,
    CTRL_REG5 = 0x2A,
    OFF_P = 0x2B,
    OFF_T = 0x2C,
    OFF_H = 0x2D
};

namespace CanSat
{
    class MPL3115A
    {

    private:
        struct BarometerData
        {
            float pressure;
            float temperature;
            float altitude;
        };

        TwoWire *_i2cPort;                       // The generic connection to user's chosen I2C hardware
        uint8_t _I2Caddress = MPL3115A2_ADDRESS; // Default 7-bit unshifted address of the MPL3115A2
        // Clears then sets the OST bit which causes the sensor to immediately take another reading
        // Needed to sample faster than 1Hz
        void toggleOneShot(void)
        {
            byte tempSetting = IIC_Read(CTRL_REG1); // Read current settings
            tempSetting &= ~(1 << 1);               // Clear OST bit
            IIC_Write(CTRL_REG1, tempSetting);

            tempSetting = IIC_Read(CTRL_REG1); // Read current settings to be safe
            tempSetting |= (1 << 1);           // Set OST bit
            IIC_Write(CTRL_REG1, tempSetting);
        }

        // These are the two I2C functions in this sketch.
        byte IIC_Read(byte regAddr)
        {
            // This function reads one byte over IIC
            _i2cPort->beginTransmission(_I2Caddress);
            _i2cPort->write(regAddr);              // Address of CTRL_REG1
            _i2cPort->endTransmission(false);      // Send data to I2C dev with option for a repeated start. THIS IS NECESSARY and not supported before Arduino V1.0.1!
            _i2cPort->requestFrom(_I2Caddress, 1); // Request the data...
            return _i2cPort->read();
        }

        void IIC_Write(byte regAddr, byte value)
        {
            // This function writes one byto over IIC
            _i2cPort->beginTransmission(_I2Caddress);
            _i2cPort->write(regAddr);
            _i2cPort->write(value);
            _i2cPort->endTransmission(true);
        }

    public:
        BarometerData barometer_data;
        void Initialize(TwoWire &wirePort = Wire, uint8_t deviceAddress = MPL3115A2_ADDRESS)
        {
            _i2cPort = &wirePort;
            _I2Caddress = deviceAddress;
            setModeAltimeter(); // Measure altitude above sea level in meters

            setOversampleRate(7); // Set Oversample to the recommended 128
            enableEventFlags();   // Enable all three pressure and temp event flags
        }

        void Update()
        {
            barometer_data.pressure = readPressure();
            barometer_data.temperature = readTemp();
            barometer_data.altitude = readAltitudeFt();
        }

        BarometerData GetData()
        {
            return barometer_data;
        }

        // Returns the number of meters above sea level
        // Returns -1 if no new data is available
        float readAltitude()
        {
            toggleOneShot(); // Toggle the OST bit causing the sensor to immediately take another reading

            // Wait for PDR bit, indicates we have new pressure data
            int counter = 0;
            while ((IIC_Read(STATUS) & (1 << 1)) == 0)
            {
                if (++counter > 600)
                    return (-999); // Error out after max of 512ms for a read
                delay(1);
            }

            // Read pressure registers
            _i2cPort->beginTransmission(_I2Caddress);
            _i2cPort->write(OUT_P_MSB);       // Address of data to get
            _i2cPort->endTransmission(false); // Send data to I2C dev with option for a repeated start. THIS IS NECESSARY and not supported before Arduino V1.0.1!
            if (_i2cPort->requestFrom(_I2Caddress, 3) != 3)
            { // Request three bytes
                return -999;
            }

            byte msb, csb, lsb;
            msb = _i2cPort->read();
            csb = _i2cPort->read();
            lsb = _i2cPort->read();

            // The least significant bytes l_altitude and l_temp are 4-bit,
            // fractional values, so you must cast the calulation in (float),
            // shift the value over 4 spots to the right and divide by 16 (since
            // there are 16 values in 4-bits).
            float tempcsb = (lsb >> 4) / 16.0;

            float altitude = (float)((msb << 8) | csb) + tempcsb;

            return (altitude);
        }

        // Returns the number of feet above sea level
        float readAltitudeFt()
        {
            return (readAltitude() * 3.28084);
        }

        // Reads the current pressure in Pa
        // Unit must be set in barometric pressure mode
        // Returns -1 if no new data is available
        float readPressure()
        {
            // Check PDR bit, if it's not set then toggle OST
            if (IIC_Read(STATUS) & (1 << 2) == 0)
                toggleOneShot(); // Toggle the OST bit causing the sensor to immediately take another reading

            // Wait for PDR bit, indicates we have new pressure data
            int counter = 0;
            while (IIC_Read(STATUS) & (1 << 2) == 0)
            {
                if (++counter > 600)
                    return (-999); // Error out after max of 512ms for a read
                delay(1);
            }

            // Read pressure registers
            _i2cPort->beginTransmission(_I2Caddress);
            _i2cPort->write(OUT_P_MSB);       // Address of data to get
            _i2cPort->endTransmission(false); // Send data to I2C dev with option for a repeated start. THIS IS NECESSARY and not supported before Arduino V1.0.1!
            if (_i2cPort->requestFrom(_I2Caddress, 3) != 3)
            { // Request three bytes
                return -999;
            }

            byte msb, csb, lsb;
            msb = _i2cPort->read();
            csb = _i2cPort->read();
            lsb = _i2cPort->read();

            toggleOneShot(); // Toggle the OST bit causing the sensor to immediately take another reading

            // Pressure comes back as a left shifted 20 bit number
            long pressure_whole = (long)msb << 16 | (long)csb << 8 | (long)lsb;
            pressure_whole >>= 6; // Pressure is an 18 bit number with 2 bits of decimal. Get rid of decimal portion.

            lsb &= 0x3F;                               // B00110000; //Bits 5/4 represent the fractional component
            lsb >>= 4;                                 // Get it right aligned
            float pressure_decimal = (float)lsb / 4.0; // Turn it into fraction

            float pressure = (float)pressure_whole + pressure_decimal;

            return (pressure);
        }

        float readTemp()
        {
            if (IIC_Read(STATUS) & (1 << 1) == 0)
                toggleOneShot(); // Toggle the OST bit causing the sensor to immediately take another reading

            // Wait for TDR bit, indicates we have new temp data
            int counter = 0;
            while ((IIC_Read(STATUS) & (1 << 1)) == 0)
            {
                if (++counter > 600)
                    return (-999); // Error out after max of 512ms for a read
                delay(1);
            }

            // Read temperature registers
            _i2cPort->beginTransmission(_I2Caddress);
            _i2cPort->write(OUT_T_MSB);       // Address of data to get
            _i2cPort->endTransmission(false); // Send data to I2C dev with option for a repeated start. THIS IS NECESSARY and not supported before Arduino V1.0.1!
            if (_i2cPort->requestFrom(_I2Caddress, 2) != 2)
            { // Request two bytes
                return -999;
            }

            byte msb, lsb;
            msb = _i2cPort->read();
            lsb = _i2cPort->read();

            toggleOneShot(); // Toggle the OST bit causing the sensor to immediately take another reading

            // Negative temperature fix by D.D.G.
            uint16_t foo = 0;
            bool negSign = false;

            // Check for 2s compliment
            if (msb > 0x7F)
            {
                foo = ~((msb << 8) + lsb) + 1; // 2’s complement
                msb = foo >> 8;
                lsb = foo & 0x00F0;
                negSign = true;
            }

            // The least significant bytes l_altitude and l_temp are 4-bit,
            // fractional values, so you must cast the calulation in (float),
            // shift the value over 4 spots to the right and divide by 16 (since
            // there are 16 values in 4-bits).
            float templsb = (lsb >> 4) / 16.0; // temp, fraction of a degree

            float temperature = (float)(msb + templsb);

            if (negSign)
                temperature = 0 - temperature;

            return (temperature);
        }

        // Give me temperature in fahrenheit!
        float readTempF()
        {
            return ((readTemp() * 9.0) / 5.0 + 32.0); // Convert celsius to fahrenheit
        }

        // Sets the mode to Barometer
        // CTRL_REG1, ALT bit
        void setModeBarometer()
        {
            byte tempSetting = IIC_Read(CTRL_REG1); // Read current settings
            tempSetting &= ~(1 << 7);               // Clear ALT bit
            IIC_Write(CTRL_REG1, tempSetting);
        }

        // Sets the mode to Altimeter
        // CTRL_REG1, ALT bit
        void setModeAltimeter()
        {
            byte tempSetting = IIC_Read(CTRL_REG1); // Read current settings
            tempSetting |= (1 << 7);                // Set ALT bit
            IIC_Write(CTRL_REG1, tempSetting);
        }

        // Puts the sensor in standby mode
        // This is needed so that we can modify the major control registers
        void setModeStandby()
        {
            byte tempSetting = IIC_Read(CTRL_REG1); // Read current settings
            tempSetting &= ~(1 << 0);               // Clear SBYB bit for Standby mode
            IIC_Write(CTRL_REG1, tempSetting);
        }

        // Puts the sensor in active mode
        // This is needed so that we can modify the major control registers
        void setModeActive()
        {
            byte tempSetting = IIC_Read(CTRL_REG1); // Read current settings
            tempSetting |= (1 << 0);                // Set SBYB bit for Active mode
            IIC_Write(CTRL_REG1, tempSetting);
        }

        // Call with a rate from 0 to 7. See page 33 for table of ratios.
        // Sets the over sample rate. Datasheet calls for 128 but you can set it
        // from 1 to 128 samples. The higher the oversample rate the greater
        // the time between data samples.
        void setOversampleRate(byte sampleRate)
        {
            if (sampleRate > 7)
                sampleRate = 7; // OS cannot be larger than 0b.0111
            sampleRate <<= 3;   // Align it for the CTRL_REG1 register

            byte tempSetting = IIC_Read(CTRL_REG1); // Read current settings
            tempSetting &= 0xc7;                    // B11000111; //Clear out old OS bits
            tempSetting |= sampleRate;              // Mask in new OS bits
            IIC_Write(CTRL_REG1, tempSetting);
        }

        // Enables the pressure and temp measurement event flags so that we can
        // test against them. This is recommended in datasheet during setup.
        void enableEventFlags()
        {
            IIC_Write(PT_DATA_CFG, 0x07); // Enable all three pressure and temp event flags
        }
    };
}