#include "display_manager.h"

void DisplayManager::initStripR()
{
    this->ledsR = new CRGB[this->numLedsR];
    this->strip_R.addLeds<WS2812B, LED_PORT_R, GRB>(this->ledsR, this->numLedsR);
    this->strip_R.setBrightness(this->brightness);

    // Initialize strips displays and signal lines
    // this->acc = new ThreeSegmentDisplay(this->strip_R, this->ledsR, 0, CRGB::Red);
    // this->a = new ThreeSegmentDisplay(this->strip_R, this->ledsR, 21, CRGB::Blue);
    this->wyak = new SignalLine(this->strip_R, this->ledsR, 0, 14, CRGB::Orange);
    this->busA = new BusLine(this->strip_R, this->ledsR, 15, 14, CRGB::Blue);
}


void DisplayManager::initStripL()
{
    this->ledsL = new CRGB[numLedsL];
    this->strip_L.addLeds<WS2812B, LED_PORT_L, GRB>(ledsL, numLedsL);
    this->strip_L.setBrightness(brightness);

    // Initialize strips displays and signal lines
    this->s = new ThreeSegmentDisplay(this->strip_L, this->ledsL, 0, CRGB::Orange);
}


DisplayManager::DisplayManager(int numLedsR, int numLedsL, int brightness)
{
    this->numLedsR = numLedsR;
    this->numLedsL = numLedsL;
    this->brightness = brightness;

    this->initStripL();
    this->initStripR();

    this->clearDisplay();
}


void DisplayManager::clearDisplay() {
    this->strip_L.clear();
    this->strip_R.clear();
}