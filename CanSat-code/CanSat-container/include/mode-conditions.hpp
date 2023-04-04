#include "../include/container-dto.hpp"
#include "../lib/ParachuteServo.hpp"

#define SAMPLE_COUNT 5
#define ACCEL_CONVERSION 9.80665 / 16384

#define LAUNCHED_ACCEL_THRESHOLD 2
#define LAUNCHED_ALTITUDE_INCREASE_THRESHOLD 2

#define PARACHUTE_ALTITUDE_THRESHOLD 6



/*
    Goal: Check threshold and switch mode 
    Update each sensor required
*/

namespace CanSat
{
    class ModeConditions
    {
    private:
        float initialAltitude = 0;
        bool initialAltitudeSet = false;
        bool gpsSet = true;

        int launchedCounter = 0;

        float previousAltitude;
        int descentCounter = 0;

        int deployedCounter  = 0;
        int parachuteThresholdMetCounter;
        int stationaryCounter;

        

    public:
        Container_Data PreFlight(Container_Data container_data) // flight mode 'U'
        {
            //Average starting altitude. Collects 100 samples then once done collecting, sets it
            if (container_data.heartbeat_count > 25 && container_data.heartbeat_count < 125) {
                initialAltitude += container_data.barometer_data.relativeAltitude;
            }

            if (initialAltitudeSet == false && container_data.heartbeat_count > 125) {
                initialAltitudeSet = true;
                initialAltitude /= 100;
                previousAltitude = initialAltitude; // for use during launched mode
            }

            // if (gpsSet == false && container_data.gps_data.latitude != 0 && container_data.gps_data.longitude != 0) {
            //     gpsSet = true;
            // }

            if (initialAltitudeSet == true && gpsSet == true){
                container_data.flight_mode = 'R';
            }

            return container_data;
        }

        Container_Data ReadyToLaunch(Container_Data container_data) //flight mode 'R'
        {
            int accelY = container_data.imu_data.acceleration_y * ACCEL_CONVERSION;
            // if (accel is > 10m/s^2 AND altitude has raised LAUNCHED_ALTITUDE_INCREASE_THRESHOLD) steak is incremented. At 5, switch to launched
            if (accelY > LAUNCHED_ACCEL_THRESHOLD && container_data.barometer_data.relativeAltitude > (initialAltitude+LAUNCHED_ALTITUDE_INCREASE_THRESHOLD)){
                launchedCounter++;
            }

            if (launchedCounter >= SAMPLE_COUNT) {
                container_data.flight_mode = 'L';
            }
        }

        Container_Data Launched(Container_Data container_data) // flight mode 'L'
        {
            int maxAlt = container_data.barometer_data.relativeAltitude;
            if (container_data.barometer_data.relativeAltitude < maxAlt){
                altitudeCounter ++;
            }
            else {
                altitudeCounter = 0;
            }

            if (descentCounter >= SAMPLE_COUNT){
                container_data.flight_mode = 'D';
            }
            return container_data;
        }

        Container_Data Deployed(Container_Data container_data) // flight mode 'D'
        
            if (container_data.barometer_data.relativeAltitude < PARACHUTE_DEPLOY_ALTITUDE) {
                parachuteThresholdMetCounter++;
            } 
            else {
                parachuteThresholdMetCounter = 0;
            }

            if (parachuteThresholdMetCounter > SAMPLE_COUNT){
                container_data.flight_mode = 'S';
            }
            return container_data;
        }

        Container_Data ParachuteDeploy(Container_Data container_data) // flight mode 'S'
        {   
            //ParachuteServo parachute(36);
            //parachute.ReleaseParachute();
            
            if (container_data.barometer_data.relativeAltitude <= container_data.barometer_data.relativeAltitude + 1 || container_data.barometer_data.relativeAltitude >= container_data.barometer_data.relativeAltitude -1) {
                stationaryCounter++;
            }
            else {
                stationaryCounter = 0;
            }

            if (stationaryCounter > SAMPLE_COUNT) {
                container_data.flight_mode = 'G';
            }
            return container_data;
        }

        Container_Data Land(Container_Data container_data) // flight mode 'G'
        {
            return container_data;
        }
    };

}