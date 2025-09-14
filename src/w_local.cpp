#include "w_local.h"

const std::unordered_map<std::string, W_Local::CommandFunction> W_Local::signalMap = {
    {"il",   &W_Local::il},
    {"wel",  &W_Local::wel},
    {"wyl",  &W_Local::wyl},
    {"wyad", &W_Local::wyad},
    {"wei",  &W_Local::wei},
    {"weak", &W_Local::weak},
    {"dod",  &W_Local::dod},
    {"ode",  &W_Local::ode},
    {"przep",&W_Local::przep},
    {"wyak", &W_Local::wyak},
    {"weja", &W_Local::weja},
    {"wea",  &W_Local::wea},
    {"czyt", &W_Local::czyt},
    {"pisz", &W_Local::pisz},
    {"wes",  &W_Local::wes},
    {"wys",  &W_Local::wys}};

W_Local::W_Local(DisplayManager *dispMan, HumanInterface *humInter)
{
    this->dispMan = dispMan;
    this->humInter = humInter;
}

W_Local::~W_Local(){}

void W_Local::runLocal()
{
    this->readButtonInputs();

    this->display();
}

void W_Local::baseSignal(uint16_t from, uint16_t &to)
{
    if(from > 999){
        from = 999;
        to = from;
    } 
}

void W_Local::il()
{
    if(this->L < 999){
        this->L++;
    }
}

void W_Local::wel()
{
    this->baseSignal(this->bus.at("A"), this->L);
}

void W_Local::wyl()
{
    this->baseSignal(this->L, this->bus.at("A"));
}

void W_Local::wyad()
{
    this->baseSignal(this->I, this->bus.at("A"));
}

void W_Local::wei()
{
    this->baseSignal(this->bus.at("S"), this->I);
}

void W_Local::weak()
{
    this->baseSignal(this->JAML, this->AK);
}

void W_Local::dod()
{
    uint16_t outcome = this->JAML + this->AK;
    this->baseSignal(outcome, this->JAML);
}

void W_Local::ode()
{
    uint16_t outcome = this->JAML - this->AK;
    this->baseSignal(outcome, this->JAML);
}

void W_Local::przep()
{
    this->baseSignal(this->JAML, this->AK);
}

void W_Local::wyak()
{
    this->baseSignal(this->AK, this->bus.at("S"));
}

void W_Local::weja()
{
    this->baseSignal(this->bus.at("S"), this->JAML);
}

void W_Local::wea()
{
    int value = this->bus.at("A");

    if(value >= 4)
        value = 3;

    this->A = this->bus.at("A");
}

void W_Local::czyt()
{
    this->baseSignal(this->PaO[this->A].first, this->S);
}

void W_Local::pisz()
{
    this->baseSignal(this->S, this->PaO[this->A].first);
}

void W_Local::wes()
{
    this->baseSignal(this->S, this->bus.at("S"));
}

void W_Local::wys()
{
    this->baseSignal(this->S, this->bus.at("S"));
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

void W_Local::display()
{
    if(this->dispMan){
        // Three digit displays
        if(this->dispMan->a)    this->dispMan->a->displayValue(this->A);
        if(this->dispMan->acc)  this->dispMan->acc->displayValue(this->AK);
        if(this->dispMan->c)    this->dispMan->c->displayValue(this->L);
        if(this->dispMan->i)    this->dispMan->i->displayValue(this->I);
        if(this->dispMan->s)    this->dispMan->s->displayValue(this->S);
        
        //Signal lines
        for(const auto& signal : this->signal){
            if(signal.second){
                this->dispMan->setSignalLineState(signal.first, signal.second);
            }
        }

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
}

void W_Local::readButtonInputs()
{
    char* button = this->humInter->getPressedButton();

    if(button == nullptr) return;

    Serial.println(button);

    std::string buttonStr(button);

    if(buttonStr == "TAKT"){
        this->takt();
    }
    else{
        this->nextLineSignals.push_back(buttonStr);
        
        // Check if signal exists in map before setting value
        auto it = this->signal.find(buttonStr);
        if(it != this->signal.end()) {
            it->second = true;
        }
    }
}
