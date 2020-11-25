#pragma once

#define MAX_SENSORS 6

#include "Sensors/Sensor.hpp"
#include "Sensors/TemperatureSensor.hpp"

// Pin configuration
#define PIN_TEMPERATURE 3

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
    sensors[sensorCount++] = new TemperatureSensor('C', PIN_TEMPERATURE);
}