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
}

W_Local::~W_Local(){}

void W_Local::baseSignal(uint16_t from, uint16_t &to)
{
    if(from > 999){
        from = 999;
    } 
    if(from < 0){
        from = 0;
    }

    to = from;
}

void W_Local::il()
{
    if(this->values.at("L") < 999){
        this->values.at("L")++;
    }
}

void W_Local::wel()
{
    this->baseSignal(this->bus.at("A"), this->values.at("L"));
}

void W_Local::wyl()
{
    this->baseSignal(this->values.at("L"), this->bus.at("A"));
}

void W_Local::wyad()
{
    this->baseSignal(this->values.at("I"), this->bus.at("A"));
}

void W_Local::wei()
{
    this->baseSignal(this->bus.at("S"), this->values.at("I"));
}

void W_Local::weak()
{
    this->baseSignal(this->values.at("JAML"), this->values.at("AK"));
}

void W_Local::dod()
{
    uint16_t outcome = this->values.at("JAML") + this->values.at("AK");
    this->baseSignal(outcome, this->values.at("JAML"));
}

void W_Local::ode()
{
    uint16_t outcome = this->values.at("JAML") - this->values.at("AK");
    this->baseSignal(outcome, this->values.at("JAML"));
}

void W_Local::przep()
{
    this->baseSignal(this->values.at("JAML"), this->values.at("AK"));
}

void W_Local::wyak()
{
    this->baseSignal(this->values.at("AK"), this->values.at("S"));
}

void W_Local::weja()
{
    this->baseSignal(this->bus.at("S"), this->values.at("JAML"));
}

void W_Local::wea()
{
    int value = this->bus.at("A");

    if(value >= 63){
        value = 63;
    }
    if(value <= 0){
        value = 0;
    }

    this->values.at("A") = this->bus.at("A");
}

void W_Local::czyt()
{
    this->baseSignal(this->PaO[this->values.at("A")].first, this->values.at("S"));
}

void W_Local::pisz()
{
    this->baseSignal(this->values.at("S"), this->PaO[this->values.at("A")].first);
}

void W_Local::wes()
{
    this->baseSignal(this->values.at("S"), this->bus.at("S"));
}

void W_Local::wys()
{
    this->baseSignal(this->values.at("S"), this->bus.at("S"));
}

void W_Local::takt()
{
    if(!this->nextLineSignals.empty()){
        // perform operations selected
        for (const auto& signal : this->nextLineSignals) {
            auto it = signalMap.find(signal);
            if (it != signalMap.end()) {
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
        if(this->dispMan->a)    this->dispMan->a->displayValue(this->values.at("A"));
        if(this->dispMan->acc)  this->dispMan->acc->displayValue(this->values.at("AK"));
        if(this->dispMan->c)    this->dispMan->c->displayValue(this->values.at("L"));
        if(this->dispMan->i)    this->dispMan->i->displayValue(this->values.at("I"));
        if(this->dispMan->s)    this->dispMan->s->displayValue(this->values.at("S"));
        
        //Signal lines
        for (const auto& sig : this->signal) {
            if (sig.first == "IL"     && this->dispMan->il)     this->dispMan->il->turnOnLine(sig.second);
            if (sig.first == "WEL"    && this->dispMan->wel)    this->dispMan->wel->turnOnLine(sig.second);
            if (sig.first == "WYL"    && this->dispMan->wyl)    this->dispMan->wyl->turnOnLine(sig.second);
            if (sig.first == "WYAD"   && this->dispMan->wyad1  && this->dispMan->wyad1){
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
        for(int i = 0; i < 4; i++){
            if(this->dispMan->pao[i]){
                this->dispMan->pao[i]->displayLine(i, this->PaO[i].first, this->PaO[i].second);
            }
        }

        //Bus lines
        if(this->dispMan->busA) this->dispMan->busA->turnOnLine(this->bus.at("A"));
        if(this->dispMan->busS) this->dispMan->busS->turnOnLine(this->bus.at("S"));
    }

    this->dispMan->refreshDisplay();
}

void W_Local::readButtonInputs()
{
    char* button = this->humInter->getPressedButton();

    if(button == nullptr) {
        this->lastPressedButton = "";
        return;
    }

    std::string buttonStr(button);

    if(this->lastPressedButton != button){
        if(buttonStr == "TAKT"){
            this->takt();
        }
        else{
            auto itSignal = std::find(this->nextLineSignals.begin(), this->nextLineSignals.end(), buttonStr);
            if(itSignal != this->nextLineSignals.end()) {
                this->nextLineSignals.erase(itSignal);
            } else {
                if(isSignalValid(buttonStr)) {
                    this->nextLineSignals.push_back(buttonStr);
                }
            }
            
            auto it = this->signal.find(buttonStr);
            if(it != this->signal.end()) {
                it->second = !it->second;
            }
        }
        this->lastPressedButton = button;
    }
}

void W_Local::insertMode(uint16_t &value)
{
    EncoderState enc = this->humInter->getEncoderState();

    if(enc == UP){
        if(value < 999) {
            value++;
        }
        else {
            value = 0;
        }
    }
    else if(enc == DOWN){
        if(value > 0) {
            value--;
        }
        else {
            value = 999;
        } 
    }
}

ThreeDigitDisplay *W_Local::getSelectedDisplay()
{
    if (!dispMan) return nullptr;
    
    if (selectedValue == "A")  return dispMan->a;
    if (selectedValue == "AK") return dispMan->acc;
    if (selectedValue == "L")  return dispMan->c;
    if (selectedValue == "I")  return dispMan->i;
    if (selectedValue == "S")  return dispMan->s;
    
    return nullptr;
}

void W_Local::handleInsertMode()
{
    ThreeDigitDisplay *display = getSelectedDisplay();
    if (!display) return;

    bool currentState = humInter->getEncoderButtonState();
    unsigned long now = millis();
    static unsigned long pressStartTime = 0;
    const unsigned int LONG_PRESS_TIME = 500;
    static bool buttonPressedLastFrame = false;

    if (currentState) {
        if( pressStartTime == 0) {
            pressStartTime = now;
        }  
        else if(now - pressStartTime >= LONG_PRESS_TIME){
            insertModeEnabled = !insertModeEnabled;
        }
    }
    else {
        pressStartTime = 0;
    }
    
    if (insertModeEnabled) { 
        if (buttonPressedLastFrame && !currentState) {
            // Get iterator to current selection
            auto it = values.find(selectedValue);

            // Move to next item, or wrap to beginning
            ++it;
            if (it == values.end()) {
                it = values.begin();
            }
            selectedValue = it->first;
            buttonPressedLastFrame = false;
            
            // Change back color of the previously selected display
            display->setColor(dispMan->getElementColor(DisplayElement::DIGIT_DISPLAY));

            // Update display pointer for new selection
            display = getSelectedDisplay();
            
            // !!! JAML is the last one in the map and when it is selected it should loop around !!!
            if(display == nullptr){ 
                selectedValue = "L"; 
            }
        }
        else if(currentState){
            buttonPressedLastFrame = true;
        }

        dispMan->blinkingAnimation(display, DisplayElement::DIGIT_DISPLAY);        
        insertMode(values.at(selectedValue)); 
    }
}

void W_Local::handleSerialDebug()
{
    if (Serial.available() > 0) {
        String input = Serial.readStringUntil('\n');
        input.trim();
        input.toUpperCase();

        // Handle TAKT separately since it's not in signalMap
        if (input == "TAKT") {
            Serial.printf("[W_LOCAL][DEBUG]: Executing TAKT\n");
            this->takt();
            return;
        }

        // Check if it's a valid signal
        auto it = signalMap.find(input.c_str());
        if (it != signalMap.end()) {
            std::string buttonStr(input.c_str());
            
            auto itSignal = std::find(this->nextLineSignals.begin(), this->nextLineSignals.end(), buttonStr);
            if (itSignal != this->nextLineSignals.end()) {
                this->nextLineSignals.erase(itSignal);
            } else {
                if(isSignalValid(buttonStr)) {
                    this->nextLineSignals.push_back(buttonStr);
                }
            }
            
            auto itSig = this->signal.find(buttonStr);
            if (itSig != this->signal.end()) {
                itSig->second = !itSig->second;
            }
        } else {
            Serial.printf("[W_LOCAL][DEBUG]: Unknown command: %s\n", input.c_str());
            Serial.println("[W_LOCAL][DEBUG]: Valid commands: IL, WEL, WYL, WYAD, WEI, WEAK, DOD, ODE, PRZEP, WYAK, WEJA, WEA, CZYT, PISZ, WES, WYS, TAKT");
        }
    }
}

void W_Local::printValuesToSerial()
{
    static unsigned long lastPrintTime = 0;
    unsigned long currentTime = millis();
    uint16_t interval = 500;  // Changed from 50ms to 500ms for better readability

    if (currentTime - lastPrintTime >= interval) {
        Serial.println("\n========== STAN MASZYNY ==========");
        
        // Lista rozkazów
        Serial.print("Lista rozkazow: ");
        if (nextLineSignals.empty()) {
            Serial.println("[pusta]");
        } else {
            for (const auto& sig : nextLineSignals) {
                Serial.print(sig.c_str());
                Serial.print(" ");
            }
            Serial.println();
        }

        // Wartości z wyświetlaczy
        Serial.print("Wartosci: ");
        for (const auto& val : values) {
            Serial.print(val.first.c_str());
            Serial.print("=");
            Serial.print(val.second);
            Serial.print(" ");
        }
        Serial.println();

        // Pamięć PAO
        Serial.println("Pamiec PAO:");
        for(int i = 0; i < 4; i++){
            Serial.print("  [");
            Serial.print(i);
            Serial.print("]: ");
            Serial.print(this->PaO[i].first);
            Serial.print(" | ");
            Serial.println(this->PaO[i].second);
        }

        Serial.println("==================================\n");
        lastPrintTime = currentTime;
    }
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
    handleSerialDebug();

    handleInsertMode();

    readButtonInputs();

    refreshDisplay();

    printValuesToSerial();
}