#include "pao_display_line.h"

PaODisplayLine::PaODisplayLine(NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod>* strip, int startIndex, RgbColor color)
    : LedElement(strip, startIndex, color){
        this->addr = new TwoDigitDisplay(this->strip, this->startIndex, this->color);
        this->val  = new ThreeDigitDisplay(this->strip, this->startIndex + 14, this->color);
        this->arg  = new TwoDigitDisplay(this->strip, this->startIndex + 35, this->color);
}


void PaODisplayLine::displayLine(int addr, int val, int arg)
{
    this->addr->displayValue(addr);
    this->val->displayValue(val);
    this->arg->displayValue(arg);
}


void PaODisplayLine::changeColor(RgbColor color)
{
    this->color = color;
    this->addr->changeColor(color);
    this->val->changeColor(color);
    this->arg->changeColor(color);
}
