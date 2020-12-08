#pragma once

#include "SensorManager.hpp"
#include "TimeManager.hpp"
#include "StatusManager.hpp"

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
    time_t timestamp;
    Sample samples[MAX_SENSORS];
    int sampleCount = 0;
};

#include "StorageManager.hpp"

class SampleCollector
{
public:
    void CollectSamples(SensorManager *manager);
    unsigned long getSampleCount()
    {
        return StorageManager::ReadSampleCount();
    }
    String PopSample()
    {
        return StorageManager::PopSample();
    }
};

void SampleCollector::CollectSamples(SensorManager *manager)
{
    unsigned long currentLength = StorageManager::ReadSampleCount();
    if (currentLength >= MAX_STORED_SAMPLES)
    {
        StatusManager::Show(0xFF0000, 4); // Storage is full
        return;
    }

    // Retrieve current value of all sensors.
    SampleCollection collection;
    collection.sampleCount = 0;

    collection.timestamp = TimeManager::GetEpoch();
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

    StorageManager::PushSample(collection);
}