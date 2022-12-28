#pragma once
#include <string>
#include "../lib/payload-dto.hpp"

namespace CanSat
{
    class MissionControlHandler
    {
    public:
        std::string CansatPayloadData(Payload_Data data)
        {
            char request_parameter[200];
            snprintf(
                request_parameter, 200, kGETRequestFormat,
                data.id, data.heartbeat_count, data.is_operational, data.flight_mode, data.air_speed, data.camera_status.trig, 
                data.barometer_data.altitude, data.barometer_data.pressure, data.barometer_data.temperature, data.imu_data.acceleration_x, 
                data.imu_data.acceleration_y, data.imu_data.acceleration_z, data.imu_data.gyro_x, data.imu_data.gyro_y, data.imu_data.gyro_z, data.battery_data.voltage);
            return request_parameter;
        }
    };

}