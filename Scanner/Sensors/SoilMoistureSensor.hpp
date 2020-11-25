#pragma once

#include "Sensor.hpp"

class SoilMoistureSensor : public Sensor
{
public:
    SoilMoistureSensor(char identifier, int pin);
    float GetSample();
    bool IsSampleValid(float sample);

private:
    int pin;
};

SoilMoistureSensor::SoilMoistureSensor(char identifier, int pin) : Sensor(identifier)
{
    this->pin = pin;
    pinMode(pin, INPUT);
}

float SoilMoistureSensor::GetSample()
{
    return analogRead(pin);
}

bool SoilMoistureSensor::IsSampleValid(float sample)
{
    return !isnan(sample);
}