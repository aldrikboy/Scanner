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
- write offset of next collection to return to new file to stop iterating over entire file
- Set sample interval to 4 hours?
- Handle errors and display on leds
- make constants for led colors
- are leds PWM?
- maak legenda voor statusmanager
*/

Communicator *communicator = new BluetoothCommunicator();
SensorManager *sensorManager = new SensorManager();
SampleCollector *sampleCollector = new SampleCollector();

void setup()
{
    Serial.begin(9600);

    sensorManager->SetupSensors();
    communicator->SetupConnection();
    StatusManager::Setup();
    TimeManager::Setup();

    if (!StorageManager::Setup())
        StatusManager::Report(0xFFFF00);

    StorageManager::ClearData();

    //Serial.println("Available bytes: " + (String)freeMemory());
    //Serial.println("Max samples: " + (String)((unsigned long)MAX_STORED_SAMPLES) + ", Stored: " + (String)StorageManager::ReadSampleCount());
}

void loop()
{
    delay(5000);

    StatusManager::Show(0x0000FF, 2); // Notify about sampling
    sampleCollector->CollectSamples(sensorManager);
    if (communicator->IsConnected())
    {
        StatusManager::Show(0x00FF00, 2); // Notify about message handling
        communicator->HandleIncommingMessages(sampleCollector);
    }
    else
    {
        StatusManager::Show(0xFF0000, 2); // Notify about communication error
    }

    //PowerManager::WaitForNextIteration();
}
