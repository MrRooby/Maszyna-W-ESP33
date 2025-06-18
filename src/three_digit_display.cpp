#include "three_digit_display.h"

ThreeDigitDisplay::ThreeDigitDisplay(NeoPixelBus<NeoGrbFeature, NeoEsp32Rmt0Ws2812xMethod> *strip0, int startIndex, RgbColor color)
    : LedElement(strip0, startIndex, color)
{
    this->display[0] = Segment(this->strip0, this->startIndex, this->color);
    this->display[1] = Segment(this->strip0, this->startIndex + 7, this->color);
    this->display[2] = Segment(this->strip0, this->startIndex + 14, this->color);
}

ThreeDigitDisplay::ThreeDigitDisplay(NeoPixelBus<NeoGrbFeature, NeoEsp32Rmt1Ws2812xMethod> *strip1, int startIndex, RgbColor color)
    : LedElement(strip1, startIndex, color)
{
    this->display[0] = Segment(this->strip1, this->startIndex, this->color);
    this->display[1] = Segment(this->strip1, this->startIndex + 7, this->color);
    this->display[2] = Segment(this->strip1, this->startIndex + 14, this->color);
}

void ThreeDigitDisplay::displayValue(int value)
{
    if(value < 0){
        value = 0;
    }
    else if (value > 999){
        value = 999;
    }

    int huns = (value / 100) % 10;
    int tens = (value / 10) % 10;
    int ones = value % 10;

    this->display[0].displayNumber(huns);
    this->display[1].displayNumber(tens);
    this->display[2].displayNumber(ones);
}


void ThreeDigitDisplay::changeColor(RgbColor color)
{
    this->color = color;

    this->display[0].changeColor(color);
    this->display[1].changeColor(color);
    this->display[2].changeColor(color);
}
