#pragma once

#include "leds.h"
#include "segment.h"

class DisplayManager {
private:
    Leds leds;
    CFastLED fastLED;
    
    Segment acc[3];
    Segment a[3];

    void initAcc(int index_start);
    
public:
    DisplayManager(int numOfLEDs, int brightness);

    void updateAccDisplay(int value);

    void clearDisplay();
};