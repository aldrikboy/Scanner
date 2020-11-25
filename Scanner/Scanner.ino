#include "Communication/BluetoothCommunicator.hpp"
#include "Communication/Communicator.hpp"

#include "Sensors/Sensor.hpp"
#include "Sensors/TemperatureSensor.hpp"
#include "SensorManager.hpp"

#include "SampleCollector.hpp"

/*

TODO
- Make bluetoothConnector use parameters when setting up connection
- Get epoch timestamp for sampleCollection
- Set sample interval to 4 hours?
- Find out how much heap we have left for sample buffer

*/

Communicator *communicator = new BluetoothCommunicator("ecoGrounder Scanner");
SensorManager *sensorManager = new SensorManager();
SampleCollector *sampleCollector = new SampleCollector();

void setup()
{
    Serial.begin(9600);

    sensorManager->SetupSensors();
    communicator->SetupConnection();
}

void loop()
{
    delay(1000);
    sampleCollector->CollectSamples(sensorManager);

    if (communicator->IsConnected())
        communicator->HandleIncommingMessages(sampleCollector);
}
