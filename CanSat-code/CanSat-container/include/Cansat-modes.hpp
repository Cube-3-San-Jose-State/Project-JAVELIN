#pragma once
#include <queue>
#include "../include/container-dto.hpp"


#define PARACHUTE_DEPLOY_ALTITUDE 400
#define PARACHUTE_DEPLOY_SAMPLE_COUNT 7
#define CONTAINER_SAMPLE_ALTITUDE 10
#define CONTAINER_SAMPLE_ACCEL 10
#define ALTITUDE_SAMPLE_COUNT 5

int accelY = 0;

namespace CanSat
{
    class CanSatModes
    {
    private:
        static int parachuteThresholdMetCounter;
        static int altitudeCounter;
        static const int ACCEL_CONVERSION = 9.80665 / 16384;
    public:
        static Container_Data PreFlight(Container_Data container_data) // flight mode 'U'
        {
            return container_data;
        }

        static Container_Data Launched(Container_Data container_data) // flight mode 'L'
        {
            // accelY = container_data.imu_data.acceleration_y * ACCEL_CONVERSION;

            // if (accelY > CONTAINER_SAMPLE_ACCEL && container_data.barometer_data.altitude > CONTAINER_SAMPLE_ALTITUDE){
            //     container_data.flight_mode = 'L';
            // }

            // int maxAlt = container_data.barometer_data.altitude;
            // if (container_data.barometer_data.altitude < maxAlt){
            //     altitudeCounter ++;
            // }
            // else {
            //     altitudeCounter = 0;
            // }


            // if (altitudeCounter > ALTITUDE_SAMPLE_COUNT){
            //     container_data.flight_mode = 'D';
            // }

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
            //payloadServo.ReleasePayload();
            container_data.flight_mode = 'A';
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