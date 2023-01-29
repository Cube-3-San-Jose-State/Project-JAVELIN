#pragma once
#include <queue>
#include "../include/container-dto.hpp"
#include "../lib/ParachuteServo.hpp"
#include "../lib/PayloadServo.hpp"

namespace CanSat
{
    class CanSatModes
    {
    private:
        static int parachuteThresholdMetCounter;
    public:
        static Container_Data PreFlight(Container_Data container_data) // flight mode 'U'
        {
            return container_data;
        }

        static Container_Data Launched(Container_Data container_data) // flight mode 'L'
        {
            return container_data;
        }

        static Container_Data CanSatDeployed(Container_Data container_data) // flight mode 'D'
        {            
            return container_data;
        }

        static Container_Data ParachuteDeploy(Container_Data container_data) // flight mode 'S'
        {
            return container_data;
        }

        static Container_Data PayloadDeploy(Container_Data container_data) // flight mode 'P'
        {
            return container_data;
        }

        static Container_Data PayloadLanding(Container_Data container_data) // flight mode 'A'
        {
            return container_data;
        }

        static Container_Data Land(Container_Data container_data) // flight mode 'G'
        {
            return container_data;
        }
    };

}