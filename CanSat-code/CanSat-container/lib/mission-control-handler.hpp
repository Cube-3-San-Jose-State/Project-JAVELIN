#pragma once
#include <string>
#include "../lib/container-dto.hpp"

namespace CanSat
{
    class MissionControlHandler
    {
    public:
        std::string CreateGETRequestParameterWithRoverStatus(Container_Data data)
        {
            char request_parameter[200];
            snprintf(
                request_parameter, 200, kGETRequestFormat,
                data.heartbeat_count, data.is_operational, data.flight_mode, data.air_speed, data.gps_data.latitude, data.gps_data.longitude, 
                data.barometer_data.altitude, data.barometer_data.pressure, data.barometer_data.temperature, data.imu_data.acceleration_x, 
                data.imu_data.acceleration_y, data.imu_data.acceleration_z, data.imu_data.gyro_x, data.imu_data.gyro_y, data.imu_data.gyro_z, data.battery_data.voltage);
            return request_parameter;
        }

        Container_Data ParseMissionControlData(std::string &response)
        {
            response = response.substr(response.find('{'));
            int actual_arguments = sscanf(
                response.c_str(), kResponseBodyFormat,
                &data_.heartbeat_count, &data_.is_operational, &data_.flight_mode, &data_.air_speed, &data_.gps_data.latitude, &data_.gps_data.longitude, 
                &data_.barometer_data.altitude, &data_.barometer_data.pressure, &data_.barometer_data.temperature, &data_.imu_data.acceleration_x, 
                &data_.imu_data.acceleration_y, &data_.imu_data.acceleration_z, &data_.imu_data.gyro_x, &data_.imu_data.gyro_y, &data_.imu_data.gyro_z, &data_.battery_data.voltage);
            if (actual_arguments != kExpectedNumberOfArguments)
            {
                printf("Received %d arguments, expected %d", actual_arguments, kExpectedNumberOfArguments);
            }
            return data_;
        }

    private:
        Container_Data data_;
        const int kExpectedNumberOfArguments = 15;
    };

}