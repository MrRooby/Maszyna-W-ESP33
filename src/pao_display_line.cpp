#include "pao_display_line.h"

PaODisplayLine::PaODisplayLine(NeoPixelBus<NeoGrbFeature, NeoEsp32Rmt0Ws2812xMethod>* strip0, int startIndex, RgbColor color)
    : LedElement(strip0, startIndex, color){
        this->addr = new TwoDigitDisplay(this->strip0, this->startIndex, this->color);
        this->val  = new ThreeDigitDisplay(this->strip0, this->startIndex + 14, this->color);
        this->arg  = new TwoDigitDisplay(this->strip0, this->startIndex + 35, this->color);
}

PaODisplayLine::PaODisplayLine(NeoPixelBus<NeoGrbFeature, NeoEsp32Rmt1Ws2812xMethod>* strip1, int startIndex, RgbColor color)
    : LedElement(strip1, startIndex, color){
        this->addr = new TwoDigitDisplay(this->strip1, this->startIndex, this->color);
        this->val  = new ThreeDigitDisplay(this->strip1, this->startIndex + 14, this->color);
        this->arg  = new TwoDigitDisplay(this->strip1, this->startIndex + 35, this->color);
}


void PaODisplayLine::displayLine(int addr, int val, int arg)
{
    this->addr->displayValue(addr);
    this->val->displayValue(val);
    this->arg->displayValue(arg);
}

void PaODisplayLine::loadingAnimation(){
    this->addr->loadingAnimation();
    this->val->loadingAnimation();
    this->arg->loadingAnimation();
}


void PaODisplayLine::setColor(RgbColor color)
{
    this->color = color;
    this->addr->setColor(color);
    this->val->setColor(color);
    this->arg->setColor(color);
}
