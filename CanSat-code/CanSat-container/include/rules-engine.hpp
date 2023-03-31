#pragma once

#include "../include/container-dto.hpp"
#include "../lib/XBEE.hpp"

/*
    Goal: Validate system process ie data integrity and mode transitions

    Validates sensor data.
    Checks to see if sensor data makes sense. (eg: recalibrate altitude sensor if values are jumping, or if GPA is jumping around)
    Also ensures all steps go in correct order (eg: parachute doesn't deploy too soon)
    Returns a boolean which confirms expected data.
    
    Maybe make a function that uses error codes and then passes that to the radio
*/

namespace CanSat
{

    class RulesEngine
    {
        private:
            int expected_heartbeat_ = 0;
            char modes[5] = {'U', 'L', 'D', 'S', 'G'};
            int current_mode = 0;
        public:
            RulesEngine()
            {
            }

            bool ValidateHeartbeatCount(Container_Data container_data)
            {

                if (container_data.heartbeat_count != expected_heartbeat_)
                {
                    container_data.heartbeat_count = 0;
                    expected_heartbeat_ = 0;
                    return false;
                }
                else
                {
                    expected_heartbeat_++;
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
                if (container_data.barometer_data.relativeAltitude > -1 && container_data.barometer_data.relativeAltitude < 9000000 && container_data.barometer_data.temperature > -1 && container_data.barometer_data.temperature < 900)
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
                    return container_data;
                }
            }
    };
}