#include <LowPower.h>

class PowerManager
{
public:
    static void Sleep();
};

void PowerManager::Sleep()
{
    // Note here that USART0_ON means we will wake up when data is received via Bluetooth
    // So the device that is trying to communicate will never have to wait for the
    // full 8 seconds to get a response.
    LowPower.idle(SLEEP_8S, ADC_OFF, TIMER2_OFF, TIMER1_OFF, TIMER0_OFF, SPI_OFF, USART0_ON, TWI_OFF);
}