#include "leds.h"

Leds::Leds(): numLeds(0), leds(nullptr) {}


Leds::~Leds()
{
    if (leds != nullptr) {
        delete[] leds;
    }
}


Leds::Leds(byte brightness, int numLeds)
{
    leds = new CRGB[numLeds];
    fastLED.addLeds<WS2812B, LED_PORT, GRB>(leds, numLeds);
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
