#include <FastLED.h>

#define NUM_OF_LEDS 7


class Segment
{
    private:
        int numOfFirstLED;

        CRGB segmentLeds[7];
        CRGB* leds;
        CRGB segmentColor = CRGB::Red;

        CFastLED fastLED;

    public:

        Segment(CRGB* leds, int numOfFirstLED, CRGB color);

        void displayNumber(int number);
};