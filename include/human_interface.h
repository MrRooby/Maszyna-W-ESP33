#pragma once

#include <Arduino.h>
#include "pins.h"
#include <map>


enum OnboardLED {
    TOP,
    BOTTOM
};

class HumanInterface
{
private:
    uint16_t lastButtonState = 0;
    uint16_t debouncedState = 0;
    unsigned long lastDebounceTime = 0;
    const unsigned long debounceDelayMilliseconds = 20;

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

    ~HumanInterface();

    char* getPressedButton();

    bool getWiFiSwitchState();

    void controlOnboardLED(OnboardLED led, bool choice);
    
    void controlBacklightLED(bool choice);
};