#pragma once
#include "../lib/payload-dto.hpp"

namespace CanSat
{
    class MissionControlHandler
    {
    public:
        String CansatPayloadData(Payload_Data data)
        {
            char request_parameter[512];
            snprintf(
                request_parameter, 512, kGETRequestFormat,
                data.id, data.heartbeat_count, data.is_operational, data.flight_mode, data.air_speed, data.camera_status.trig, 
                data.barometer_data.altitude, data.barometer_data.pressure, data.barometer_data.temperature, data.imu_data.acceleration_x, 
                data.imu_data.acceleration_y, data.imu_data.acceleration_z, data.imu_data.gyro_x, data.imu_data.gyro_y, data.imu_data.gyro_z, data.imu_data.roll, data.imu_data.pitch, data.battery_data.voltage);
            return request_parameter;
        }
        
        std::string JSONifyData(Payload_Data data)
        {
            char json_data[512];
            snprintf(
                json_data, 512, kResponseBodyFormat,
                data.id, data.heartbeat_count, data.is_operational, data.flight_mode, data.air_speed, data.camera_status.trig, 
                data.barometer_data.altitude, data.barometer_data.pressure, data.barometer_data.temperature, data.imu_data.acceleration_x, 
                data.imu_data.acceleration_y, data.imu_data.acceleration_z, data.imu_data.gyro_x, data.imu_data.gyro_y, data.imu_data.gyro_z, data.imu_data.roll, data.imu_data.pitch, data.battery_data.voltage);
            return json_data;
        }
    };

}