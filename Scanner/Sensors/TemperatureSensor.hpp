#pragma once

#include "Sensor.hpp"
#include "DHTSensor.hpp"
#include "DHT.h"

#define DHTTYPE DHT11

class TemperatureSensor : public Sensor, DHTSensor
{
public:
    TemperatureSensor(char identifier, int pin);
    float GetSample();
    bool IsSampleValid(float sample);
};

TemperatureSensor::TemperatureSensor(char identifier, int pin) : Sensor(identifier), DHTSensor(pin)
{
}

float TemperatureSensor::GetSample()
{
    // read temperature as Celsius
    return dht_instance.readTemperature();
}

bool TemperatureSensor::IsSampleValid(float sample)
{
    return !isnan(sample);
}