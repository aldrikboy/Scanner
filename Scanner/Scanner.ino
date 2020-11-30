#include "Communication/BluetoothCommunicator.hpp"
#include "Communication/Communicator.hpp"

#include "Sensors/Sensor.hpp"
#include "Sensors/TemperatureSensor.hpp"
#include "SensorManager.hpp"

#include "SampleCollector.hpp"
#include "StatusManager.hpp"
#include "PowerManager.hpp"
#include "TimeManager.hpp"

/*

TODO
- Get epoch timestamp for sampleCollection
- Set sample interval to 4 hours?
- Do an extra scan the moment the user requests for data

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
