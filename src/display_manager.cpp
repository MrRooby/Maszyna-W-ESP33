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
    this->a      = new ThreeDigitDisplay(this->stripR, this->ledsR, 0,       CRGB::Orange);
    this->wea    = new SignalLine(       this->stripR, this->ledsR, 21, 3,   CRGB::Orange);
    this->pao[0] = new PaODisplayLine(   this->stripR, this->ledsR, 24,      CRGB::Orange);
    
    this->czyt1  = new SignalLine(       this->stripR, this->ledsR, 73, 1,   CRGB::Orange);
    this->pisz   = new SignalLine(       this->stripR, this->ledsR, 74, 2,   CRGB::Orange);
    this->czyt2  = new SignalLine(       this->stripR, this->ledsR, 76, 1,   CRGB::Orange);
    
    this->pao[1] = new PaODisplayLine(   this->stripR, this->ledsR, 77,      CRGB::Orange);
    this->pao[2] = new PaODisplayLine(   this->stripR, this->ledsR, 126,     CRGB::Orange);
    this->pao[3] = new PaODisplayLine(   this->stripR, this->ledsR, 175,     CRGB::Orange);
    this->s      = new ThreeDigitDisplay(this->stripR, this->ledsR, 224,     CRGB::Orange);
    this->wes    = new SignalLine(       this->stripR, this->ledsR, 245, 9,  CRGB::Orange);
    this->wys    = new SignalLine(       this->stripR, this->ledsR, 254, 9,  CRGB::Orange);
    this->busA   = new BusLine(          this->stripR, this->ledsR, 263, 76, CRGB::Orange);
    
    for (size_t i = 0; i < 4; i++){
        if(this->pao[i] == nullptr){
            Serial.printf("Error: Failed to initialize pao[%d]!\n", i);
        }
    }
}


void DisplayManager::initStripL()
{
    this->ledsL = new CRGB[numLedsL];
    this->stripL.addLeds<WS2812B, LED_PORT_L, GRB>(ledsL, numLedsL);
    this->stripL.setBrightness(brightness);
    
    // Initialize strips displays and signal lines
    this->c      = new ThreeDigitDisplay(this->stripL, this->ledsL, 0,       CRGB::Orange);
    this->wel    = new SignalLine(       this->stripL, this->ledsL, 21, 3,   CRGB::Orange);
    this->wyl    = new SignalLine(       this->stripL, this->ledsL, 24, 3,   CRGB::Orange);
    this->il     = new SignalLine(       this->stripL, this->ledsL, 27, 3,   CRGB::Orange);
    this->wyad1  = new SignalLine(       this->stripL, this->ledsL, 30, 35,  CRGB::Orange);
    this->stop   = new SignalLine(       this->stripL, this->ledsL, 65, 8,   CRGB::Orange);
    this->wyad2  = new SignalLine(       this->stripL, this->ledsL, 73, 8,   CRGB::Orange);
    this->i      = new ThreeDigitDisplay(this->stripL, this->ledsL, 81,      CRGB::Orange);
    this->wei    = new SignalLine(       this->stripL, this->ledsL, 102, 3,  CRGB::Orange);
    this->weja   = new SignalLine(       this->stripL, this->ledsL, 105, 4,  CRGB::Orange);
    
    // Welcome to readibility hell
    this->przep1 = new SignalLine(       this->stripL, this->ledsL, 109, 1,  CRGB::Orange);
    this->ode1   = new SignalLine(       this->stripL, this->ledsL, 110, 1,  CRGB::Orange);
    this->dod1   = new SignalLine(       this->stripL, this->ledsL, 111, 1,  CRGB::Orange);
    this->weak   = new SignalLine(       this->stripL, this->ledsL, 112, 2,  CRGB::Orange);
    this->dod2   = new SignalLine(       this->stripL, this->ledsL, 114, 1,  CRGB::Orange);
    this->ode2   = new SignalLine(       this->stripL, this->ledsL, 115, 1,  CRGB::Orange);
    this->przep2 = new SignalLine(       this->stripL, this->ledsL, 116, 1,  CRGB::Orange);

    this->acc    = new ThreeDigitDisplay(this->stripL, this->ledsL, 117,     CRGB::Orange);
    this->wyak   = new SignalLine(       this->stripL, this->ledsL, 138, 33, CRGB::Orange);
    this->busS   = new BusLine(          this->stripL, this->ledsL, 171, 76, CRGB::Orange);
}


void DisplayManager::clearDisplay() {
    this->stripL.clear();
    this->stripR.clear();
}