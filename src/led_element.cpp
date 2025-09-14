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

void LedElement::setColor(RgbColor color) {
    this->color = color;
    
    for(int i = 0; i < this->pixelCount; i++) {
        if(this->strip0){
            this->strip0->SetPixelColor(this->startIndex + i, this->color);
        }
        else if(this->strip1){
            this->strip1->SetPixelColor(this->startIndex + i, this->color);
        }
    }
}