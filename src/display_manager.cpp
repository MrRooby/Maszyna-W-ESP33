#include "display_manager.h"

void DisplayManager::initStripR()
{
    this->ledsR = new CRGB[this->numLedsR];
    this->strip_R.addLeds<WS2812B, LED_PORT_R, GRB>(this->ledsR, this->numLedsR);
    this->strip_R.setBrightness(this->brightness);

    // Initialize strips displays and signal lines
    this->acc = new ThreeSegmentDisplay(this->strip_R, this->ledsR, 0, CRGB::Red);
    this->a = new ThreeSegmentDisplay(this->strip_R, this->ledsR, 21, CRGB::Blue);
}


void DisplayManager::initStripL()
{
    this->ledsL = new CRGB[numLedsL];
    this->strip_L.addLeds<WS2812B, LED_PORT_L, GRB>(ledsL, numLedsL);
    this->strip_L.setBrightness(brightness);

    // Initialize strips displays and signal lines
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

void DisplayManager::updateDisplay()
{
    // this->clearDisplay();
    this->strip_L.show();
    this->strip_R.show();
}

void DisplayManager::clearDisplay() {
    this->strip_L.clear();
    this->strip_R.clear();
}