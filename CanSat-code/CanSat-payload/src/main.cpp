#include <Arduino.h>
#include "../lib/MPL3115A2.hpp"
#include "../lib/MPU6050.hpp"
#include "../lib/MiniSpyCamera.hpp"
#include "../lib/XBEE.hpp"
#include "../include/mission-control-handler.hpp"
#include "../include/payload-dto.hpp"

using namespace CanSat;

MPL3115A2 barometer(17, 16);
MPU6050 IMU(18, 19);
MiniSpyCamera camera(41);
XBEE transmitter(7, 8);
MissionControlHandler mission_control_handler;
Payload_Data payload_data;
String json_data = "";
int heartbeat = 0;

Payload_Data ReadAllSensors(Payload_Data payload_data){
    payload_data.heartbeat_count = ++heartbeat;
    // Read all sensors
    barometer.Update();
    IMU.Update();

    payload_data.imu_data.acceleration_x = IMU.GetAccelerometer().x;
    payload_data.imu_data.acceleration_y = IMU.GetAccelerometer().y;
    payload_data.imu_data.acceleration_z = IMU.GetAccelerometer().z;
    payload_data.imu_data.gyro_x = IMU.GetGyroscope().x;
    payload_data.imu_data.gyro_y = IMU.GetGyroscope().y;
    payload_data.imu_data.gyro_z = IMU.GetGyroscope().z;
    payload_data.imu_data.pitch = IMU.GetAttitude().pitch;
    payload_data.imu_data.roll = IMU.GetAttitude().roll;
    
    payload_data.barometer_data.temperature = barometer.GetData().temperature;
    payload_data.barometer_data.altitude = barometer.GetData().altitude;
    return payload_data;
}

void setup() {
    Serial.begin(9600);
    barometer.Initialize();
    IMU.Initialize();
    transmitter.Initialize(9600);
    
    payload_data.id = 'P';
    payload_data.flight_mode = 'D';
    payload_data.battery_data.voltage = 0;
}

void loop() {
    payload_data = ReadAllSensors(payload_data);
    json_data = mission_control_handler.JSONifyData(payload_data);
    // Serial.println(json_data);
    delay(100);
}