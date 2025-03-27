#include <FastLED.h>
#include "segment.h"
#include "led_element.h"

class ThreeSegmentDisplay : protected LedElement{
    private:
        Segment display[3];

    public:

        ThreeSegmentDisplay(CFastLED fastLED, CRGB *leds, int startIndex, CRGB color);    

        void displayValue(int value);

        void changeColor(CRGB color);
};