#pragma once
#include <string>
#include "../lib/payload-dto.hpp"

namespace CanSat
{
    class MissionControlHandler
    {
    public:
        std::string CreateGETRequestParameterWithRoverStatus(Payload_Data data)
        {
            char request_parameter[200];
            snprintf(
                request_parameter, 200, kGETRequestFormat,
                data.id, data.heartbeat_count, data.is_operational, data.flight_mode, data.air_speed, data.camera_status.trig, 
                data.barometer_data.altitude, data.barometer_data.pressure, data.barometer_data.temperature, data.imu_data.acceleration_x, 
                data.imu_data.acceleration_y, data.imu_data.acceleration_z, data.imu_data.gyro_x, data.imu_data.gyro_y, data.imu_data.gyro_z, data.battery_data.voltage);
            return request_parameter;
        }

        Payload_Data ParseMissionControlData(std::string &response)
        {
            response = response.substr(response.find('{'));
            int actual_arguments = sscanf(
                response.c_str(), kResponseBodyFormat,
                &payload_data_.id, &payload_data_.heartbeat_count, &payload_data_.is_operational, &payload_data_.flight_mode, &payload_data_.air_speed, &payload_data_.camera_status.trig, 
                &payload_data_.barometer_data.altitude, &payload_data_.barometer_data.pressure, &payload_data_.barometer_data.temperature, &payload_data_.imu_data.acceleration_x, 
                &payload_data_.imu_data.acceleration_y, &payload_data_.imu_data.acceleration_z, &payload_data_.imu_data.gyro_x, &payload_data_.imu_data.gyro_y, &payload_data_.imu_data.gyro_z, &payload_data_.battery_data.voltage);
            if (actual_arguments != kExpectedNumberOfArguments)
            {
                printf("Received %d arguments, expected %d", actual_arguments, kExpectedNumberOfArguments);
            }
            return payload_data_;
        }

    private:
        Payload_Data payload_data_;
        const int kExpectedNumberOfArguments = 15;
    };

}