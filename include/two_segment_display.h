
#include <FastLED.h>
#include "segment.h"
#include "led_element.h"

class TwoSegmentDisplay : protected LedElement{
    private:
        Segment display[];

    public:

        TwoSegmentDisplay(CFastLED fastLED, CRGB *leds, int startIndex, CRGB color);    

        void displayValue(int value);

        void changeColor(CRGB color);
};