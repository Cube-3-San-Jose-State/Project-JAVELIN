#pragma once
#include "./payload-dto.hpp";

namespace CanSat
{
    class MissionControlHandler
    {
    public:
        String CansatPayloadData(Payload_Data data)
        {
            char request_parameter[500];
            snprintf(
                request_parameter, 500, kGETRequestFormat,
                data.id, data.heartbeat_count, data.is_operational, data.flight_mode, data.air_speed, data.camera_status.trig, 
                data.barometer_data.altitude, data.barometer_data.temperature, data.imu_data.acceleration_x, 
                data.imu_data.acceleration_y, data.imu_data.acceleration_z, data.imu_data.gyro_x, data.imu_data.gyro_y, data.imu_data.gyro_z, data.imu_data.roll, data.imu_data.pitch, data.battery_data.voltage);
            return request_parameter;
        }
        
        String JSONifyData(Payload_Data data)
        {
            char json_data[500];
            snprintf(
                json_data, 500, kResponseBodyFormat,
                data.id, data.heartbeat_count, data.is_operational, data.flight_mode, data.air_speed, data.camera_status.trig, 
                data.barometer_data.altitude, data.barometer_data.temperature, data.imu_data.acceleration_x, 
                data.imu_data.acceleration_y, data.imu_data.acceleration_z, data.imu_data.gyro_x, data.imu_data.gyro_y, data.imu_data.gyro_z, data.imu_data.roll, data.imu_data.pitch, data.battery_data.voltage);
            Serial.println(json_data);
            return json_data;
        }
    };

}