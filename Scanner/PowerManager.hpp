// Information for this implemtation retrieved from:
// https://thekurks.net/blog/2018/1/24/guide-to-arduino-sleep-mode#anchor-link-2

#include <Adafruit_SleepyDog.h>

class PowerManager
{
public:
    static void WaitForNextIteration();
};

void PowerManager::WaitForNextIteration()
{
    Watchdog.sleep(8000);
}