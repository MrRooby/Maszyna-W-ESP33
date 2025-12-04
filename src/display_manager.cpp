#include "display_manager.h"


DisplayManager::DisplayManager(const char *signalLineColorHEX, 
                               const char *displayColorHEX, 
                               const char *busColorHEX) 
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
        {"przep",  this->przep},
        {"ode",    this->ode},
        {"dod",    this->dod},
        {"weak",   this->weak},
        {"wyak",   this->wyak},
        {"wea",    this->wea},
        {"czyt",   this->czyt},
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
    delete this->czyt;
    delete this->pisz;
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
    delete this->przep;
    delete this->ode;
    delete this->dod;
    delete this->weak;
    delete this->acc;
    delete this->wyak;
    delete this->busS;

    delete this->stripR;
    delete this->stripL;
}

void DisplayManager::initStripR() {
    // Initialize strips displays and signal lines
    this->wyak   = new SignalLine(       this->stripR, 0, 34,  this->signalLineColor);
    this->acc    = new ThreeDigitDisplay(this->stripR, 34,     this->displayColor);
    this->wea    = new SignalLine(       this->stripR, 55, 3,  this->signalLineColor);
    this->a      = new ThreeDigitDisplay(this->stripR, 58,     this->displayColor);
    this->pao[0] = new PaODisplayLine(   this->stripR, 79,     this->displayColor);
    this->pao[1] = new PaODisplayLine(   this->stripR, 128,    this->displayColor);
    this->pisz   = new SignalLine(       this->stripR, 177, 3, this->signalLineColor);
    this->czyt   = new SignalLine(       this->stripR, 180, 3, this->signalLineColor);
    this->pao[2] = new PaODisplayLine(   this->stripR, 184,    this->displayColor);
    this->pao[3] = new PaODisplayLine(   this->stripR, 233,    this->displayColor);
    this->s      = new ThreeDigitDisplay(this->stripR, 282,    this->displayColor);
    this->wys    = new SignalLine(       this->stripR, 303, 9, this->signalLineColor);
    this->wes    = new SignalLine(       this->stripR, 312, 9, this->signalLineColor);
    this->busS   = new BusLine(          this->stripR, 321, /*TODO liczba ledów magistrali*/78, this->busColor);
}

void DisplayManager::initStripL() {
    // Initialize strips displays and signal lines
    this->weja   = new SignalLine(       this->stripL, 0, 4,   this->signalLineColor);
    this->wei    = new SignalLine(       this->stripL, 4, 4,   this->signalLineColor);
    this->i      = new ThreeDigitDisplay(this->stripL, 8,      this->displayColor);
    this->przep  = new SignalLine(       this->stripL, 29, 3,  this->signalLineColor);
    this->ode    = new SignalLine(       this->stripL, 33, 3,  this->signalLineColor);
    this->dod    = new SignalLine(       this->stripL, 36, 3,  this->signalLineColor);
    this->weak   = new SignalLine(       this->stripL, 39, 3,  this->signalLineColor);
    this->wyad1  = new SignalLine(       this->stripL, 42, 8,  this->signalLineColor);
    this->stop   = new SignalLine(       this->stripL, 50, 16, this->signalLineColor);
    this->wyad2  = new SignalLine(       this->stripL, 65, 36, this->signalLineColor);
    this->wyl    = new SignalLine(       this->stripL, 101, 3, this->signalLineColor);
    this->wel    = new SignalLine(       this->stripL, 104, 3, this->signalLineColor);
    this->il     = new SignalLine(       this->stripL, 107, 3, this->signalLineColor);
    this->c      = new ThreeDigitDisplay(this->stripL, 110,    this->displayColor);
    this->busA   = new BusLine(          this->stripL, 131, /*TODO liczba ledów magistrali*/78, this->busColor);
}

RgbColor DisplayManager::hexToRgbColor(std::string colorHEX)
{
    if(colorHEX.size() != 7)
        return hexToRgbColor("#880011"); // if confused then pink
    
    std::string redHEX = colorHEX.substr(1, 2);
    std::string greenHEX = colorHEX.substr(3, 2);
    std::string blueHEX = colorHEX.substr(5, 2);

    int red = std::stoi(redHEX, nullptr, 16);
    int green = std::stoi(greenHEX, nullptr, 16);
    int blue = std::stoi(blueHEX, nullptr, 16);
   
    return RgbColor(red, green, blue);
}

void DisplayManager::setDisplayColor(std::string signalLineColorHEX, std::string displayColorHEX, std::string busColorHEX)
{
    if(signalLineColorHEX.size() > 0) { this->signalLineColor = hexToRgbColor(signalLineColorHEX); }
    if(displayColorHEX.size() > 0)    { this->displayColor = hexToRgbColor(displayColorHEX); }
    if(busColorHEX.size() > 0)        { this->busColor = hexToRgbColor(busColorHEX); }
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
}

void DisplayManager::refreshDisplay(){
    this->stripL->Show();
    this->stripR->Show();
}

void DisplayManager::changeDisplayColor(const char *signalLineColorHEX, const char *displayColorHEX, const char *busColorHEX)
{
    Serial.printf("[DisplayManager]: New Colors\n Signal Line = {%s}\n Display = {%s}\n Bus = {%s}\n", signalLineColorHEX, displayColorHEX, busColorHEX);   
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
    if (this->il)     this->il->setColor(this->signalLineColor);
    if (this->wel)    this->wel->setColor(this->signalLineColor);
    if (this->wyl)    this->wyl->setColor(this->signalLineColor);
    if (this->wyad1)  this->wyad1->setColor(this->signalLineColor);
    if (this->wyad2)  this->wyad2->setColor(this->signalLineColor);
    if (this->wei)    this->wei->setColor(this->signalLineColor);
    if (this->weja)   this->weja->setColor(this->signalLineColor);
    if (this->przep)  this->przep->setColor(this->signalLineColor);
    if (this->ode)    this->ode->setColor(this->signalLineColor);
    if (this->dod)    this->dod->setColor(this->signalLineColor);
    if (this->weak)   this->weak->setColor(this->signalLineColor);
    if (this->wyak)   this->wyak->setColor(this->signalLineColor);
    if (this->wea)    this->wea->setColor(this->signalLineColor); 
    if (this->czyt)   this->czyt->setColor(this->signalLineColor);
    if (this->pisz)   this->pisz->setColor(this->signalLineColor);
    if (this->wes)    this->wes->setColor(this->signalLineColor);
    if (this->wys)    this->wys->setColor(this->signalLineColor);
    if (this->stop)   this->stop->setColor(this->signalLineColor);
     
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

RgbColor DisplayManager::getElementColor(const DisplayElement element)
{
    switch (element){
        case DisplayElement::BUS_LINE:
            return this->busColor;
            break;
        
        case DisplayElement::DIGIT_DISPLAY:
            return this->displayColor;
            break;
        
        case DisplayElement::SIGNAL_LINE:
            return this->signalLineColor;
            break;

        default:
            break;
    }
}

void DisplayManager::blinkingAnimation(LedElement *display, const DisplayElement type)
{
    if(display == nullptr)
        return;
        
    unsigned long currentTime = millis();
    if (currentTime - lastBlinkTime >= BLINK_INTERVAL){
        blinkState = !blinkState;
        lastBlinkTime = currentTime;

        if (blinkState)        {
            display->setColor(getElementColor(type));
        }
        else{
            display->setColor(RgbColor(0, 0, 0));
        }
    }
}

void DisplayManager::ledTestAnimation(int updateSpeedMillis, bool printInSerial, bool reset)
{
    long now = millis();
    
    static int iL = 0;
    static int iR = 0;

    if(reset){
        iL = 0;
        iR = 0;
    }

    if(now - this->lastRefreshTime >= updateSpeedMillis){
        if(this->stripL){
            this->stripL->SetPixelColor(iL, RgbColor(255, 0, 0));
            this->stripL->SetPixelColor(iL - 1, RgbColor(0, 0, 0));
            if(printInSerial) Serial.printf("[DisplayManager]: LEFT [%i]\n", iL);
        }
        if(this->stripR){
            this->stripR->SetPixelColor(iR, RgbColor(255, 0, 0));
            this->stripR->SetPixelColor(iR - 1, RgbColor(0, 0, 0));
            if(printInSerial) Serial.printf("[DisplayManager]: RIGHT [%i]\n", iR);
        }

        if(iL < LED_COUNT_L){iL++;}
        else if(iL >= LED_COUNT_L) {iL = 0;}
        
        if(iR < LED_COUNT_R){iR++;}
        else if(iR >= LED_COUNT_R) {iR = 0;}

        lastRefreshTime = now;
    }
}

void DisplayManager::controlAllLEDs(int red, int green, int blue)
{
    RgbColor testColor(red, green, blue);

    if(this->stripL){
        for(int i = 0; i < LED_COUNT_L; i++){
            this->stripL->SetPixelColor(i, testColor);
        }
    }
    if(this->stripR){
        for(int i = 0; i < LED_COUNT_R; i++){
            this->stripR->SetPixelColor(i, testColor);
        }
    }
}

void DisplayManager::showIP(const IPAddress ip)
{
    this->pao[0]->addr->displayLetters('I', 'P');

    for(int i = 0; i < 4; i++){
        this->pao[i]->val->displayValue(ip[i], false);
    }
}
