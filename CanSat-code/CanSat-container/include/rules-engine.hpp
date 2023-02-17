#pragma once

#include "../include/container-dto.hpp"

namespace CanSat
{

    class RulesEngine
    {
    public:
        Container_Data Validate(Container_Data container_data)
        {
            if (container_data.barometer_data.altitude < 0 && container_data.flight_mode == 'U') //Check the satellites status, and modify the sensor values if needed
            {
                container_data.barometer_data.altitude = 0;
            }

            heartbeatCounter++;
            if (heartbeatCounter != container_data.heartbeat_count){
                printf("Heartbeat is out of sync, resetting to 0 and establishing new heartbeat count");
                container_data.heartbeat_count = 0;
                heartbeatCounter = 0;
            }

            if (container_data.flight_mode == 'D' && container_data.barometer_data.altitude <= parachute_deployment_altitude) {
                container_data.flight_mode = 'S'; //If we the cansat is deployed already and has reached the parachute deployment altitude, switch to parachute deployment mode
            }


            return container_data;
        }

    private:
        int heartbeatCounter = 0;
        int parachute_deployment_altitude = 400;
    };

}