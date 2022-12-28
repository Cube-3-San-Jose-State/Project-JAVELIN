#pragma once
#include <string>
#include "../lib/container-dto.hpp"

namespace CanSat
{
    class MissionControlHandler
    {
    public:
        std::string CansatContainerData(Container_Data data)
        {
            char request_parameter[200];
            snprintf(
                request_parameter, 200, kGETRequestFormat,
                data.id, data.heartbeat_count, data.is_operational, data.deploy_payload, data.flight_mode, data.air_speed, data.gps_data.latitude, data.gps_data.longitude, 
                data.barometer_data.altitude, data.barometer_data.pressure, data.barometer_data.temperature, data.imu_data.acceleration_x, 
                data.imu_data.acceleration_y, data.imu_data.acceleration_z, data.imu_data.gyro_x, data.imu_data.gyro_y, data.imu_data.gyro_z, data.battery_data.voltage);
            return request_parameter;
        }

        Container_Data ParseMissionControlData(std::string &response)
        {
            response = response.substr(response.find('{'));
            int actual_arguments = sscanf(
                response.c_str(), kResponseBodyFormat,
                &container_data_.id, &container_data_.heartbeat_count, &container_data_.is_operational, &container_data_.deploy_payload, &container_data_.flight_mode, &container_data_.air_speed, &container_data_.gps_data.latitude, &container_data_.gps_data.longitude, 
                &container_data_.barometer_data.altitude, &container_data_.barometer_data.pressure, &container_data_.barometer_data.temperature, &container_data_.imu_data.acceleration_x, 
                &container_data_.imu_data.acceleration_y, &container_data_.imu_data.acceleration_z, &container_data_.imu_data.gyro_x, &container_data_.imu_data.gyro_y, &container_data_.imu_data.gyro_z, &container_data_.battery_data.voltage);
            if (actual_arguments != kExpectedNumberOfArguments)
            {
                printf("Received %d arguments, expected %d", actual_arguments, kExpectedNumberOfArguments);
            }
            return container_data_;
        }

    private:
        Container_Data container_data_;
        const int kExpectedNumberOfArguments = 17;
    };

}