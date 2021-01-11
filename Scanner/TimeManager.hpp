#pragma once

#include <TimeLib.h>
#include <Wire.h>
#include "DS1307.h"
#include <RTClib.h>

RTC_DS1307 clock;
class TimeManager
{
public:
    static bool Setup();
    static time_t GetEpoch();
    static void SetTime();
};

bool TimeManager::Setup()
{
    if (!clock.begin())
    {
        //Serial.println("Couldn't find RTC");
        return false;
    }

    //SetTime();
    return true;
}

void TimeManager::SetTime()
{
    if (!clock.isrunning())
    {
        // this will adjust to the date and time stored in the RTC
        clock.adjust(DateTime(2021, 1, 11, 9, 12, 30));
    }
}

time_t TimeManager::GetEpoch()
{
    DateTime now = clock.now();
    return now.unixtime();
}