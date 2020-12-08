#pragma once

#include "Communicator.hpp"

#include <SoftwareSerial.h>

using namespace std;

#define RxD 6
#define TxD 7

#define DEBUG_ENABLED 1

class BluetoothCommunicator : public Communicator
{
public:
    BluetoothCommunicator(char *displayName);
    void SetupConnection();

private:
    SoftwareSerial blueToothSerial;
    char *displayName = 0;

protected:
    virtual bool DataAvailable() override;
    virtual char ReadData() override;
    virtual void SendData(char data) override;
};

BluetoothCommunicator::BluetoothCommunicator(char *displayName) : blueToothSerial(RxD, TxD)
{
    this->displayName = displayName;
    pinMode(RxD, INPUT);
    pinMode(TxD, OUTPUT);
}

bool BluetoothCommunicator::DataAvailable()
{
    return blueToothSerial.available();
}

char BluetoothCommunicator::ReadData()
{
    return blueToothSerial.read();
}

void BluetoothCommunicator::SendData(char data)
{
    blueToothSerial.print(data);
}

void BluetoothCommunicator::SetupConnection()
{
    // Set BluetoothBee BaudRate to default baud rate 9600
    blueToothSerial.begin(9600);

    // // set the bluetooth work in slave mode
    // blueToothSerial.print("\r\n+STWMOD=0\r\n");

    // // set the bluetooth name as "SeeedBTSlave"
    // blueToothSerial.print("\r\n+STNA=" + (String)this->displayName + "\r\n");

    // // Permit Paired device to connect me
    // blueToothSerial.print("\r\n+STOAUT=1\r\n");

    // // Auto-connection should be forbidden here
    // blueToothSerial.print("\r\n+STAUTO=0\r\n");

    // This delay is required.
    delay(2000);

    // make the slave bluetooth inquirable
    // blueToothSerial.print("\r\n+INQ=1\r\n");

    Serial.println("The slave bluetooth is inquirable!");

    // This delay is required.
    delay(2000);
    blueToothSerial.flush();

    this->setConnected(true);
}