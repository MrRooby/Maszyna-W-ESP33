#include "three_digit_display.h"

ThreeDigitDisplay::ThreeDigitDisplay(NeoPixelBus<NeoGrbFeature, NeoEsp32Rmt0Ws2812xMethod> *strip0, int startIndex, RgbColor color)
    : LedElement(strip0, startIndex, color){
    this->pixelCount = 21;
    
    this->display[0] = Segment(strip0, startIndex, color);
    this->display[1] = Segment(strip0, startIndex + 7, color);
    this->display[2] = Segment(strip0, startIndex + 14, color);
}

ThreeDigitDisplay::ThreeDigitDisplay(NeoPixelBus<NeoGrbFeature, NeoEsp32Rmt1Ws2812xMethod> *strip1, int startIndex, RgbColor color)
    : LedElement(strip1, startIndex, color){
    this->pixelCount = 21;

    this->display[0] = Segment(strip1, startIndex, color);
    this->display[1] = Segment(strip1, startIndex + 7, color);
    this->display[2] = Segment(strip1, startIndex + 14, color);
}

void ThreeDigitDisplay::displayValue(int value, bool enableLeadingZero){
    if(value < 0)
        value = 0;
    else if (value > 999)
        value = 999;

    int huns = (value / 100) % 10;
    int tens = (value / 10) % 10;
    int ones = value % 10;

    this->display[0].displayNumber(enableLeadingZero ? huns : 11); // 11 is out of range for displayNumber, which turns off the segment
    if(!enableLeadingZero){
        if(huns == 0)
            tens == 11;
    }
    this->display[1].displayNumber(tens);
    this->display[2].displayNumber(ones);
}

void ThreeDigitDisplay::displayLetters(const char firstLetter, const char secondLetter, const char thirdLetter)
{
    this->display[0].displayLetter(firstLetter);
    this->display[1].displayLetter(secondLetter);
    this->display[2].displayLetter(thirdLetter);
}

void ThreeDigitDisplay::loadingAnimation(){
    this->display[0].loadingAnimation();
    this->display[1].loadingAnimation();
    this->display[2].loadingAnimation();
}

void ThreeDigitDisplay::setColor(RgbColor color){
    this->color = color;

    this->display[0].setColor(color);
    this->display[1].setColor(color);
    this->display[2].setColor(color);
}

