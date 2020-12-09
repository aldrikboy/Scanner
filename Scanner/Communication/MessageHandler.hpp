#pragma once

#include "../SensorManager.hpp"
#include "../SampleCollector.hpp"
#include <ArduinoUniqueID.h>

class MessageHandler
{
public:
    String HandleMessage(String message, SampleCollector *sampleCollector);
};

String MessageHandler::HandleMessage(String message, SampleCollector *sampleCollector)
{
    //Serial.println("MESSAGE: " + message);
    if (message == "{COUNT}")
    {
        String response = "{";
        response += (String)sampleCollector->getSampleCount();
        response += "}\n";

        return response;
    }
    else if (message == "{UPDATE}")
    {
        if (sampleCollector->getSampleCount())
        {
            String response = "{";

            String data = sampleCollector->PopSample();

            if (data != "")
            {
                response += (String) "I:";

                for (size_t i = 0; i < UniqueIDsize; i++)
                    response += (String)UniqueID[i];
                response += ',';

                response += data;
            }
            response += "}\n";
            return response;
        }
    }
    else
    {
        // Unknown message received.
    }

    return "{}\n";
}