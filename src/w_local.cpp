#include "w_local.h"



const std::unordered_map<std::string, W_Local::CommandFunction> W_Local::signalMap = {
    {"IL",   &W_Local::il},
    {"WEL",  &W_Local::wel},
    {"WYL",  &W_Local::wyl},
    {"WYAD", &W_Local::wyad},
    {"WEI",  &W_Local::wei},
    {"WEAK", &W_Local::weak},
    {"DOD",  &W_Local::dod},
    {"ODE",  &W_Local::ode},
    {"PRZEP",&W_Local::przep},
    {"WYAK", &W_Local::wyak},
    {"WEJA", &W_Local::weja},
    {"WEA",  &W_Local::wea},
    {"CZYT", &W_Local::czyt},
    {"PISZ", &W_Local::pisz},
    {"WES",  &W_Local::wes},
    {"WYS",  &W_Local::wys}
};


const std::unordered_map<std::string, std::vector<std::string>> W_Local::signalConflicts = {
    {"CZYT",  {"PISZ"}},
    {"PISZ",  {"CZYT"}},

    {"WYAK",  {"WYS"}},
    {"WYS",   {"WYAK"}},
    
    {"IL",    {"WEL"}},
    {"WEL",   {"IL"}},
    
    {"DOD",   {"ODE", "PRZEP"}},
    {"ODE",   {"DOD", "PRZEP"}},
    {"PRZEP", {"DOD", "ODE"}},
    
    {"WYL",   {"WYAD"}},
    {"WYAD",  {"WYL"}}
};

W_Local::W_Local(DisplayManager *dispMan, HumanInterface *humInter)
{
    this->dispMan  = dispMan;
    this->humInter = humInter;

    initRegisters();
}

W_Local::~W_Local(){}

template<size_t N>
uint8_t W_Local::binaryTo_uint8_t(std::bitset<N> number)
{
    uint8_t result = 0;

    if(!number.none()){
        for(int i = 0; i < N; i++){
            result += number[i] * std::pow(2, i);
        }
    }

    return result;
}

uint8_t W_Local::getPaOAddr()
{
    return binaryTo_uint8_t(A);
}

void W_Local::initRegisters()
{
    this->A = _5Bit("00000");
    this->busA = _5Bit("00000");
    this->S = _8Bit("00000000");
    this->I = _8Bit("00000000");
    this->L = _5Bit("00000000");
    this->AK = _8Bit("00000000");
}

void W_Local::il()
{
    _5Bit one("00001");
    while(one != 0){
        _5Bit carry = L & one;
        L ^= one;
        one = carry << 1;
    }
}

void W_Local::wel()
{
    L = busA;
}

void W_Local::wyl()
{
    busA = L;
}

void W_Local::wyad()
{
    for(int i = 0; i < 5; i++){
        busA[i] = I[i];
    }
}

void W_Local::wei()
{
    I = busS;
}

void W_Local::weak()
{
    AK = JAML;
}

void W_Local::dod()
{
    AK |= JAML;
}

void W_Local::ode()
{
    _8Bit JAML_COPY = JAML;  
    while (JAML_COPY.any()){
        _8Bit borrow = ~AK & JAML_COPY; // NOT A AND B
        AK  = AK ^ JAML_COPY; // A XOR B
        JAML_COPY = borrow << 1; // borrow shift right by 1
    }
}

void W_Local::przep()
{
    AK = busS;
}

void W_Local::wyak()
{
    busS = AK;
}

void W_Local::weja()
{
    JAML = busS;
}

void W_Local::wea()
{
    A = busA;
}

void W_Local::czyt()
{
    uint8_t adres = binaryTo_uint8_t(A);
    
    S = PaO[adres];
}

void W_Local::pisz()
{
    uint8_t adres = binaryTo_uint8_t(A);
    
    PaO[adres] = S; 
}

void W_Local::wes()
{
    S = busS;
}

void W_Local::wys()
{
    busS = S;
}

void W_Local::takt()
{
    if(!this->nextLineSignals.empty()){
        // perform operations selected
        for (const auto& signal : this->nextLineSignals) {
            auto it = this->signalMap.find(signal);
            if (it != this->signalMap.end()) {
                (this->*(it->second))();
            }
        }
             
        // turn off all the signal lines after takt is executed
        for(auto &signal : this->signal){
            signal.second = false;
        }

        this->nextLineSignals.clear();
    }
}

void W_Local::refreshDisplay()
{
    if(this->dispMan){
        // Three digit displays
        if(this->dispMan->a)    this->dispMan->a->displayValue(binaryTo_uint8_t(A));
        if(this->dispMan->acc)  this->dispMan->acc->displayValue(binaryTo_uint8_t(AK));
        if(this->dispMan->c)    this->dispMan->c->displayValue(binaryTo_uint8_t(L));
        if(this->dispMan->i)    this->dispMan->i->displayValue(binaryTo_uint8_t(I));
        if(this->dispMan->s)    this->dispMan->s->displayValue(binaryTo_uint8_t(S));
        
        // Turn off all signal lines that are NOT in nextLineSignals
        for (const auto& sig : this->signal) {
            if (sig.first == "IL"     && this->dispMan->il)     this->dispMan->il->turnOnLine(sig.second);
            if (sig.first == "WEL"    && this->dispMan->wel)    this->dispMan->wel->turnOnLine(sig.second);
            if (sig.first == "WYL"    && this->dispMan->wyl)    this->dispMan->wyl->turnOnLine(sig.second);
            if (sig.first == "WYAD"   && this->dispMan->wyad1  && this->dispMan->wyad2){
                                                                    this->dispMan->wyad1->turnOnLine(sig.second); 
                                                                    this->dispMan->wyad2->turnOnLine(sig.second);}
            if (sig.first == "WEI"    && this->dispMan->wei)    this->dispMan->wei->turnOnLine(sig.second);
            if (sig.first == "WEJA"   && this->dispMan->weja)   this->dispMan->weja->turnOnLine(sig.second);
            if (sig.first == "PRZEP"  && this->dispMan->przep)  this->dispMan->przep->turnOnLine(sig.second); 
            if (sig.first == "ODE"    && this->dispMan->ode)    this->dispMan->ode->turnOnLine(sig.second);
            if (sig.first == "DOD"    && this->dispMan->dod)    this->dispMan->dod->turnOnLine(sig.second);  
            if (sig.first == "WEAK"   && this->dispMan->weak)   this->dispMan->weak->turnOnLine(sig.second);
            if (sig.first == "WYAK"   && this->dispMan->wyak)   this->dispMan->wyak->turnOnLine(sig.second);
            if (sig.first == "WEA"    && this->dispMan->wea)    this->dispMan->wea->turnOnLine(sig.second);
            if (sig.first == "CZYT"   && this->dispMan->czyt)   this->dispMan->czyt->turnOnLine(sig.second); 
            if (sig.first == "PISZ"   && this->dispMan->pisz)   this->dispMan->pisz->turnOnLine(sig.second);
            if (sig.first == "WES"    && this->dispMan->wes)    this->dispMan->wes->turnOnLine(sig.second);
            if (sig.first == "WYS"    && this->dispMan->wys)    this->dispMan->wys->turnOnLine(sig.second);
        }
    

        // TODO trzeba zrobić warunek włączenia się ledów stopu
        if(this->dispMan->stop)   this->dispMan->stop->turnOnLine(false);

        //PaO
        for(int i = 0; i <= 3; i++){
            if(this->dispMan->pao[i]){
                static RgbColor tempColor(0, 255, 0);
                
                if(binaryTo_uint8_t(A) == i + PaORangeLow){
                    this->dispMan->pao[i]->setTemporaryColor(tempColor);
                }
                else{
                    this->dispMan->pao[i]->restoreColor();
                }
                
                _3Bit arg;
                for(int n = 5; n < 8; n++){
                    arg[n] = PaO[i+PaORangeLow][n];   
                }
                this->dispMan->pao[i]->displayLine(i+PaORangeLow, binaryTo_uint8_t(PaO[i+PaORangeLow]), binaryTo_uint8_t(arg));
            }
        }

        //Bus lines
        this->refreshBUSLines();
        
        this->dispMan->refreshDisplay();
    }
}

void W_Local::refreshBUSLines()
{
    unsigned long now = millis();
    static unsigned long busATurnOnTime = 0;
    static unsigned long busSTurnOnTime = 0;
    
    if(this->busLED.at("A")){
        if(this->dispMan->busA) this->dispMan->busA->turnOnLine(true);
        busATurnOnTime = now; 
        this->busLED.at("A") = false;
    }
    if(this->busLED.at("S")){
        if(this->dispMan->busS) this->dispMan->busS->turnOnLine(true);
        busSTurnOnTime = now;
        this->busLED.at("S") = false;
    }

    if(now - busATurnOnTime >= BUS_LIGHT_UP_MILLIS)
    {
        if(this->dispMan->busA) this->dispMan->busA->turnOnLine(false);
    }
    
    if(now - busSTurnOnTime >= BUS_LIGHT_UP_MILLIS)
    {
        if(this->dispMan->busS) this->dispMan->busS->turnOnLine(false);
    }
}

void W_Local::readButtonInputs()
{
    char* button = this->humInter->getPressedButton();

    if(button != this->lastPressedButton) {
        if(button != nullptr){
            std::string buttonStr(button);

            Serial.println(button);

            if(buttonStr == "TAKT"){
                this->takt();
            }
            else {
                auto itSignal = std::find(this->nextLineSignals.begin(), this->nextLineSignals.end(), buttonStr);
                
                if(itSignal != this->nextLineSignals.end()){
                    this->nextLineSignals.erase(itSignal);
                    
                    auto it = this->signal.find(buttonStr);
                    if(it != this->signal.end()){
                        it->second = false;
                    }
                }
                else{
                    if(isSignalValid(buttonStr)) {
                        this->nextLineSignals.push_back(buttonStr);
                    }

                    auto it = this->signal.find(buttonStr);
                    if(it != this->signal.end()){
                        it->second = true;
                    }
                }
            }
        }

        this->lastPressedButton = button;
    }
}

template<size_t N>
void W_Local::insertMode(std::bitset<N> &selectedRegister)
{
    EncoderState enc = this->humInter->getEncoderState();
    uint8_t regVal = binaryTo_uint8_t(selectedRegister);
    uint8_t maxVal = pow(2, N);

    if(enc == UP){
        if(regVal < maxVal) {
            regVal++;
        }
        else {
            regVal = 0;
        }
    }
    else if(enc == DOWN){
        if(regVal > 0) {
            regVal--;
        }
        else {
            regVal = maxVal;
        } 
    }

    selectedRegister = std::bitset<N>(regVal);
}

void W_Local::scrollPaO()
{
    EncoderState enc = this->humInter->getEncoderState();

    if(enc != IDLE){
        if(enc == UP){
            if(PaORangeLow < (31 - 3)) {
                PaORangeLow++;
            }
            else {
                PaORangeLow  = 0;
            }
        }
        else if(enc == DOWN){
            if(PaORangeLow > 0) {
                PaORangeLow--;
            }
            else {
                PaORangeLow  = (31 - 3);
            }
        }
        Serial.printf("[W_LOCAL][DEBUG] PaORange %d<->%d\n", PaORangeLow, (PaORangeLow+3));
    }
}

ThreeDigitDisplay *W_Local::getSelectedDisplay(const Register selectedRegister)
{
    if (!dispMan) return nullptr;
    
    if (selectedRegister == regA)  return dispMan->a;
    if (selectedRegister == regAK) return dispMan->acc;
    if (selectedRegister == regL)  return dispMan->c;
    if (selectedRegister == regI)  return dispMan->i;
    if (selectedRegister == regS)  return dispMan->s;
    
    return nullptr;
}

void W_Local::handleEncoderMode()
{
    static size_t selectedValue = 0;
    ThreeDigitDisplay *display = getSelectedDisplay(static_cast<Register>(selectedValue));
    if (!display) return;

    bool currentState = humInter->getEncoderButtonState();
    unsigned long now = millis();
    static unsigned long pressStartTime = 0;
    const unsigned int LONG_PRESS_TIME = 500;
    static bool buttonStateLastFrame = HIGH;
    static bool insertModeToggled = false;
    static bool suppressNextReleaseClick = false;

    if (currentState == LOW) {
        if(pressStartTime == 0) {
            pressStartTime = now;
        }  
        else if((now - pressStartTime >= LONG_PRESS_TIME) && !insertModeToggled){
            insertModeEnabled = !insertModeEnabled;
            insertModeToggled = true;

            suppressNextReleaseClick = true;
            
            Serial.printf("[W_LOCAL][DEBUG] Insert mode toggled: {%d}\n", insertModeEnabled);
        }
    }
    else {
        pressStartTime = 0;
        if(insertModeToggled && !insertModeEnabled){
            display->setColor(dispMan->getElementColor(DisplayElement::DIGIT_DISPLAY));
        }
        insertModeToggled = false;
    }
     
    // warunek jest pomijany w takcie zmiany trybu, aby uniknąć zmiany wyświetlacza zaraz po puszczeniu przycisku
    if (insertModeEnabled) {
        if (suppressNextReleaseClick) {
            if (currentState == HIGH) {
                suppressNextReleaseClick = false;
                buttonStateLastFrame = HIGH; // prevents LOW->HIGH edge detection on this release
            }
        }
        else if (buttonStateLastFrame == LOW && currentState == HIGH) {
            if(selectedValue < 4)
                selectedValue++;
            else
                selectedValue = 0;

            // Change back color of the previously selected display
            display->setColor(dispMan->getElementColor(DisplayElement::DIGIT_DISPLAY));
            
            // Update display pointer for new selection
            display = getSelectedDisplay(static_cast<Register>(selectedValue));
            Serial.println("[W_LOCAL][DEBUG] Display changed");
        }
        
        buttonStateLastFrame = currentState;

        dispMan->blinkingAnimation(display, DisplayElement::DIGIT_DISPLAY);        
        
        // Dispatch: selecting appropriate std::bitset<N> at compile time
        switch (static_cast<Register>(selectedValue)) {
            case Register::regA:  insertMode(A);  break; // bitset<5>
            case Register::regI:  insertMode(I);  break; // bitset<5>
            case Register::regL:  insertMode(L);  break; // bitset<5>
            case Register::regAK: insertMode(AK); break; // bitset<8>
            case Register::regS:  insertMode(S);  break; // bitset<8>
            default: break;
        }
    }
    else {
        scrollPaO();
    }
}

void W_Local::printValuesToSerial()
{
    // static unsigned long lastPrintTime = 0;
    // unsigned long currentTime = millis();
    // uint16_t interval = 500;  // Changed from 50ms to 500ms for better readability

    // if (currentTime - lastPrintTime >= interval) {
    //     Serial.println("\n========== STAN MASZYNY ==========");
        
    //     // Lista rozkazów
    //     Serial.print("Lista rozkazow: ");
    //     if (nextLineSignals.empty()) {
    //         Serial.println("[pusta]");
    //     } else {
    //         for (const auto& sig : nextLineSignals) {
    //             Serial.print(sig.c_str());
    //             Serial.print(" ");
    //         }
    //         Serial.println();
    //     }

    //     // Wartości z wyświetlaczy
    //     Serial.print("Wartosci: ");
    //     for (const auto& val : values) {
    //         Serial.print(val.first.c_str());
    //         Serial.print("=");
    //         Serial.print(val.second);
    //         Serial.print(" ");
    //     }
    //     Serial.println();

    //     // Pamięć PAO
    //     Serial.println("Pamiec PAO:");
    //     for(int i = 0; i < 4; i++){
    //         Serial.print("  [");
    //         Serial.print(i);
    //         Serial.print("]: ");
    //         Serial.print(this->PaO[i].first);
    //         Serial.print(" | ");
    //         Serial.println(this->PaO[i].second);
    //     }

    //     Serial.println("==================================\n");
    //     lastPrintTime = currentTime;
    // }
}

bool W_Local::isSignalValid(const std::string &newSignal)
{
    auto conflictIt = signalConflicts.find(newSignal);
    if (conflictIt == signalConflicts.end()) {
        return true;
    }

    const auto& conflicts = conflictIt->second;
    
    for (const auto& existingSignal : this->nextLineSignals) {
        for (const auto& conflictSignal : conflicts) {
            if (existingSignal == conflictSignal) {
                Serial.printf("[W_LOCAL][DEBUG]: Signal '%s' conflicts with '%s'\n", 
                            newSignal.c_str(), conflictSignal.c_str());
                return false;
            }
        }
    }
    
    return true;
}

void W_Local::runLocal()
{
    readButtonInputs();

    handleEncoderMode();
    
    refreshDisplay();
    
    // printValuesToSerial();
}
