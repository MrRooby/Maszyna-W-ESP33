#include "two_digit_display.h"

TwoDigitDisplay::TwoDigitDisplay(NeoPixelBus<NeoGrbFeature, NeoEsp32Rmt0Ws2812xMethod>* strip0, int startIndex, RgbColor color)
    : LedElement(strip0, startIndex, color)
{
    this->pixelCount = 14;

    this->display[0] = Segment(strip0, startIndex, color);
    this->display[1] = Segment(strip0, startIndex + 7, color);
}

TwoDigitDisplay::TwoDigitDisplay(NeoPixelBus<NeoGrbFeature, NeoEsp32Rmt1Ws2812xMethod>* strip1, int startIndex, RgbColor color)
    : LedElement(strip1, startIndex, color)
{
    this->pixelCount = 14;

    this->display[0] = Segment(strip1, startIndex, color);
    this->display[1] = Segment(strip1, startIndex + 7, color);
}

void TwoDigitDisplay::displayValue(int value, bool enableLeadingZero)
{
    if(value < 0){
        value = 0;
    }
    else if (value > 99){
        value = 99;
    }

    int tens = (value / 10) % 10;
    int ones = value % 10;

    this->display[0].displayNumber(enableLeadingZero ? tens : 11);
    this->display[1].displayNumber(ones);
}

void TwoDigitDisplay::displayLetters(const char firstLetter, const char secondLetter)
{
    this->display[0].displayLetter(firstLetter);
    this->display[1].displayLetter(secondLetter);
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
