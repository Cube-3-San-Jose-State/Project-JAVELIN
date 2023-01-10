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
MPL3115A barometer;
MPU6050 IMU;
PA1616S GPS;
XBEE xbee(2, 3);
String Json_data = "";


Container_Data ReadAllSensors(Container_Data container_data){
    // Read all sensors
    barometer.update();
    IMU.ReadMPU();
    GPS.update();

    container_data.imu_data.acceleration_x = IMU.GetAccel().x;
    container_data.imu_data.acceleration_y = IMU.GetAccel().y;
    container_data.imu_data.acceleration_z = IMU.GetAccel().z;
    container_data.imu_data.gyro_x = IMU.GetGyro().x;
    container_data.imu_data.gyro_y = IMU.GetGyro().y;
    container_data.imu_data.gyro_z = IMU.GetGyro().z;
    
    container_data.gps_data.latitude = GPS.getLatitude();
    container_data.gps_data.longitude = GPS.getLongitude();
    container_data.barometer_data.temperature = barometer.getTemperature();
    container_data.barometer_data.pressure = barometer.getPressure();
    container_data.barometer_data.altitude = barometer.getAltitude();

    return container_data;
}


void setup() {
    Serial.begin(9600);
    barometer.Initialize();
    IMU.Initialize();
    GPS.Initialize();
    xbee.Initialize(9600);

}

void loop() {
    container_data = ReadAllSensors(container_data);
    container_data = mode_select.SelectMode(container_data);
    Json_data = mission_control_handler.CansatContainerData(container_data);
    xbee.transmitData(Json_data);
    delay(1000);
}

