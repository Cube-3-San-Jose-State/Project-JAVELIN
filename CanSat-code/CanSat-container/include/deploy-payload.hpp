#pragma once
#include <string>
#include <Servo.h>
#include "../include/container-dto.hpp"

namespace CanSat
{

    class PayloadDeployment
    {

    public:
        PayloadDeployment(int servo_pin, float desired_altitude)
        {
            servo_pin_ = servo_pin;
            desired_altitude_ = desired_altitude;
            servo_.attach(servo_pin_);
        }

        Container_Data Deploy(Container_Data data, int servo_angle = 180)
        {
            if (data.barometer_data.altitude <= desired_altitude_ && data.payload_deployed != 1 && data.flight_mode == 'F')
            {
                servo_.write(servo_angle);
                data.payload_deployed = 1;
            }
            else if (data.payload_deployed == 1)
            {
                printf("Payload already deployed. Altitude: %f, Deploy payload: %d, Flight mode: %c", data.barometer_data.altitude, data.payload_deployed, data.flight_mode);
            }
            else
            {
                printf("Not deploying payload yet. Altitude: %f, Deploy payload: %d, Flight mode: %c", data.barometer_data.altitude, data.payload_deployed, data.flight_mode);
            }

            return data;
        }

    private:
        Servo servo_;
        int servo_pin_;
        float desired_altitude_;
        int deployed_time_;
    };

}