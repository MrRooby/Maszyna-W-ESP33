#include "led_element.h"

LedElement::LedElement(CFastLED strip, CRGB *leds, int startIndex, CRGB color) {
    this->strip = strip;
    this->leds = leds;
    this->color = color;
    this->startIndex = startIndex;
}


void LedElement::changeColor(CRGB color) {
    this->color = color;
}