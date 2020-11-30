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
    Serial.println("MESSAGE: " + message);
    if (message == "{COUNT}")
    {
        String response = "{";
        response += (String)sampleCollector->getSampleCount();
        response += "}\n";

        return response;
    }
    else if (message == "{UPDATE}")
    {
        String response = "{";
        if (sampleCollector->getSampleCount())
        {
            SampleCollection collection = sampleCollector->PopSample();

            response += (String) "I:";

            for (size_t i = 0; i < UniqueIDsize; i++)
                response += (String)UniqueID[i];

            response += (String) ",T:";
            response += (String)collection.timestamp;

            for (int i = 0; i < collection.sampleCount; i++)
            {
                response += ",";
                Sample sample = collection.samples[i];

                response += (String)sample.identifier;
                response += (String) ":";
                response += (String)sample.sample;
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