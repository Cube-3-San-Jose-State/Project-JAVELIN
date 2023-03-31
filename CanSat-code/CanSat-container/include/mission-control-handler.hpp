#pragma once
#include "../include/container-dto.hpp"

namespace CanSat
{
    class MissionControlHandler
    {
    public:
        String FormatContainerData(Container_Data data)
        {
            char request_parameter[500];
            snprintf(
                request_parameter, 500, kResponseBodyFormat,
                data.id, 
                data.heartbeat_count, 
                data.payload_deployed, 
                data.flight_mode, 
                data.gps_data.latitude, data.gps_data.longitude, 
                data.barometer_data.altitude, data.barometer_data.temperature, 
                data.imu_data.acceleration_x, data.imu_data.acceleration_y, data.imu_data.acceleration_z, 
                data.imu_data.gyro_x, data.imu_data.gyro_y, data.imu_data.gyro_z,
                data.imu_data.pitch, data.imu_data.roll, data.compass_data.magnet_x, data.compass_data.magnet_y, data.compass_data.magnet_z
            );
            return request_parameter;
        }

        Container_Data ParseMissionControlData(std::string &response)
        {
            response = response.substr(response.find('{'));

            int actual_arguments = sscanf(
                response.c_str(), kResponseBodyFormat,
                &container_data_.id, 
                &container_data_.heartbeat_count, 
                &container_data_.payload_deployed, 
                &container_data_.flight_mode, 
                &container_data_.gps_data.latitude, &container_data_.gps_data.longitude, 
                &container_data_.barometer_data.altitude, &container_data_.barometer_data.temperature,
                &container_data_.imu_data.acceleration_x, &container_data_.imu_data.acceleration_y, &container_data_.imu_data.acceleration_z, 
                &container_data_.imu_data.gyro_x, &container_data_.imu_data.gyro_y, &container_data_.imu_data.gyro_z,
                &container_data_.imu_data.pitch, &container_data_.imu_data.pitch
            );

            if (actual_arguments != kExpectedNumberOfArguments) {
                printf("Received %d arguments, expected %d", actual_arguments, kExpectedNumberOfArguments);
            }
            return container_data_;
        }

    private:
        Container_Data container_data_;
        const int kExpectedNumberOfArguments = 18;
    };

}