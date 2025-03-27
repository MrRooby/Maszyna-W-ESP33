#include "display_manager.h"


DisplayManager::DisplayManager(int numLedsR, int numLedsL, int brightness)
{
    this->numLedsR = numLedsR;
    this->numLedsL = numLedsL;
    this->brightness = brightness;

    this->initStripL();
    this->initStripR();

    this->clearDisplay();
}


void DisplayManager::initStripR()
{
    this->ledsR   = new CRGB[this->numLedsR];
    this->stripR.addLeds<WS2812B, LED_PORT_R, GRB>(this->ledsR, this->numLedsR);
    this->stripR.setBrightness(this->brightness);

    // Initialize strips displays and signal lines
    this->acc    = new ThreeDigitDisplay(this->stripR, this->ledsR, 0,                           CRGB::Orange);
    // this->a      = new ThreeDigitDisplay(this->stripR, this->ledsR, 21,                          CRGB::Orange);
    // this->pao[0] = new PaODisplayLine(     this->stripR, this->ledsR, 42,                          CRGB::Orange);
    // this->pao[1] = new PaODisplayLine(     this->stripR, this->ledsR, 91,                          CRGB::Orange);
    // this->czyt   = new SignalLine(         this->stripR, this->ledsR, 140,             /*length*/, CRGB::Orange);
    // this->pao[2] = new PaODisplayLine(     this->stripR, this->ledsR, /*start index*/,             CRGB::Orange);
    // this->pisz   = new SignalLine(         this->stripR, this->ledsR, /*start index*/, /*length*/, CRGB::Orange);
    // this->pao[3] = new PaODisplayLine(     this->stripR, this->ledsR, /*start index*/,             CRGB::Orange);
    // this->wyak   = new SignalLine(         this->stripR, this->ledsR, /*start index*/, /*length*/, CRGB::Orange);
    // this->s      = new ThreeDigitDisplay(this->stripR, this->ledsR, /*start index*/,             CRGB::Orange);
    // this->wes    = new SignalLine(         this->stripR, this->ledsR, /*start index*/, /*length*/, CRGB::Orange);
    // this->wys    = new SignalLine(         this->stripR, this->ledsR, /*start index*/, /*length*/, CRGB::Orange);
    // this->busS   = new BusLine(            this->stripR, this->ledsR, /*start index*/, /*length*/, CRGB::Orange);
}


void DisplayManager::initStripL()
{
    this->ledsL = new CRGB[numLedsL];
    this->stripL.addLeds<WS2812B, LED_PORT_L, GRB>(ledsL, numLedsL);
    this->stripL.setBrightness(brightness);

    // Initialize strips displays and signal lines
    // this->busA  = new BusLine(            this->stripL, this->ledsL, 0,               /*length*/, CRGB::Orange);
    // this->icc   = new SignalLine(         this->stripL, this->ledsL, /*start index*/  /*length*/, CRGB::Orange);
    // this->wec   = new SignalLine(         this->stripL, this->ledsL, /*start index*/, /*length*/, CRGB::Orange);
    // this->wyc   = new SignalLine(         this->stripL, this->ledsL, /*start index*/, /*length*/, CRGB::Orange);
    this->c     = new ThreeDigitDisplay(this->stripL, this->ledsL, 0/*start index*/,             CRGB::Orange);
    // this->wyad  = new SignalLine(         this->stripL, this->ledsL, /*start index*/, /*length*/, CRGB::Orange);
    // this->i     = new ThreeDigitDisplay(this->stripL, this->ledsL, /*start index*/,             CRGB::Orange);
    // this->wei   = new SignalLine(         this->stripL, this->ledsL, /*start index*/, /*length*/, CRGB::Orange);
    // this->weja  = new SignalLine(         this->stripL, this->ledsL, /*start index*/, /*length*/, CRGB::Orange);
    // this->przep = new SignalLine(         this->stripL, this->ledsL, /*start index*/, /*length*/, CRGB::Orange);
    // this->dak   = new SignalLine(         this->stripL, this->ledsL, /*start index*/, /*length*/, CRGB::Orange);
    // this->iak   = new SignalLine(         this->stripL, this->ledsL, /*start index*/, /*length*/, CRGB::Orange);
    // this->weak  = new SignalLine(         this->stripL, this->ledsL, /*start index*/, /*length*/, CRGB::Orange);
    // this->acc   = new ThreeDigitDisplay(this->stripL, this->ledsL, /*start index*/,             CRGB::Orange);
}


void DisplayManager::clearDisplay() {
    this->stripL.clear();
    this->stripR.clear();
}