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

/*

TODO
- Set sample interval to 4 hours?
- maak legenda voor statusmanager
- remove global File variable
*/

Communicator *communicator = new BluetoothCommunicator();
SensorManager *sensorManager = new SensorManager();
SampleCollector *sampleCollector = new SampleCollector();

#define REPORT_FASE(_f)    \
    Serial.print("Fase "); \
    Serial.println(#_f);

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
    StatusManager::Show(COLOR_BLUE, 2); // Notify about sampling
    sampleCollector->CollectSamples(sensorManager);
    if (communicator->IsConnected())
    {
        StatusManager::Show(COLOR_GREEN, 2); // Notify about message handling
        communicator->HandleIncommingMessages(sampleCollector);
    }
    else
    {
        StatusManager::Show(0xFF0000, 2); // Notify about communication error
    }

    delay(5000);
}
