
#include <FastLED.h>
#include "three_segment_display.h"
#include "two_segment_display.h"
#include "led_element.h"

class PaODisplayLine: protected LedElement{
    private:
        
        TwoSegmentDisplay *addr = nullptr;
        ThreeSegmentDisplay *val = nullptr;
        TwoSegmentDisplay *arg = nullptr;

    public:

        PaODisplayLine(CFastLED strip, CRGB *leds, int startIndex, CRGB color);    

        void displayLine(int addr, int val, int arg);

        void changeColor(CRGB color);
};