#include <unordered_map>
#include "../include/container-dto.hpp"
#include "../lib/ParachuteServo.hpp"

#define PREFLIGHT_EXIT_ALTITUDE 3 // Leaves Preflight at +3m from starting altitude AND
#define PREFLIGHT_EXIT_ACCEL 2 // Leaves Preflight at +2m/s

#define LAUNCHED_EXIT_ACCEL -2 // Leaves launched at -2m/s AND
#define LAUNCHED_EXIT_STREAK 5 // Leaves launched after 5 readings of descending values

#define DEPLOYED_EXIT_ALTITUDE 5 // Leaves deployed at +5m from starting altitude AND
#define DEPLOYED_EXIT_STREAK 5 // Leaves deployed after 5 readings of values under threshold

#define PARACHUTE_EXIT_ALTITUDE_RANGE 1 //Leaves parachute if altitude is within +-1m AND
#define PARACHUTE_EXIT_ACCEL 1 //Leaves parachute if altitude is < 1m/s AND
#define PARACHUTE_EXIT_STREAK 5 // Leaves parachute if above is true 5 times

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
        //Launched variables
        float maxAltitude = 0;
        int fallingCounter = 0;
        //Deployed variables
        int deployParachuteCounter = 0;
        //Parachute variables
        // ParachuteServo parachute(36);
        bool parachuteOpened = false;
        int groundedCounter;
        // Grounded variables
    public:
        Container_Data PreFlight(Container_Data container_data) // flight mode 'U'
        {  
            //Average starting altitude. Collects 20 samples then once done collecting, sets it
            if (container_data.heartbeat_count > 10 && container_data.heartbeat_count < 30) {
                initialAltitude += container_data.barometer_data.relativeAltitude;
            }

            if (initialAltitudeSet == false && container_data.heartbeat_count > 30) {
                initialAltitudeSet = true;
                initialAltitude /= 20;
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
            bool preflightAltitudeThresholdMet = container_data.barometer_data.relativeAltitude > PREFLIGHT_EXIT_ALTITUDE;
            bool preflightAccelThresholdMet = (container_data.imu_data.acceleration_y * ACCEL_CONVERSION) >= PREFLIGHT_EXIT_ACCEL;
            bool gpsSet = (container_data.gps_data.latitude != 0.0 && container_data.gps_data.latitude != 0.0); 
            //bool gpsSet = true; // Uncomment for testing purposes

            if (preflightAltitudeThresholdMet && preflightAccelThresholdMet && gpsSet){
                container_data.flight_mode = 'L';
            }

            return container_data;
        }

        Container_Data Launched(Container_Data container_data) // flight mode 'L'
        {
            if (container_data.barometer_data.relativeAltitude < maxAltitude){
                fallingCounter++;
            } else {
                maxAltitude = container_data.barometer_data.relativeAltitude;
                fallingCounter = 0;
            }

            bool fallingAccelThresholdMet = (container_data.imu_data.acceleration_y * ACCEL_CONVERSION) <= LAUNCHED_EXIT_ACCEL;
            bool fallingThresholdMet = fallingCounter >= LAUNCHED_EXIT_STREAK;
            if (fallingAccelThresholdMet && fallingThresholdMet) {
                container_data.flight_mode = 'D';
            }

            return container_data;
        }

        Container_Data Deployed(Container_Data container_data) // flight mode 'D'
        {
            if (container_data.barometer_data.relativeAltitude < DEPLOYED_EXIT_ALTITUDE) {
                deployParachuteCounter++;
            } 
            else {
                deployParachuteCounter = 0;
            }

            bool deployParachuteThresholdMet = deployParachuteCounter >= DEPLOYED_EXIT_STREAK;
            if (deployParachuteThresholdMet) { 
                container_data.flight_mode = 'S';
            }

            return container_data;
        }

        Container_Data ParachuteDeploy(Container_Data container_data) // flight mode 'S'
        {
            // if (parachuteOpened == false) {
            //     parachute.ReleaseParachute();
            //     parachuteOpened = true;
            // }
            
            if (container_data.barometer_data.relativeAltitude <= PARACHUTE_EXIT_ALTITUDE_RANGE && container_data.barometer_data.relativeAltitude >= -PARACHUTE_EXIT_ALTITUDE_RANGE) {
                groundedCounter++;
            }
            else {
                groundedCounter = 0;
            }
            
            bool parachuteRangeThresholdMet = groundedCounter >= PARACHUTE_EXIT_STREAK;
            bool parachuteAccelThresholdMet = container_data.imu_data.acceleration_y < PARACHUTE_EXIT_ACCEL;

            if (parachuteRangeThresholdMet && parachuteAccelThresholdMet) {
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