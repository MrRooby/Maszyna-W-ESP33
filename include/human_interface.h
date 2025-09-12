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
    const unsigned int debounceDelayMilliseconds = 20;

    int lastEncButtonState = 0;
    int debouncedEncButtonState = 0;
    unsigned long lastEncButtonDebounceTime = 0;

    const unsigned int debounceEncMilliseconds = 15;
    unsigned long lastEncTime;
    int lastCLK = 0;
    
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

    bool getEncoderButtonState();

    EncoderState getEncoderState();

    void controlOnboardLED(OnboardLED led, bool choice);
    
    void controlBacklightLED(bool choice);
};