#pragma once

#include <SD.h>
#include <stdlib.h>
#include "SampleCollector.hpp"

#define DATA_FILE_NAME "d"
#define COUNT_FILE_NAME "c"

// 14.4gb effective storage on 16gb card
#define SENSOR_SAMPLE_RESERVE_SLOTS 8                                                // Reserve 8 slots for future expansions
#define TIMESTAMP_SIZE 10                                                            // length of unix time as string
#define SAMPLE_SIZE 10                                                               // max length of a sample as string
#define COLLECTION_SIZE (TIMESTAMP_SIZE + SENSOR_SAMPLE_RESERVE_SLOTS * SAMPLE_SIZE) // Size of a sample collection
#define MAX_STORED_SAMPLES (14400000000 / COLLECTION_SIZE)                           // 160 million

class StorageManager
{
public:
    static bool Setup();
    static unsigned long ReadSampleCount();
    static void WriteSampleCount(unsigned long number);
    static void ClearData();
    static void PushSample(SampleCollection collection);
    static String PopSample();

private:
    int NextLine(char *buffer, int bufLength);
    static bool initialized;
    static File file;
};

bool StorageManager::initialized = false;
File StorageManager::file;

bool StorageManager::Setup()
{
    if (!SD.begin(10))
    {
        return false;
    }

    StorageManager::initialized = true;

    if (!SD.exists(COUNT_FILE_NAME))
    {
        // Create empty count file
        file = SD.open(COUNT_FILE_NAME, FILE_WRITE);
        file.close();
    }

    if (!SD.exists(DATA_FILE_NAME))
    {
        // Create empty data file
        file = SD.open(DATA_FILE_NAME, FILE_WRITE);
        file.close();
    }

    return true;
}

void StorageManager::ClearData()
{
    StorageManager::WriteSampleCount(0);
    file = SD.open(DATA_FILE_NAME, O_READ | O_WRITE | O_CREAT | O_TRUNC);
    if (file)
    {
        file.seek(0);
        file.flush();
        file.close();
    }
}

String StorageManager::PopSample()
{
    if (ReadSampleCount() == 0)
        return "";

    file = SD.open(DATA_FILE_NAME, O_READ | O_WRITE);
    if (!file)
    {
        // Read failure
        return;
    }

    // Read olders sample
    file.seek(0);
    String result = "";
    bool isNewCollection = false;
    while (file.available())
    {
        char ch = file.peek();
        if (ch == '\n' || ch == '\r')
        {
            if (isNewCollection)
            {
                break;
            }
            else
            {
                file.read();
                result = "";
                continue;
            }
        }
        if (ch == 'T')
        {
            file.write('X'); // Replace T with X here. Marking collection as old.
            isNewCollection = true;
        }
        else
        {
            file.read();
        }
        result += ch;
    }

    file.close();

    StorageManager::WriteSampleCount(ReadSampleCount() - 1);

    return result;
}

void StorageManager::PushSample(SampleCollection collection)
{
    String data = "";
    data += (String) "T:";
    data += (String)collection.timestamp;

    for (int i = 0; i < collection.sampleCount; i++)
    {
        data += ",";
        Sample sample = collection.samples[i];

        data += (String)sample.identifier;
        data += (String) ":";
        data += (String)sample.sample;
    }

    file = SD.open(DATA_FILE_NAME, O_READ | O_WRITE | O_CREAT);
    if (!file)
    {
        // Read failure
        return;
    }

    file.seek(file.size());
    file.println(data);
    file.flush();

    // Uncomment to print file content to Serial
    // Serial.println("");
    // file.seek(0);
    // while (file.available())
    // {
    //     Serial.print((char)file.read());
    // }

    int size = file.size();
    file.close();

    StorageManager::WriteSampleCount(ReadSampleCount() + 1);
}

void StorageManager::WriteSampleCount(unsigned long count)
{
    file = SD.open(COUNT_FILE_NAME, O_READ | O_WRITE | O_CREAT | O_TRUNC);
    if (!file)
    {
        // Read failure
        return;
    }

    file.seek(0);
    int result = file.println((String)count);
    file.flush();
    file.close();
}

unsigned long StorageManager::ReadSampleCount()
{
    file = SD.open(COUNT_FILE_NAME, FILE_READ);
    if (!file)
    {
        // Read failure
        return;
    }

    file.seek(0);
    String result = "";
    while (file.available())
    {
        char ch = file.read();
        if (ch == '\n' || ch == '\r')
            break;
        result += ch;
    }

    file.close();
    return result.toInt();
}