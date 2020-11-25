#pragma once

#include "SensorManager.hpp"

#include <stdio.h>

#define MAX_SAMPLE_STORAGE 10
#define INVALID_VALUE -999

class Sample
{
public:
    char identifier;
    float sample;
};

class SampleCollection
{
public:
    unsigned long timestamp;
    Sample samples[MAX_SENSORS];
    int sampleCount = 0;
};

class SampleCollector
{
public:
    void CollectSamples(SensorManager *manager);
    int getSampleCount()
    {
        return sampleCount;
    }
    SampleCollection getSample(int index)
    {
        return samples[index];
    }
    SampleCollection PopSample()
    {
        // remove and return last sample
        if (sampleCount)
        {
            sampleCount--;
            return getSample(sampleCount);
        }
    }

private:
    SampleCollection samples[MAX_SAMPLE_STORAGE];
    int sampleCount = 0;
};

void SampleCollector::CollectSamples(SensorManager *manager)
{
    // Retrieve current value of all sensors.
    SampleCollection collection;
    collection.sampleCount = 0;

    collection.timestamp = millis(); // get epoch here
    for (int i = 0; i < manager->getSensorCount(); i++)
    {
        Sensor *sensor = manager->getSensor(i);

        if (sensor)
        {
            collection.samples[collection.sampleCount].identifier = sensor->identifier;
            collection.samples[collection.sampleCount].sample = sensor->GetSample();

            // If sample is invalid, ignore it.
            if (sensor->IsSampleValid(collection.samples[collection.sampleCount].sample))
            {
                collection.sampleCount++;
            }
        }
    }

    if (sampleCount >= MAX_SAMPLE_STORAGE)
    {
        // Move all values 1 slot down
        for (int i = 0; i < sampleCount; i++)
        {
            if (i >= sampleCount - 1)
                break;

            samples[i] = samples[i + 1];
        }

        samples[sampleCount - 1] = collection;
    }
    else
    {
        samples[sampleCount++] = collection;
    }
}