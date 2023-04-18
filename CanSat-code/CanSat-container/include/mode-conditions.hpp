#include "../include/container-dto.hpp"
#include "../lib/ParachuteServo.hpp"

#define SAMPLE_COUNT 3
#define LAND_SAMPLE_COUNT 5
#define ACCEL_CONVERSION 9.80665 / 16384

#define LAUNCHED_ALTITUDE_INCREASE_THRESHOLD 1

#define DESCENT_ERROR_MARGIN .5

#define PARACHUTE_ALTITUDE_THRESHOLD 5

#define PARACHUTE_DEPLOY_ALTITUDE 400
#define PARACHUTE_DEPLOY_SAMPLE_COUNT 7
#define CONTAINER_SAMPLE_ALTITUDE 10
#define CONTAINER_SAMPLE_ACCEL 10
#define ALTITUDE_SAMPLE_COUNT 5

/*
    Goal: Check threshold and switch mode
    Update each sensor required
*/

namespace CanSat {
    class ModeConditions {
    private:
        float initialAltitude = 0;
        bool initialAltitudeSet = false;
        bool gpsSet = true;

        float maxAlt = 0;
        int launchedCounter = 0;

        float previousAltitude;
        int descentCounter = 0;

        int deployedCounter = 0;
        int parachuteThresholdMetCounter;
        int stationaryCounter;



    public:
        Container_Data PreFlight(Container_Data container_data) // flight mode 'U'
        {  
            //Average starting altitude. Collects 20 samples then once done collecting, sets it
            if (container_data.heartbeat_count > 10 && container_data.heartbeat_count < 40) {
                initialAltitude += container_data.barometer_data.relativeAltitude;
            }

            if (initialAltitudeSet == false && container_data.heartbeat_count > 40) {
                initialAltitudeSet = true;
                initialAltitude /= 30;
                previousAltitude = initialAltitude; // for use during launched mode
            }

            // if (gpsSet == false && container_data.gps_data.latitude != 0 && container_data.gps_data.longitude != 0) {
            //     gpsSet = true;
            // }

            if (initialAltitudeSet == true && gpsSet == true) {
                container_data.flight_mode = 'A';
            }

            return container_data;
        }

        Container_Data Armed(Container_Data container_data) //flight mode 'A'
        {
            // if (initialAltitude + container_data.barometer_data.relativeAltitude > (initialAltitude + LAUNCHED_ALTITUDE_INCREASE_THRESHOLD)) {
            //     launchedCounter++;
            // } else {
            //     launchedCounter = 0;
            // }

            // if (launchedCounter >= SAMPLE_COUNT) {
            //     container_data.flight_mode = 'L';
            // }

            if (initialAltitude + container_data.barometer_data.relativeAltitude > 3 + initialAltitude) {
                launchedCounter ++;
            }
            else {
                launchedCounter = 0;
            }

            if (launchedCounter > SAMPLE_COUNT) {
                container_data.flight_mode = 'L';
            }

            return container_data;
        }

        Container_Data Launched(Container_Data container_data) // flight mode 'L'
        {
            // if (altitude is still climbing): setmaxalt
            if (initialAltitude + container_data.barometer_data.relativeAltitude > initialAltitude + maxAlt) {
                maxAlt = container_data.barometer_data.relativeAltitude;
                descentCounter = 0;
            } 
            // else: altitude is less than max - 1m
            // else if (initialAltitude + containher_data.barometer_data.relativeAltitude < maxAlt - DESCENT_ERROR_MARGIN) { 
            //     descentCounter++;
            // }

            else if (initialAltitude + container_data.barometer_data.relativeAltitude < maxAlt - DESCENT_ERROR_MARGIN) { 
                descentCounter++;
            }

            if (descentCounter >= SAMPLE_COUNT) {
                container_data.flight_mode = 'D';
            }
            return container_data;
        }

        Container_Data Deployed(Container_Data container_data) // flight mode 'D'
        {
            // if (container_data.barometer_data.relativeAltitude < PARACHUTE_DEPLOY_ALTITUDE) {
            //     parachuteThresholdMetCounter++;
            // }
            // else {
            //     parachuteThresholdMetCounter = 0;
            // }

            // if (parachuteThresholdMetCounter > SAMPLE_COUNT) {
            //     container_data.flight_mode = 'S';
            // }

            if (initialAltitude + container_data.barometer_data.relativeAltitude < 3 + initialAltitude) {
                container_data.flight_mode = 'S';
            }

            
        return container_data;
    }

    Container_Data ParachuteDeploy(Container_Data container_data) // flight mode 'S'
    {

        // if (container_data.barometer_data.relativeAltitude <= container_data.barometer_data.relativeAltitude + 2 || container_data.barometer_data.relativeAltitude >= container_data.barometer_data.relativeAltitude - 2) {
        //     stationaryCounter++;
        // }

        if (container_data.barometer_data.relativeAltitude <= container_data.barometer_data.relativeAltitude + 1 && container_data.barometer_data.relativeAltitude >= container_data.barometer_data.relativeAltitude - 1) {
            stationaryCounter++;
        }
        else {
            stationaryCounter = 0;
        }

        if (stationaryCounter > LAND_SAMPLE_COUNT) {
            container_data.flight_mode = 'G';
        }

        // if (container_data.barometer_data.relativeAltitude < 2 && container_data.barometer_data.relativeAltitude > 0 ) {
        //     stationaryCounter++;
        // }
        // else {
        //     stationaryCounter = 0;
        // }

        // if (stationaryCounter > SAMPLE_COUNT) {
        //     container_data.flight_mode = 'G';
        // }

      //  if (container_data.barometer_data.relativeAltitude < 2) {
       //     container_data.flight_mode = 'G';
       // }
        return container_data;
    }

    Container_Data Land(Container_Data container_data) // flight mode 'G'
    {
        return container_data;
    }
};
}