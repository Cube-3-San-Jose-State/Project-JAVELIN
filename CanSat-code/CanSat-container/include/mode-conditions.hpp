#include "../include/container-dto.hpp"
#define PARACHUTE_DEPLOY_ALTITUDE 400
#define PARACHUTE_DEPLOY_SAMPLE_COUNT 7
#define CONTAINER_SAMPLE_ALTITUDE 10

#define LAUNCHED_ACCEL_THRESHOLD 10
#define LAUNCHED_ALTITUDE_THRESHOLD 5

#define DEPLOYED_ALTITUDE_SAMPLE_COUNT 5
#define ACCEL_CONVERSION 9.80665 / 16384

/*
    Goal: Check threshold and switch mode 
    Update each sensor required
*/

namespace CanSat
{
    class ModeConditions
    {
    private:
        int altitudeCounter = 0;
    public:
        Container_Data PreFlight(Container_Data container_data) // flight mode 'U'
        {
            int accelY = container_data.imu_data.acceleration_y * ACCEL_CONVERSION;

            if (accelY > LAUNCHED_ACCEL_THRESHOLD && container_data.barometer_data.altitude > LAUNCHED_ALTITUDE_THRESHOLD){
                container_data.flight_mode = 'L';
            }
            return container_data;
        }

        Container_Data Launched(Container_Data container_data) // flight mode 'L'
        {
            int maxAlt = container_data.barometer_data.altitude;
            if (container_data.barometer_data.altitude < maxAlt){
                altitudeCounter ++;
            }
            else {
                altitudeCounter = 0;
            }

            if (altitudeCounter > DEPLOYED_ALTITUDE_SAMPLE_COUNT){
                container_data.flight_mode = 'D';
            }
            return container_data;
        }

        Container_Data Deployed(Container_Data container_data) // flight mode 'D'
        {

            return container_data;
        }

        Container_Data ParachuteDeploy(Container_Data container_data) // flight mode 'S'
        {

            return container_data;
        }

        Container_Data Land(Container_Data container_data) // flight mode 'G'
        {
            return container_data;
        }
    };

}