#pragma once

#include <SD.h>
#include "SampleCollector.hpp"

#define DATA_FILE_NAME "d"
#define COUNT_FILE_NAME "c"
#define OFFSET_FILE_NAME "o"

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
    static long ReadSampleOffset();
    static void WriteSampleOffset(long number);
    static void WriteNumber(String fileName, long number);
    static long ReadNumber(String fileName);
    static void CreateEmpty(String name);
    static bool initialized;
};

bool StorageManager::initialized = false;

long StorageManager::ReadNumber(String fileName)
{
    File file = SD.open(fileName, FILE_READ);
    if (!file)
        return;

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

void StorageManager::WriteNumber(String fileName, long number)
{
    File file = SD.open(fileName, O_READ | O_WRITE | O_CREAT | O_TRUNC);
    if (!file)
        return;

    file.seek(0);
    file.println((String)number);
    file.flush();
    file.close();
}

void StorageManager::CreateEmpty(String name)
{
    if (!SD.exists(name))
    {
        // Create empty file
        File file = SD.open(name, FILE_WRITE);
        file.close();
    }
}

bool StorageManager::Setup()
{
    if (!SD.begin(10))
    {
        return false;
    }

    StorageManager::initialized = true;

    CreateEmpty(COUNT_FILE_NAME);
    CreateEmpty(DATA_FILE_NAME);
    CreateEmpty(OFFSET_FILE_NAME);

    return true;
}

void StorageManager::ClearData()
{
    StorageManager::WriteSampleCount(0);
    StorageManager::WriteSampleOffset(0);
    File file = SD.open(DATA_FILE_NAME, O_READ | O_WRITE | O_CREAT | O_TRUNC);
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

    long offset = ReadSampleOffset();

    File file = SD.open(DATA_FILE_NAME, O_READ | O_WRITE);
    if (!file)
    {
        // Read failure
        return;
    }

    // Read olders sample
    file.seek(offset);
    String result = "";
    while (file.available())
    {
        char ch = file.read();
        if (ch == '\n' || ch == '\r')
        {
            break;
        }
        result += ch;
    }

    file.close();

    StorageManager::WriteSampleCount(ReadSampleCount() - 1);
    StorageManager::WriteSampleOffset(offset + result.length() + 2);

    return result;
}

void StorageManager::PushSample(SampleCollection collection)
{
    String data = "";
    data += (String) "T:";
    data += (String)collection.timestamp;

    for (int i = 0; i < collection.sampleCount; i++)
    {
        data += ',';
        Sample sample = collection.samples[i];

        data += (String)sample.identifier;
        data += (String)':';
        data += (String)sample.sample;
    }

    File file = SD.open(DATA_FILE_NAME, O_READ | O_WRITE | O_CREAT);
    if (!file)
    {
        // Read failure
        return;
    }

    file.seek(file.size());
    file.println(data);
    file.flush();

    // Uncomment to print file content to Serial
#if 0
    Serial.println("");
    file.seek(0);
    while (file.available())
    {
        Serial.print((char)file.read());
    }
#endif

    file.close();

    StorageManager::WriteSampleCount(ReadSampleCount() + 1);
}

void StorageManager::WriteSampleCount(unsigned long count)
{
    WriteNumber(COUNT_FILE_NAME, count);
}

unsigned long StorageManager::ReadSampleCount()
{
    return ReadNumber(COUNT_FILE_NAME);
}

void StorageManager::WriteSampleOffset(long number)
{
    WriteNumber(OFFSET_FILE_NAME, number);
}

long StorageManager::ReadSampleOffset()
{
    return ReadNumber(OFFSET_FILE_NAME);
}