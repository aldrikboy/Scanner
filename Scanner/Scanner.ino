#include "Communication/BluetoothCommunicator.hpp"
#include "Communication/Communicator.hpp"

#include "Sensors/Sensor.hpp"
#include "Sensors/TemperatureSensor.hpp"
#include "SensorManager.hpp"

#include "SampleCollector.hpp"
#include "StatusManager.hpp"
#include "PowerManager.hpp"
#include "TimeManager.hpp"
//#include "StorageManager.hpp"

#include <MemoryFree.h>

/*

TODO
- Set sample interval to 4 hours?
- Write data to SD card
- begin returning data from bottom instead of top of samples to return oldest first

*/

Communicator *communicator = new BluetoothCommunicator("ecoGrounder Scanner");
SensorManager *sensorManager = new SensorManager();
SampleCollector *sampleCollector = new SampleCollector();

void setup()
{
    Serial.begin(9600);

    sensorManager->SetupSensors();
    communicator->SetupConnection();
    StatusManager::Setup();
    TimeManager::Setup();

    //StorageManager::Setup();

    Serial.println("Available bytes: " + (String)freeMemory());
}

void loop()
{
    delay(3000);

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
