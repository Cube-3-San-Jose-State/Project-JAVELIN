#pragma once

#include "../include/container-dto.hpp"
#include "../include/cansat-modes.hpp"

/*
    Goal: Check if sensor data has surpassed threhold, and if so move to next mode (eg: if altitude > threshold, move to next mode)
*/
namespace CanSat
{
    class ModeSelect
    {
    public:
        static Container_Data SelectMode(Container_Data container_data)
        {
            switch (container_data.flight_mode)
            {
            case 'U':
                return CanSatModes::PreFlight(container_data);
                break;
            case 'L':
                return CanSatModes::Launched(container_data);
                break;
            case 'D':
                return CanSatModes::CanSatDeployed(container_data);
                break;
            case 'S':
                return CanSatModes::ParachuteDeploy(container_data);
                break;
            case 'G':
                return CanSatModes::Land(container_data);
                break;
            default:
                return container_data;
                
                break;
            }
        }

    private:
    };
}