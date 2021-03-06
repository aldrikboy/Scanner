#pragma once

#define PIN_LED_R 3
#define PIN_LED_G 9
#define PIN_LED_B 5

#define COLOR_RED 0xFF0000
#define COLOR_GREEN 0x00FF00
#define COLOR_BLUE 0x0000FF

#define COLOR_YELLOW (COLOR_RED | COLOR_GREEN)
#define COLOR_PURPLE (COLOR_RED | COLOR_BLUE)
#define COLOR_CYAN (COLOR_GREEN | COLOR_BLUE)
#define COLOR_WHITE (COLOR_RED | COLOR_GREEN | COLOR_BLUE)

class StatusManager
{
public:
    static void Setup();
    static void TurnOff();
    static void TurnOn();
    static void Report(long color);
    static void Show(long color, int repeat = 1);

private:
    static long currentColor;
};

long StatusManager::currentColor = 0xFFFFFF;

void StatusManager::Setup()
{
    pinMode(PIN_LED_R, OUTPUT);
    pinMode(PIN_LED_G, OUTPUT);
    pinMode(PIN_LED_B, OUTPUT);

    StatusManager::TurnOff();
}

void StatusManager::TurnOn()
{
    analogWrite(PIN_LED_R, (char)(StatusManager::currentColor >> 16));
    analogWrite(PIN_LED_G, (char)(StatusManager::currentColor >> 8));
    analogWrite(PIN_LED_B, (char)(StatusManager::currentColor));
}

void StatusManager::TurnOff()
{
    analogWrite(PIN_LED_R, 0);
    analogWrite(PIN_LED_G, 0);
    analogWrite(PIN_LED_B, 0);
}

void StatusManager::Report(long color)
{
    while (1)
    {
        Show(color, 1);
    }
}

void StatusManager::Show(long color, int repeat = 1)
{
    StatusManager::currentColor = color;
    for (int i = 0; i < repeat; i++)
    {
        StatusManager::TurnOn();
        delay(100);
        StatusManager::TurnOff();
        delay(100);
    }
}
