#include "two_digit_display.h"

TwoDigitDisplay::TwoDigitDisplay(NeoPixelBus<NeoGrbFeature, NeoEsp32Rmt0Ws2812xMethod>* strip0, int startIndex, RgbColor color)
    : LedElement(strip0, startIndex, color)
{
    this->display[0] = Segment(this->strip0, this->startIndex, this->color);
    this->display[1] = Segment(this->strip0, this->startIndex + 7, this->color);
}

TwoDigitDisplay::TwoDigitDisplay(NeoPixelBus<NeoGrbFeature, NeoEsp32Rmt1Ws2812xMethod>* strip1, int startIndex, RgbColor color)
    : LedElement(strip1, startIndex, color)
{
    this->display[0] = Segment(this->strip1, this->startIndex, this->color);
    this->display[1] = Segment(this->strip1, this->startIndex + 7, this->color);
}

void TwoDigitDisplay::displayValue(int value)
{
    if(value < 0){
        value = 0;
    }
    else if (value > 99){
        value = 99;
    }

    int tens = (value / 10) % 10;
    int ones = value % 10;

    this->display[0].displayNumber(tens);
    this->display[1].displayNumber(ones);
}

void TwoDigitDisplay::loadingAnimation()
{
    this->display[0].loadingAnimation();
    this->display[1].loadingAnimation();
}

void TwoDigitDisplay::setColor(RgbColor color)
{
    this->color = color;
    
    this->display[0].setColor(color);
    this->display[1].setColor(color);
}
