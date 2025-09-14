#include "display_manager.h"


DisplayManager::DisplayManager(std::string signalLineColorHEX, 
                               std::string displayColorHEX, 
                               std::string busColorHEX) 
{
    this->setDisplayColor(signalLineColorHEX, displayColorHEX, busColorHEX);

    this->stripR = new NeoPixelBus<NeoGrbFeature, NeoEsp32Rmt0Ws2812xMethod>(LED_COUNT_R, LED_PORT_R);
    this->stripL = new NeoPixelBus<NeoGrbFeature, NeoEsp32Rmt1Ws2812xMethod>(LED_COUNT_L, LED_PORT_L);
    
    this->stripR->Begin();
    this->stripL->Begin();
    
    this->initStripR();
    this->initStripL();

    this->signalLineMap = {
        {"il",     this->il},
        {"wel",    this->wel},
        {"wyl",    this->wyl},
        {"wyad1",  this->wyad1},
        {"wyad2",  this->wyad2},
        {"wei",    this->wei},
        {"weja",   this->weja},
        {"przep1", this->przep1},
        {"przep2", this->przep2},
        {"ode1",   this->ode1},
        {"ode2",   this->ode2},
        {"dod1",   this->dod1},
        {"dod2",   this->dod2},
        {"weak",   this->weak},
        {"wyak",   this->wyak},
        {"wea",    this->wea},
        {"czyt1",  this->czyt1},
        {"czyt2",  this->czyt2},
        {"pisz",   this->pisz},
        {"wes",    this->wes},
        {"wys",    this->wys},
        {"stop",   this->stop}
    };

    this->clearDisplay();
}

DisplayManager::~DisplayManager()
{
    delete this->a;
    delete this->wea;
    delete this->pao[0];
    delete this->czyt1;
    delete this->pisz;
    delete this->czyt2;
    delete this->pao[1];
    delete this->pao[2];
    delete this->pao[3];
    delete this->s;
    delete this->wes;
    delete this->wys;
    delete this->busA;

    delete this->c;
    delete this->wel;
    delete this->wyl;
    delete this->il;
    delete this->wyad1;
    delete this->stop;
    delete this->wyad2;
    delete this->i;
    delete this->wei;
    delete this->weja;
    delete this->przep1;
    delete this->ode1;
    delete this->dod1;
    delete this->weak;
    delete this->dod2;
    delete this->ode2;
    delete this->przep2;
    delete this->acc;
    delete this->wyak;
    delete this->busS;

    delete this->stripR;
    delete this->stripL;
}

void DisplayManager::initStripR() {
    // Initialize strips displays and signal lines
    this->a      = new ThreeDigitDisplay(this->stripR, 0,       this->displayColor);
    this->wea    = new SignalLine(       this->stripR, 21, 3,   this->displayColor);
    this->pao[0] = new PaODisplayLine(   this->stripR, 24,      this->displayColor);
    this->czyt1  = new SignalLine(       this->stripR, 73, 1,   this->signalLineColor);
    this->pisz   = new SignalLine(       this->stripR, 74, 2,   this->signalLineColor);
    this->czyt2  = new SignalLine(       this->stripR, 76, 1,   this->signalLineColor);
    this->pao[1] = new PaODisplayLine(   this->stripR, 77,      this->displayColor);
    this->pao[2] = new PaODisplayLine(   this->stripR, 126,     this->displayColor);
    this->pao[3] = new PaODisplayLine(   this->stripR, 175,     this->displayColor);
    this->s      = new ThreeDigitDisplay(this->stripR, 224,     this->displayColor);
    this->wes    = new SignalLine(       this->stripR, 245, 9,  this->signalLineColor);
    this->wys    = new SignalLine(       this->stripR, 254, 9,  this->signalLineColor);
    this->busA   = new BusLine(          this->stripR, 263, 78, this->busColor);
}

void DisplayManager::initStripL() {
    // Initialize strips displays and signal lines
    this->c      = new ThreeDigitDisplay(this->stripL, 0,       this->displayColor);
    this->wel    = new SignalLine(       this->stripL, 21, 3,   this->signalLineColor);
    this->wyl    = new SignalLine(       this->stripL, 24, 3,   this->signalLineColor);
    this->il     = new SignalLine(       this->stripL, 27, 3,   this->signalLineColor);
    this->wyad1  = new SignalLine(       this->stripL, 30, 35,  this->signalLineColor);
    this->stop   = new SignalLine(       this->stripL, 65, 8,   this->signalLineColor);
    this->wyad2  = new SignalLine(       this->stripL, 73, 8,   this->signalLineColor);
    this->i      = new ThreeDigitDisplay(this->stripL, 81,      this->displayColor);
    this->wei    = new SignalLine(       this->stripL, 102, 3,  this->signalLineColor);
    this->weja   = new SignalLine(       this->stripL, 105, 4,  this->signalLineColor);
    
    this->przep1 = new SignalLine(       this->stripL, 109, 1,  this->signalLineColor);
    this->ode1   = new SignalLine(       this->stripL, 110, 1,  this->signalLineColor);
    this->dod1   = new SignalLine(       this->stripL, 111, 1,  this->signalLineColor);
    this->weak   = new SignalLine(       this->stripL, 112, 2,  this->signalLineColor);
    this->dod2   = new SignalLine(       this->stripL, 114, 1,  this->signalLineColor);
    this->ode2   = new SignalLine(       this->stripL, 115, 1,  this->signalLineColor);
    this->przep2 = new SignalLine(       this->stripL, 116, 1,  this->signalLineColor);

    this->acc    = new ThreeDigitDisplay(this->stripL, 117,     this->displayColor);
    this->wyak   = new SignalLine(       this->stripL, 138, 33, this->signalLineColor);
    this->busS   = new BusLine(          this->stripL, 171, 78, this->busColor);
}

RgbColor DisplayManager::hexToRgbColor(std::string colorHEX)
{
    if(colorHEX.size() != 6)
        return hexToRgbColor("880011"); // if confused then pink
    
    std::string redHEX = colorHEX.substr(0, 2);
    std::string greenHEX = colorHEX.substr(2, 2);
    std::string blueHEX = colorHEX.substr(4, 2);

    int red = std::stoi(redHEX, nullptr, 16);
    int green = std::stoi(greenHEX, nullptr, 16);
    int blue = std::stoi(blueHEX, nullptr, 16);
   
    return RgbColor(red, green, blue);
}

void DisplayManager::setDisplayColor(std::string signalLineColorHEX, std::string displayColorHEX, std::string busColorHEX)
{
    this->signalLineColor = hexToRgbColor(signalLineColorHEX);
    this->displayColor = hexToRgbColor(displayColorHEX);
    this->busColor = hexToRgbColor(busColorHEX);
}

void DisplayManager::loadingAnimation(){
    unsigned long now = millis();

    if(now - this->lastUpdate >= this->timeBetweenAnimationFramesMilliseconds){
        if (this->a) this->a->loadingAnimation();
        if (this->s) this->s->loadingAnimation();
        if (this->c) this->c->loadingAnimation();
        if (this->i) this->i->loadingAnimation();
        if (this->acc) this->acc->loadingAnimation();
    
        for (int i = 0; i < 4; ++i) {
            if (this->pao[i]) this->pao[i]->loadingAnimation();
        }
    
        this->lastUpdate = now; 
    }
}

void DisplayManager::clearDisplay() {
    for (int i = 0; i < LED_COUNT_L; i++) {
        this->stripL->SetPixelColor(i, RgbColor(0, 0, 0));
    }
    for (int i = 0; i < LED_COUNT_R; i++) {
        this->stripR->SetPixelColor(i, RgbColor(0, 0, 0));
    }
    
    // this->refreshDisplay();
}

void DisplayManager::refreshDisplay(){
    this->stripL->Show();
    this->stripR->Show();
}

void DisplayManager::changeDisplayColor(std::string signalLineColorHEX, std::string displayColorHEX, std::string busColorHEX)
{
    this->setDisplayColor(signalLineColorHEX, displayColorHEX, busColorHEX);
    
    // Update all display colors
    if (this->a) this->a->setColor(this->displayColor);
    if (this->s) this->s->setColor(this->displayColor);
    if (this->c) this->c->setColor(this->displayColor);
    if (this->i) this->i->setColor(this->displayColor);
    if (this->acc) this->acc->setColor(this->displayColor);
    
    for (int i = 0; i < 4; ++i) {
        if (this->pao[i]) this->pao[i]->setColor(this->displayColor);
    }
    
    // Update signal line colors
    if (this->il) this->il->setColor(this->signalLineColor);
    if (this->wel) this->wel->setColor(this->signalLineColor);
    if (this->wyl) this->wyl->setColor(this->signalLineColor);
    if (this->wyad1) this->wyad1->setColor(this->signalLineColor);
    if (this->wyad2) this->wyad2->setColor(this->signalLineColor);
    if (this->wei) this->wei->setColor(this->signalLineColor);
    if (this->weja) this->weja->setColor(this->signalLineColor);
    if (this->przep1) this->przep1->setColor(this->signalLineColor);
    if (this->przep2) this->przep2->setColor(this->signalLineColor);
    if (this->ode1) this->ode1->setColor(this->signalLineColor);
    if (this->ode2) this->ode2->setColor(this->signalLineColor);
    if (this->dod1) this->dod1->setColor(this->signalLineColor);
    if (this->dod2) this->dod2->setColor(this->signalLineColor);
    if (this->weak) this->weak->setColor(this->signalLineColor);
    if (this->wyak) this->wyak->setColor(this->signalLineColor);
    if (this->wea) this->wea->setColor(this->signalLineColor); 
    if (this->czyt1) this->czyt1->setColor(this->signalLineColor);
    if (this->czyt2) this->czyt2->setColor(this->signalLineColor);
    if (this->pisz) this->pisz->setColor(this->signalLineColor);
    if (this->wes) this->wes->setColor(this->signalLineColor);
    if (this->wys) this->wys->setColor(this->signalLineColor);
    if (this->stop) this->stop->setColor(this->signalLineColor);
    
    // Update bus colors
    if (this->busA) this->busA->setColor(this->busColor);
    if (this->busS) this->busS->setColor(this->busColor);
}

SignalLine* DisplayManager::getSignalLine(const std::string& signalName) {
    auto it = signalLineMap.find(signalName);
    return (it != signalLineMap.end()) ? it->second : nullptr;
}

void DisplayManager::setSignalLineState(const std::string& signalName, bool state) {
    SignalLine* signalLine = getSignalLine(signalName);
    if (signalLine) {
        signalLine->turnOnLine(state);
    }
}