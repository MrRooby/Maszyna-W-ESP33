#include <FastLED.h>
#include "segment.h"

class ThreeSegmentDisplay{
    private:
        CFastLED strip;
        CRGB *leds;
        CRGB color;
        Segment display[3];

    public:

        ThreeSegmentDisplay(CFastLED fastLED, CRGB *leds, int startIndex, CRGB color);    

        void displayValue(int value);

        void changeColor(CRGB color);
};