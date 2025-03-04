#include "leds.h"

Leds::Leds(CRGB color, byte brightness)
{
    fastLED.addLeds<WS2812B, LED_PORT, GRB>(leds, NUM_OF_LEDS);

    fastLED.setBrightness(brightness);
}

CRGB* Leds::getLeds()
{
    return leds;
}

CFastLED Leds::getFastLED()
{
    return this->fastLED;
}
