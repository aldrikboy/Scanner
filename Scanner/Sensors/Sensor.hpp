#pragma once

class Sensor
{
public:
    Sensor(char identifier);
    virtual float GetSample() = 0;
    virtual bool IsSampleValid(float sample) = 0;
    char identifier;
};

Sensor::Sensor(char identifier)
{
    this->identifier = identifier;
}