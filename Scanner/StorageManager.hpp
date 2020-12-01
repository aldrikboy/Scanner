#pragma once

#include "SampleCollector.hpp"

#include <SD.h>

class StorageManager
{
public:
    static void Setup();
    static void ReadSampleCount();
    static void WriteSampleCount();

    static SampleCollection PopSample();
    static bool PushSample();

private:
    static bool initialized;
    static File file;
};

bool StorageManager::initialized = false;
File StorageManager::file;

void StorageManager::Setup()
{
    if (!SD.begin())
    {
        Serial.println("SD Card init failed.");
        return;
    }

    Serial.println("SD Card is working.");
    StorageManager::initialized = true;

    if (SD.exists("count.txt"))
    {
        file = SD.open("count.txt", FILE_WRITE);
        file.close();
    }

    if (SD.exists("data.txt"))
    {
        file = SD.open("data.txt", FILE_WRITE);
        file.close();
    }
}