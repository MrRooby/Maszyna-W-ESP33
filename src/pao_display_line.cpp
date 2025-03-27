#include "pao_display_line.h"

PaODisplayLine::PaODisplayLine(CFastLED strip, CRGB *leds, int startIndex, CRGB color)
    : LedElement(strip, leds, startIndex, color){
        this->addr = new TwoSegmentDisplay(this->strip, this->leds, this->startIndex, this->color);
        this->val  = new ThreeSegmentDisplay(this->strip, this->leds, this->startIndex + 14, this->color);
        this->arg  = new TwoSegmentDisplay(this->strip, this->leds, this->startIndex + 35, this->color);
}


void PaODisplayLine::displayLine(int addr, int val, int arg)
{
    this->addr->displayValue(addr);
    this->val->displayValue(val);
    this->arg->displayValue(arg);
}


void PaODisplayLine::changeColor(CRGB color)
{
    this->color = color;
    this->addr->changeColor(color);
    this->val->changeColor(color);
    this->arg->changeColor(color);
}
