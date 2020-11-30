#pragma once

#include "Sensor.hpp"

class UVSensor : public Sensor
{
public:
    UVSensor(char identifier, int pin);
    float GetSample();
    bool IsSampleValid(float sample);

private:
    bool success;
    int pin;
};

UVSensor::UVSensor(char identifier, int pin) : Sensor(identifier)
{
    this->pin = pin;
}

float UVSensor::GetSample()
{
    return analogRead(pin);
}

bool UVSensor::IsSampleValid(float sample)
{
    return !isnan(sample);
}