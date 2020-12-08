#pragma once

#include "Sensor.hpp"
#include <SFE_BMP180.h>

class BarometerSensor : public Sensor
{
public:
    BarometerSensor(char identifier);
    float GetSample();
    bool IsSampleValid(float sample);

private:
    bool success;
    SFE_BMP180 bmp180;
};

BarometerSensor::BarometerSensor(char identifier) : Sensor(identifier)
{
    this->success = bmp180.begin();

    //if (!this->success)
    //    Serial.println("BMP180 Barometer init failed");
}

float BarometerSensor::GetSample()
{
    if (this->success)
        return bmp180.readPressure();
    else
        return NAN;
}

bool BarometerSensor::IsSampleValid(float sample)
{
    return !isnan(sample);
}