#pragma once

// Specifications for this component
// https://components101.com/dht11-temperature-sensor

#include "Sensor.hpp"
#include "DHT.h"

#define DHTTYPE DHT11

class DHTSensor
{
public:
    DHTSensor(int pin);

protected:
    static DHT dht_instance;
    static bool initialized;
};

bool DHTSensor::initialized = false;
DHT DHTSensor::dht_instance = DHT(3, DHTTYPE); // placeholder

DHTSensor::DHTSensor(int pin)
{
    if (!DHTSensor::initialized)
    {
        DHTSensor::initialized = true;
        DHTSensor::dht_instance = DHT(pin, DHTTYPE);
        dht_instance.begin();
    }
}
