// This is the base class for all communicators.
// This class should not be instantiated.

#pragma once

class Communicator
{
public:
    bool IsConnected();
    virtual void SetupConnection(){};
    void HandleIncommingMessages();
    Communicator(){};

private:
    bool connected = false;

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

void Communicator::HandleIncommingMessages()
{
    char recvChar;
    if (this->DataAvailable())
    {
        recvChar = this->ReadData();
        Serial.print(recvChar);
    }
    if (Serial.available())
    {
        recvChar = Serial.read();
        this->SendData(recvChar);
    }
}

bool Communicator::IsConnected()
{
    return this->connected;
}