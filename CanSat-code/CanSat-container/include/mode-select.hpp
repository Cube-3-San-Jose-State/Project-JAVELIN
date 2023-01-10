#pragma once

#include "../include/container-dto.hpp"
#include "../include/cansat-modes.hpp"

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
            case 'P':
                return CanSatModes::PayloadDeploy(container_data);
                break;
            case 'F':
                return CanSatModes::SecondaryParachute(container_data);
                break;
            case 'A':
                return CanSatModes::PayloadLanding(container_data);
                break;
            case 'G':
                return CanSatModes::Land(container_data);
                break;
            default:
                return Container_Data{};
                break;
            }
        }

    private:
    };
}