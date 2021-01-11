#include "Communication/BluetoothCommunicator.hpp"
#include "Communication/Communicator.hpp"

#include "Sensors/Sensor.hpp"
#include "Sensors/TemperatureSensor.hpp"
#include "SensorManager.hpp"

#include "SampleCollector.hpp"
#include "StatusManager.hpp"
#include "PowerManager.hpp"
#include "TimeManager.hpp"
#include "StorageManager.hpp"

#include <MemoryFree.h>

Communicator *communicator = new BluetoothCommunicator();
SensorManager *sensorManager = new SensorManager();
SampleCollector *sampleCollector = new SampleCollector();

#define REPORT_FASE(_f)    \
    Serial.print("Fase "); \
    Serial.println(#_f);

#define SAMPLE_INTERVAL_TIME_SEC 60 * 1 // 1 Minute

time_t lastSampleTimestamp = 0;

void setup()
{
    Serial.begin(9600);

    REPORT_FASE(1);
    StatusManager::Setup();

    REPORT_FASE(2);
    sensorManager->SetupSensors();

    REPORT_FASE(3);
    communicator->SetupConnection();

    REPORT_FASE(4);
    if (!TimeManager::Setup())
        StatusManager::Report(COLOR_RED);

    REPORT_FASE(5);
    if (!StorageManager::Setup())
        StatusManager::Report(COLOR_GREEN);

    StorageManager::ClearData();

    //Serial.println("Available bytes: " + (String)freeMemory());
    //Serial.println("Max samples: " + (String)((unsigned long)MAX_STORED_SAMPLES) + ", Stored: " + (String)StorageManager::ReadSampleCount());
}

void loop()
{
    time_t timestamp = TimeManager::GetEpoch();

    if (timestamp - lastSampleTimestamp >= SAMPLE_INTERVAL_TIME_SEC)
    {
        StatusManager::Show(COLOR_BLUE, 2); // Notify about sampling
        sampleCollector->CollectSamples(sensorManager);

        lastSampleTimestamp = timestamp;
    }

    if (communicator->IsConnected())
    {
        StatusManager::Show(COLOR_GREEN, 2); // Notify about message handling
        communicator->HandleIncommingMessages(sampleCollector);
    }
    else
    {
        StatusManager::Show(COLOR_RED, 2); // Notify about communication error
    }

    PowerManager::Sleep();
}
