#include "display_manager.h"


DisplayManager::DisplayManager(int numLedsR, int numLedsL, int brightness) {
    this->numLedsR = numLedsR;
    this->numLedsL = numLedsL;
    this->brightness = brightness;
    
    this->stripR = new NeoPixelBus<NeoGrbFeature, NeoEsp32Rmt0Ws2812xMethod>(numLedsR, LED_PORT_R);
    this->stripL = new NeoPixelBus<NeoGrbFeature, NeoEsp32Rmt1Ws2812xMethod>(numLedsL, LED_PORT_L);
    
    this->stripR->Begin();
    this->stripL->Begin();
    
    this->initStripR();
    this->initStripL();

    this->clearDisplay();
}


void DisplayManager::initStripR() {
    // Initialize strips displays and signal lines
    this->a      = new ThreeDigitDisplay(this->stripR, 0,       RgbColor(255, 128, 0));
    this->wea    = new SignalLine(       this->stripR, 21, 3,   RgbColor(255, 128, 0));
    this->pao[0] = new PaODisplayLine(   this->stripR, 24,      RgbColor(255, 128, 0));
    this->czyt1  = new SignalLine(       this->stripR, 73, 1,   RgbColor(255, 128, 0));
    this->pisz   = new SignalLine(       this->stripR, 74, 2,   RgbColor(255, 128, 0));
    this->czyt2  = new SignalLine(       this->stripR, 76, 1,   RgbColor(255, 128, 0));
    this->pao[1] = new PaODisplayLine(   this->stripR, 77,      RgbColor(255, 128, 0));
    this->pao[2] = new PaODisplayLine(   this->stripR, 126,     RgbColor(255, 128, 0));
    this->pao[3] = new PaODisplayLine(   this->stripR, 175,     RgbColor(255, 128, 0));
    this->s      = new ThreeDigitDisplay(this->stripR, 224,     RgbColor(255, 128, 0));
    this->wes    = new SignalLine(       this->stripR, 245, 9,  RgbColor(255, 128, 0));
    this->wys    = new SignalLine(       this->stripR, 254, 9,  RgbColor(255, 128, 0));
    this->busA   = new BusLine(          this->stripR, 263, 78, RgbColor(255, 128, 0));
}

void DisplayManager::initStripL() {
    // Initialize strips displays and signal lines
    this->c      = new ThreeDigitDisplay(this->stripL, 0,       RgbColor(255, 128, 0));
    this->wel    = new SignalLine(       this->stripL, 21, 3,   RgbColor(255, 128, 0));
    this->wyl    = new SignalLine(       this->stripL, 24, 3,   RgbColor(255, 128, 0));
    this->il     = new SignalLine(       this->stripL, 27, 3,   RgbColor(255, 128, 0));
    this->wyad1  = new SignalLine(       this->stripL, 30, 35,  RgbColor(255, 128, 0));
    this->stop   = new SignalLine(       this->stripL, 65, 8,   RgbColor(255, 128, 0));
    this->wyad2  = new SignalLine(       this->stripL, 73, 8,   RgbColor(255, 128, 0));
    this->i      = new ThreeDigitDisplay(this->stripL, 81,      RgbColor(255, 128, 0));
    this->wei    = new SignalLine(       this->stripL, 102, 3,  RgbColor(255, 128, 0));
    this->weja   = new SignalLine(       this->stripL, 105, 4,  RgbColor(255, 128, 0));
    
    this->przep1 = new SignalLine(       this->stripL, 109, 1,  RgbColor(255, 128, 0));
    this->ode1   = new SignalLine(       this->stripL, 110, 1,  RgbColor(255, 128, 0));
    this->dod1   = new SignalLine(       this->stripL, 111, 1,  RgbColor(255, 128, 0));
    this->weak   = new SignalLine(       this->stripL, 112, 2,  RgbColor(255, 128, 0));
    this->dod2   = new SignalLine(       this->stripL, 114, 1,  RgbColor(255, 128, 0));
    this->ode2   = new SignalLine(       this->stripL, 115, 1,  RgbColor(255, 128, 0));
    this->przep2 = new SignalLine(       this->stripL, 116, 1,  RgbColor(255, 128, 0));

    this->acc    = new ThreeDigitDisplay(this->stripL, 117,     RgbColor(255, 128, 0));
    this->wyak   = new SignalLine(       this->stripL, 138, 33, RgbColor(255, 128, 0));
    this->busS   = new BusLine(          this->stripL, 171, 78, RgbColor(255, 128, 0));
}


void DisplayManager::clearDisplay() {
    for (int i = 0; i < this->numLedsL; i++) {
        this->stripL->SetPixelColor(i, RgbColor(0, 0, 0));
    }
    for (int i = 0; i < this->numLedsR; i++) {
        this->stripR->SetPixelColor(i, RgbColor(0, 0, 0));
    }
    this->stripL->Show();
    this->stripR->Show();
}