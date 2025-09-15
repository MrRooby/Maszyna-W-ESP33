#pragma once

#include <Arduino.h>
#include "pins.h"
#include <map>


enum OnboardLED {
    TOP,
    BOTTOM
};

enum EncoderState {
    IDLE = -1,
    UP = 1,
    DOWN = 2,
};


class HumanInterface
{
private:
    uint16_t lastButtonState = 0;
    uint16_t debouncedState = 0;
    unsigned long lastDebounceTime = 0;
    const uint16_t DEBOUNCE_BUTTON_MILLIS = 20;

    bool lastEncButtonState = LOW;
    bool debouncedEncButtonState = LOW;
    unsigned long lastEncButtonDebounceTime = 0;

    const unsigned int DEBOUNCE_ENC_MILLIS = 15;
    unsigned long lastEncTime;
    int lastCLK = 0;
    const uint16_t LONG_PRESS_TIME = 500;
    unsigned long pressStartTime = 0;

    const int ONBOARD_LED_BRIGHTNESS = 64;
    
    const std::map<int, char*> buttons = {
        { 0,  "ODE" },
        { 1,  "WES" },
        { 2,  "TAKT" },
        { 3,  "WEJA" },
        { 4,  "WYAK" },
        { 5,  "PISZ" },
        { 6,  "CZYT" },
        { 7,  "WEA" },
        { 8,  "PRZEP" },
        { 9,  "IL" },
        { 10, "WEL" },
        { 11, "WYL" },
        { 12, "WYAD" },
        { 13, "WEI" },
        { 14, "WEAK" },
        { 15, "DOD" },
        { 16, "WYS" }
    };

    void setupMux();     
    void setupEncoder();     
    void setupSwitches();
    void setupOnboardLEDs();
    void setupBacklightLEDs();
         
public:
    HumanInterface();

    char* getPressedButton();

    bool WiFiEnabled();

    bool getEncoderButtonState();

    bool getEncoderButtonLongPress();

    EncoderState getEncoderState();

    void controlOnboardLED(OnboardLED led, bool choice);
    
    void controlBacklightLED(int power);
};