#include <FastLED.h>

#define NUM_OF_LEDS 7
#define LED_PORT 5

class Leds{
    private:
        byte brightness;

        CRGB leds[NUM_OF_LEDS];        
        CRGB color;

        CFastLED fastLED = FastLED;
        
    public:
        Leds(CRGB color, byte brightness);

        CRGB* getLeds();
        CFastLED getFastLED();
};