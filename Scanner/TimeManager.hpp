#pragma once

#include <TimeLib.h>
#include <Wire.h>
#include "DS1307.h"
#include <RTClib.h>

RTC_DS1307 clock;
class TimeManager
{
public:
    static void Setup();
    static time_t GetEpoch();
    static void SetTime();
};

void TimeManager::Setup()
{
    if (!clock.begin())
    {
        // return false here
        //Serial.println("Couldn't find RTC");
    }

    //SetTime();
}

void TimeManager::SetTime()
{
    if (!clock.isrunning())
    {
        // this will adjust to the date and time stored in the RTC
        clock.adjust(DateTime(2020, 12, 4, 9, 27, 30));
    }
}

time_t TimeManager::GetEpoch()
{
    DateTime now = clock.now();
    return now.unixtime();
}