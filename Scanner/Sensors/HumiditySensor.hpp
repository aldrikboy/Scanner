#pragma once

#include "Sensor.hpp"
#include "DHTSensor.hpp"
#include "DHT.h"

#define DHTTYPE DHT11

class HumiditySensor : public Sensor, DHTSensor
{
public:
    HumiditySensor(char identifier, int pin);
    float GetSample();
    bool IsSampleValid(float sample);
};

HumiditySensor::HumiditySensor(char identifier, int pin) : Sensor(identifier), DHTSensor(pin)
{
}

float HumiditySensor::GetSample()
{
    return dht_instance.readHumidity();
}

bool HumiditySensor::IsSampleValid(float sample)
{
    return !isnan(sample);
}