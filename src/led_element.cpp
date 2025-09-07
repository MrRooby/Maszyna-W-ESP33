#include "led_element.h"

LedElement::LedElement(NeoPixelBus<NeoGrbFeature, NeoEsp32Rmt0Ws2812xMethod> *strip0, int startIndex, RgbColor color) {
    this->strip0 = strip0;
    this->color = color;
    this->startIndex = startIndex;
}

LedElement::LedElement(NeoPixelBus<NeoGrbFeature, NeoEsp32Rmt1Ws2812xMethod> *strip1, int startIndex, RgbColor color) {
    this->strip1 = strip1;
    this->color = color;
    this->startIndex = startIndex;
}

void LedElement::changeColor(RgbColor color) {
    if(this->strip0 != nullptr && this->strip1 != nullptr){
        this->color = color;
        this->strip0->SetPixelColor(this->startIndex, this->color);
        this->strip0->Show();
        this->strip1->SetPixelColor(this->startIndex, this->color);
        this->strip1->Show();
    }
}