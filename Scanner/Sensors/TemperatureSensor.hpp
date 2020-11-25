#pragma once

#include "Sensor.hpp"
#include "DHT.h"

#define DHTTYPE DHT11

class TemperatureSensor : public Sensor
{
public:
    TemperatureSensor(char identifier, int pin);
    float GetSample();
    bool IsSampleValid(float sample);

private:
    DHT dht;
};

TemperatureSensor::TemperatureSensor(char identifier, int pin) : dht(pin, DHTTYPE), Sensor(identifier)
{
    dht.begin();
}

float TemperatureSensor::GetSample()
{
    // read temperature as Celsius
    return dht.readTemperature();
}

bool TemperatureSensor::IsSampleValid(float sample)
{
    return true;
}