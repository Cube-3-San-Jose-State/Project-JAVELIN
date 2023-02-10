#include <Arduino.h>
#include "../lib/MPL3115A.hpp"
#include "../lib/MPU6050.hpp"
#include "../lib/PA1616S.hpp"
#include "../lib/XBEE.hpp"
#include "../include/mission-control-handler.hpp"
#include "../include/mode-select.hpp"
#include "../include/container-dto.hpp"

using namespace CanSat;

ModeSelect mode_select;
MissionControlHandler mission_control_handler;
Container_Data container_data;
MPL3115A barometer(18, 19);
MPU6050 IMU(17, 16);
PA1616S GPS;
XBEE xbee(14, 15);
String json_data = "";
int heartbeat = 0;

Container_Data ReadAllSensors(Container_Data container_data){
    // Read all sensors
    barometer.Update();
    IMU.Update();
    GPS.Update();

    container_data.heartbeat_count = ++heartbeat;

    container_data.imu_data.acceleration_x = IMU.GetAccelerometer().x;
    container_data.imu_data.acceleration_y = IMU.GetAccelerometer().y;
    container_data.imu_data.acceleration_z = IMU.GetAccelerometer().z;
    container_data.imu_data.gyro_x = IMU.GetGyroscope().x;
    container_data.imu_data.gyro_y = IMU.GetGyroscope().y;
    container_data.imu_data.gyro_z = IMU.GetGyroscope().z;
    
    container_data.gps_data.latitude = GPS.GetLatitude();
    container_data.gps_data.longitude = GPS.GetLongitude();
    container_data.barometer_data.temperature = barometer.GetTemperature();
    container_data.barometer_data.altitude = barometer.GetAltitude();
    return container_data;
}


void setup() {
    Serial.begin(9600);
    barometer.Initialize();
    IMU.Initialize();
    GPS.Initialize();
    xbee.Initialize(9600);

    container_data.id = 'C';
    container_data.flight_mode = 'D';
    container_data.battery_data.voltage = 0;
}

void loop() {
    container_data = ReadAllSensors(container_data);
    container_data = mode_select.SelectMode(container_data);
    json_data = mission_control_handler.CansatContainerData(container_data);
    Serial.println(json_data);
    xbee.transmitData("From Container: \n");
    xbee.transmitData(json_data);
    delay(1000);
}

