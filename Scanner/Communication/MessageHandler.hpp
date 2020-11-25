#pragma once

#include "../SensorManager.hpp"
#include "../SampleCollector.hpp"

class MessageHandler
{
public:
    String HandleMessage(String message, SampleCollector *sampleCollector);
};

String MessageHandler::HandleMessage(String message, SampleCollector *sampleCollector)
{
    if (message == "{COUNT}")
    {
        String response = "{";
        response += (String)sampleCollector->getSampleCount();
        response += "}";

        return response;
    }
    else if (message == "{UPDATE}")
    {
        String response = "{";
        if (sampleCollector->getSampleCount())
        {
            SampleCollection collection = sampleCollector->PopSample();

            response += (String) "T:";
            response += (String)collection.timestamp;

            for (int i = 0; i < collection.sampleCount; i++)
            {
                response += ",";
                Sample sample = collection.samples[i];

                response += (String)sample.identifier;
                response += (String) ":";
                response += (String)sample.sample;
            }
            response += "}";

            return response;
        }
    }
    else
    {
        // Unknown message received.
    }

    return "{}";
}