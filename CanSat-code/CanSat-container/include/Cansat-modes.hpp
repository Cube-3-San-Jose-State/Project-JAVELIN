#pragma once

#include "../include/container-dto.hpp"

namespace CanSat
{
    class CanSatModes
    {

    public:
        static Container_Data PreFlight(Container_Data container_data)
        {
            return container_data;
        }

        static Container_Data Launched(Container_Data container_data)
        {
            return container_data;
        }

        static Container_Data CanSatDeployed(Container_Data container_data)
        {
            return container_data;
        }

        static Container_Data ParachuteDeploy(Container_Data container_data)
        {
            return container_data;
        }

        static Container_Data PayloadDeploy(Container_Data container_data)
        {
            return container_data;
        }

        static Container_Data SecondaryParachute(Container_Data container_data)
        {
            return container_data;
        }

        static Container_Data PayloadLanding(Container_Data container_data)
        {
            return container_data;
        }

        static Container_Data Land(Container_Data container_data)
        {
            return container_data;
        }

    private:
    };

}