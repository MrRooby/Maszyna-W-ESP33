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
    if(swappedRG){
        color = swapRG(color);
    }
    
    this->color = color;
}

RgbColor LedElement::swapRG(const RgbColor &color)
{
    return RgbColor(color.G, color.R, color.B);
}
