#pragma once

// #include "leds.h"
#include "three_segment_display.h"

#define LED_PORT_R 5
#define LED_PORT_L 12

class DisplayManager {
private:
    CRGB *ledsR;
    CRGB *ledsL;

    CFastLED strip_R;
    CFastLED strip_L;

    int numLedsR;
    int numLedsL;

    int brightness = 60;

    void initStripR();
    void initStripL();

    
public:
    ThreeSegmentDisplay *acc = nullptr;    
    ThreeSegmentDisplay *a = nullptr;
    
    DisplayManager(int numLedsR, int numLedsL, int brightness);

    void updateDisplay();
    
    void clearDisplay();
};