#include "display_manager.h"


DisplayManager::DisplayManager(int numLedsR, int numLedsL, int brightness)
{
    this->numLedsR = numLedsR;
    this->numLedsL = numLedsL;

    this->initStripL();
    this->initStripR();

    this->changeBrightness(brightness);

    this->clearDisplay();
}


void DisplayManager::initStripR()
{
    this->ledsR   = new CRGB[this->numLedsR];
    this->stripR.addLeds<WS2812B, LED_PORT_R, GRB>(this->ledsR, this->numLedsR);

    // Initialize strips displays and signal lines
    // this->wea    = new SignalLine(       this->stripR, this->ledsR, 0,                           CRGB::Orange);
<<<<<<< HEAD
    this->a      = new ThreeDigitDisplay(this->stripR, this->ledsR, 0,             CRGB::Orange);
    // this->pao[0] = new PaODisplayLine(   this->stripR, this->ledsR, /*start index*/,             CRGB::Orange);
    // this->pao[1] = new PaODisplayLine(   this->stripR, this->ledsR, /*start index*/,             CRGB::Orange);
    // this->czyt   = new SignalLine(       this->stripR, this->ledsR, /*start index*/, /*length*/, CRGB::Orange);
    // this->pao[2] = new PaODisplayLine(   this->stripR, this->ledsR, /*start index*/,             CRGB::Orange);
    // this->pisz   = new SignalLine(       this->stripR, this->ledsR, /*start index*/, /*length*/, CRGB::Orange);
    // this->pao[3] = new PaODisplayLine(   this->stripR, this->ledsR, /*start index*/,             CRGB::Orange);
    // this->wyak   = new SignalLine(       this->stripR, this->ledsR, /*start index*/, /*length*/, CRGB::Orange);
    // this->s      = new ThreeDigitDisplay(this->stripR, this->ledsR, /*start index*/,             CRGB::Orange);
=======
    this->a      = new ThreeDigitDisplay(this->stripR, this->ledsR, 0,   CRGB::Orange);
    this->pao[0] = new PaODisplayLine(   this->stripR, this->ledsR, 21,  CRGB::Orange);
    this->pao[1] = new PaODisplayLine(   this->stripR, this->ledsR, 70,  CRGB::Orange);
    // this->czyt   = new SignalLine(       this->stripR, this->ledsR, /*start index*/, /*length*/, CRGB::Orange);
    this->pao[2] = new PaODisplayLine(   this->stripR, this->ledsR, 119, CRGB::Orange);
    // this->pisz   = new SignalLine(       this->stripR, this->ledsR, /*start index*/, /*length*/, CRGB::Orange);
    this->pao[3] = new PaODisplayLine(   this->stripR, this->ledsR, 168, CRGB::Orange);
    // this->wyak   = new SignalLine(       this->stripR, this->ledsR, /*start index*/, /*length*/, CRGB::Orange);
    this->s      = new ThreeDigitDisplay(this->stripR, this->ledsR, 217, CRGB::Orange);

    for (size_t i = 0; i < 4; i++)
    {
        if(this->pao[i] == nullptr){
            Serial.printf("Error: Failed to initialize pao[%d]!\n", i);
        }
    }
    
>>>>>>> button-implementation
    // this->wes    = new SignalLine(       this->stripR, this->ledsR, /*start index*/, /*length*/, CRGB::Orange);
    // this->wys    = new SignalLine(       this->stripR, this->ledsR, /*start index*/, /*length*/, CRGB::Orange);
    // this->busS   = new BusLine(          this->stripR, this->ledsR, /*start index*/, /*length*/, CRGB::Orange);
}


void DisplayManager::initStripL()
{
    this->ledsL = new CRGB[numLedsL];
    this->stripL.addLeds<WS2812B, LED_PORT_L, GRB>(ledsL, numLedsL);

    // Initialize strips displays and signal lines
    // this->busA  = new BusLine(          this->stripL, this->ledsL, 0,               /*length*/, CRGB::Orange);
    // this->il    = new SignalLine(       this->stripL, this->ledsL, /*start index*/, /*length*/, CRGB::Orange);
    // this->wel   = new SignalLine(       this->stripL, this->ledsL, /*start index*/, /*length*/, CRGB::Orange);
    // this->wyl   = new SignalLine(       this->stripL, this->ledsL, /*start index*/, /*length*/, CRGB::Orange);
<<<<<<< HEAD
    // this->c     = new ThreeDigitDisplay(this->stripL, this->ledsL, /*start index*/,             CRGB::Orange);
    // this->wyad  = new SignalLine(       this->stripL, this->ledsL, /*start index*/, /*length*/, CRGB::Orange);
    // this->i     = new ThreeDigitDisplay(this->stripL, this->ledsL, /*start index*/,             CRGB::Orange);
=======
    this->c     = new ThreeDigitDisplay(this->stripL, this->ledsL, 0,             CRGB::Orange);
    // this->wyad  = new SignalLine(       this->stripL, this->ledsL, /*start index*/, /*length*/, CRGB::Orange);
    this->i     = new ThreeDigitDisplay(this->stripL, this->ledsL, 21,             CRGB::Orange);
>>>>>>> button-implementation
    // this->wei   = new SignalLine(       this->stripL, this->ledsL, /*start index*/, /*length*/, CRGB::Orange);
    // this->weja  = new SignalLine(       this->stripL, this->ledsL, /*start index*/, /*length*/, CRGB::Orange);
    // this->przep = new SignalLine(       this->stripL, this->ledsL, /*start index*/, /*length*/, CRGB::Orange);
    // this->ode   = new SignalLine(       this->stripL, this->ledsL, /*start index*/, /*length*/, CRGB::Orange);
    // this->dod   = new SignalLine(       this->stripL, this->ledsL, /*start index*/, /*length*/, CRGB::Orange);
    // this->weak  = new SignalLine(       this->stripL, this->ledsL, /*start index*/, /*length*/, CRGB::Orange);
<<<<<<< HEAD
    // this->acc   = new ThreeDigitDisplay(this->stripL, this->ledsL, 0,             CRGB::Orange);
=======
    this->acc   = new ThreeDigitDisplay(this->stripL, this->ledsL, 42,             CRGB::Orange);
>>>>>>> button-implementation
}


void DisplayManager::changeBrightness(int brightness)
{
    this->brightness = brightness;
    
    this->stripL.setBrightness(this->brightness);
    this->stripR.setBrightness(this->brightness);

    this->stripL.show();
    this->stripR.show();
}


void DisplayManager::clearDisplay()
{
    this->stripL.clear();
    this->stripR.clear();
}