// This is the base class for all communicators.
// This class should not be instantiated.

// https://github.com/ricaun/ArduinoUniqueID

#pragma once

#include "MessageHandler.hpp"
#include "../SampleCollector.hpp"

class Communicator
{
public:
    bool IsConnected();
    virtual void SetupConnection(){};
    void HandleIncommingMessages(SampleCollector *sampleCollector);
    Communicator(){};

private:
    bool connected = false;
    String messageBuffer = "";
    MessageHandler messageHandler;

protected:
    void setConnected(bool connected)
    {
        this->connected = connected;
    }
    bool getConnected()
    {
        return this->connected;
    }

    virtual bool DataAvailable() = 0;
    virtual char ReadData() = 0;
    virtual void SendData(char data) = 0;
};

void Communicator::HandleIncommingMessages(SampleCollector *sampleCollector)
{
    char recvChar;
    while (this->DataAvailable())
    {
        recvChar = this->ReadData();

        // Beginning of protocol message
        if (recvChar == '{')
            messageBuffer = "";

        messageBuffer += recvChar;

        // End of protocol message
        if (recvChar == '}')
        {
            String response = messageHandler.HandleMessage(messageBuffer, sampleCollector);
            for (char &c : response)
            {
                this->SendData(c);
            }
        }
    }

    // For debugging purposes, send data from serial to communicator.
    while (Serial.available())
    {
        recvChar = Serial.read();
        this->SendData(recvChar);
    }
}

bool Communicator::IsConnected()
{
    return this->connected;
}