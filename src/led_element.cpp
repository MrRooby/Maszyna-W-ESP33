#include "led_element.h"

LedElement::LedElement(NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> *strip, int startIndex, RgbColor color) {
    this->strip = strip;
    this->color = color;
    this->startIndex = startIndex;
}


void LedElement::changeColor(RgbColor color) {
    this->color = color;
    this->strip->SetPixelColor(this->startIndex, this->color);
    this->strip->Show();
}