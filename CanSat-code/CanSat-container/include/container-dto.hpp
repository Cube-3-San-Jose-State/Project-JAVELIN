#pragma once

namespace CanSat
{
    const char kResponseBodyFormat[] = "{\"ID\":\"%c\",\"HRB\":%d,\"ISO\":%d,\"DEP\":%d,\"FLM\":\"%c\",\"SPD\":%f,\"GPS\":[%lf,%lf],\"BAR\":[%f,%f,%f],\"IMU\":[%f,%f,%f,%f,%f,%f],\"BAT\":%f}\n";
    const char kGETRequestFormat[] = "container?id=%c&heartbeat_count=%d&is_operational=%d&payload_deployed=%d&flight_mode=%c&air_speed=%f&latitude=%lf,&longitufe=%lf,&altitufe=%f,&pressure=%f,&temperature=%f,&acceleration_x=%f,&acceleration_y=%f,&acceleration_z=%f,&gyro_x=%f,&gyro_y=%f,&gyro_z=%f,&voltage=%f";

    struct GPS_Data
    {
        double latitude;
        double longitude;
    };

    struct Barometer_Data
    {
        float altitude;
        float temperature;
    };

    struct IMU_Data
    {
        float acceleration_x;
        float acceleration_y;
        float acceleration_z;
        float gyro_x;
        float gyro_y;
        float gyro_z;
    };

    struct Battery_Data
    {
        float voltage;
    };

    struct Container_Data
    {
        char id;
        int heartbeat_count;
        int is_operational;
        int payload_deployed;
        char flight_mode;
        float air_speed;
        GPS_Data gps_data;
        Barometer_Data barometer_data;
        IMU_Data imu_data;
        Battery_Data battery_data;
    };

}