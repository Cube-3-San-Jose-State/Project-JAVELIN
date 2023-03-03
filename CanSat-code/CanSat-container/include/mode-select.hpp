#pragma once
#include "../include/container-dto.hpp"
#include "../include/mode-conditions.hpp"

namespace CanSat
{
    class ModeSelect
    {
        private:
            ModeConditions modes;
        public:
            Container_Data SelectMode(Container_Data container_data)
            {
                switch (container_data.flight_mode)
                {
                case 'U': // Preflight
                    return modes.PreFlight(container_data);

                case 'L': // Launched
                    return modes.Launched(container_data);

                case 'D':
                    return modes.Deployed(container_data);

                case 'S':
                    return modes.ParachuteDeploy(container_data);

                case 'G':
                    return modes.Land(container_data);

                default:
                    return container_data;
                }
            }
    };
};