#include "Communication/BluetoothCommunicator.hpp"
#include "Communication/Communicator.hpp"

Communicator *communicator = new BluetoothCommunicator("ecoGrounder Scanner");

void setup()
{
    Serial.begin(9600);
    communicator->SetupConnection();
}

void loop()
{
    communicator->HandleIncommingMessages();
}