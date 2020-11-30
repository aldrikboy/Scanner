#pragma once

#define MAX_SENSORS 6

#include "Sensors/Sensor.hpp"
#include "Sensors/TemperatureSensor.hpp"
#include "Sensors/HumiditySensor.hpp"
#include "Sensors/SoilMoistureSensor.hpp"
#include "Sensors/BarometerSensor.hpp"
#include "Sensors/UVSensor.hpp"

// Pin configuration
#define PIN_TEMPERATURE 3
#define PIN_HUMIDITY 3
#define PIN_SOILMOISTURE A0
#define PIN_UV A3

class SensorManager
{
public:
    void SetupSensors();
    int getSensorCount()
    {
        return sensorCount;
    }
    Sensor *getSensor(int index)
    {
        if (index < 0)
            return 0;
        if (index >= sensorCount)
            return 0;
        return sensors[index];
    }

private:
    Sensor *sensors[MAX_SENSORS];
    int sensorCount = 0;
};

void SensorManager::SetupSensors()
{
    sensors[sensorCount++] = new HumiditySensor('L', PIN_HUMIDITY);
    sensors[sensorCount++] = new TemperatureSensor('C', PIN_TEMPERATURE);
    sensors[sensorCount++] = new SoilMoistureSensor('S', PIN_SOILMOISTURE);
    //sensors[sensorCount++] = new BarometerSensor('B');
    //sensors[sensorCount++] = new UVSensor('U', PIN_UV);
}