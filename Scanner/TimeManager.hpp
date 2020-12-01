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
        Serial.println("Couldn't find RTC");
    }
}

void TimeManager::SetTime()
{
    // clock.begin();
    // clock.fillByYMD(2020, 11, 30);
    // clock.fillByHMS(11, 47, 50);
    // clock.fillDayOfWeek(MON);
    // clock.setTime();               //write time to the RTC chip
}

time_t TimeManager::GetEpoch()
{
    DateTime now = clock.now();
    return now.unixtime();
}