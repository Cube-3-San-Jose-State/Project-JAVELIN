#pragma once

#include "../include/container-dto.hpp"
#include "../lib/XBEE.hpp"

/*
    Goal: Validate system process ie data integrity and mode transitions

    Validates sensor data.
    Checks to see if sensor data makes sense. (eg: recalibrate altitude sensor if values are jumping, or if GPA is jumping around)
    Also ensures all steps go in correct order (eg: parachute doesn't deploy too soon)
*/

namespace CanSat
{

    class RulesEngine
    {
    public:
        RulesEngine(XBEE &to_groundcontrol_)
        {
            to_groundcontrol = to_groundcontrol_;
        }
        /*
            Required validate function data:
                confirm altitude
        */

        // THE RULES ENGINE INDIVIDUAL FUNCTIONS SHOULD ONLY RETURN BOOLEAN VALUES SO MAKE SURE WE ARE GETTING EXPECTED DATA

        bool ValidateHeartbeatCount(Container_Data container_data)
        {

            if (container_data.heartbeat_count != expexted_heartbeat_)
            {
                to_groundcontrol.transmitData("Heartbeat is out of sync, resetting to 0 and establishing new heartbeat count");
                container_data.heartbeat_count = 0;
                expexted_heartbeat_ = 0;
                return false;
            }
            else
            {
                expexted_heartbeat_++;
                return true;
            }
        }

        bool ValidateIMU(Container_Data container_data)
        {
            if (container_data.imu_data.acceleration_x > -1.1 && container_data.imu_data.acceleration_x < 1.1 && container_data.imu_data.acceleration_y > -1.1 && container_data.imu_data.acceleration_y < 1.1 && container_data.imu_data.acceleration_z > -1.1 && container_data.imu_data.acceleration_z < 1.1)
            {
                return true;
            }
            else
            {
                return false;
            }
        }

        bool ValidateGPS(Container_Data container_data)
        {
            if (container_data.gps_data.latitude > -1 && container_data.gps_data.longitude > -1) // example only change to correct validation
            {
                return true;
            }
            else
            {
                return false;
            }
        }

        bool ValidateAltitude(Container_Data container_data)
        {
            if (container_data.barometer_data.altitude > -1 && container_data.barometer_data.altitude < 9000000 && container_data.barometer_data.temperature > -1 && container_data.barometer_data.temperature < 900)
            {
                return true;
            }
            else
            {
                return false;
            }
        }

        // Check what next mode should be
        bool ValidateMode(Container_Data container_data)
        {
            if (container_data.flight_mode == modes[current_mode])
            {
                current_mode++;
                return true;
            }
            else
            {
                return false;
            }
        }

        Container_Data MainValidation(Container_Data container_data)
        {
            if (ValidateHeartbeatCount(container_data), ValidateMode(container_data) && ValidateAltitude(container_data) && ValidateIMU(container_data) && ValidateGPS(container_data))
            {
                return container_data;
            }
            else
            {
                to_groundcontrol.transmitData("Failed Validation!!!");
                return container_data;
            }
        }

        // Maybe make a functiuon that uses error codes and then passes that to the radio

    private:
        int expexted_heartbeat_ = 0;
        char modes[5] = {'U', 'L', 'D', 'S', 'G'};
        int current_mode = 0;
        XBEE to_groundcontrol;
    };

}