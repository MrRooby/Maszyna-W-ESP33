#include "human_interface.h"

void HumanInterface::setupMux()
{
    pinMode(MUX_COM, INPUT_PULLUP);
    pinMode(MUX_S0, OUTPUT);
    pinMode(MUX_S1, OUTPUT);
    pinMode(MUX_S2, OUTPUT);
    pinMode(MUX_S3, OUTPUT);
}

void HumanInterface::setupEncoder()
{
    pinMode(ENC_CLK, INPUT_PULLUP);
    pinMode(ENC_DT, INPUT_PULLUP);
    pinMode(ENC_SW, INPUT_PULLUP);
}

void HumanInterface::setupSwitches()
{
    pinMode(WIFI_SWITCH, INPUT_PULLUP);
    pinMode(WYS_BTN, INPUT_PULLUP);
}

void HumanInterface::setupOnboardLEDs()
{
    pinMode(ONBOARD_LED_TOP, OUTPUT);
    pinMode(ONBOARD_LED_BOTTOM, OUTPUT);
}

void HumanInterface::setupBacklightLEDs()
{
    pinMode(LED_BACKLIGHT_1, OUTPUT);
    pinMode(LED_BACKLIGHT_2, OUTPUT);
}

HumanInterface::HumanInterface()
{
    this->setupEncoder();
    this->setupMux();
    this->setupSwitches();
    this->setupOnboardLEDs();
    this->setupBacklightLEDs();

    this->controlBacklightLED(0);
    this->controlOnboardLED(TOP, LOW);
    this->controlOnboardLED(BOTTOM, LOW);
}

char* HumanInterface::getPressedButton()
{
    if(digitalRead(WYS_BTN) == LOW){
        return buttons.at(16);
    }

    uint16_t currentState = 0;
    
    for (int i = 0; i < 16; ++i) {
        digitalWrite(MUX_S0, i & 0x01);
        digitalWrite(MUX_S1, (i >> 1) & 0x01);
        digitalWrite(MUX_S2, (i >> 2) & 0x01);
        digitalWrite(MUX_S3, (i >> 3) & 0x01);

        delayMicroseconds(5);

        if (digitalRead(MUX_COM) == LOW)
            currentState |= (1 << i);
    }

    if (currentState != lastButtonState) {
        lastDebounceTime = millis();
        lastButtonState = currentState;
    }

    if ((millis() - lastDebounceTime) > DEBOUNCE_BUTTON_MILLIS) {
        debouncedState = currentState;
    }

    for (int i = 0; i < 16; ++i) {
        if (debouncedState & (1 << i)) {
            return buttons.at(i);
        }
    }
    
    return nullptr;
}

bool HumanInterface::WiFiEnabled()
{
    if(digitalRead(WIFI_SWITCH) == LOW){
        return true;
    }
    
    return false;
}

bool HumanInterface::getEncoderButtonState()
{
    static long lastStateChangeTime = 0;
    static bool prev = HIGH;
    bool current = digitalRead(ENC_SW); 

    unsigned long now = millis();

    if(prev != current){
        
        lastStateChangeTime = now;
    }
    
    if(now - lastStateChangeTime >= DEBOUNCE_BUTTON_MILLIS)
    {
        if(prev == HIGH && current == LOW)
            return current;
        
        prev = current;
    }
}

bool HumanInterface::getEncoderButtonLongPress()
{
    bool currentState = getEncoderButtonState();
    unsigned long now = millis();

    if(currentState){
        if( pressStartTime == 0){
            pressStartTime = now;
        }
        else if(now - pressStartTime >= LONG_PRESS_TIME) {
            return true;
        }
    }
    else {
        pressStartTime = 0;
    }

    return false;
}

EncoderState HumanInterface::getEncoderState()
{
    int currentCLK = digitalRead(ENC_CLK);
    int currentDT = digitalRead(ENC_DT);

    unsigned long now = millis();

    if (now - lastEncTime < DEBOUNCE_ENC_MILLIS) {
        return EncoderState::IDLE;
    }

    EncoderState returnState = EncoderState::IDLE;

    if(lastCLK == HIGH && currentCLK == LOW){
        if(currentDT == HIGH)
            returnState = EncoderState::UP;
        else
            returnState = EncoderState::DOWN;
        lastEncTime = now;
    }

    lastCLK = currentCLK;

    return returnState;
}

void HumanInterface::controlOnboardLED(OnboardLED led, bool choice)
{
    if(choice){
        if(led == TOP){
            analogWrite(ONBOARD_LED_TOP, ONBOARD_LED_BRIGHTNESS);
        }
        else if(led == BOTTOM){
            analogWrite(ONBOARD_LED_BOTTOM, ONBOARD_LED_BRIGHTNESS);
        }
    }
    else{
        if(led == TOP){
            analogWrite(ONBOARD_LED_TOP, 0);
        }
        else if(led == BOTTOM){
            analogWrite(ONBOARD_LED_BOTTOM, 0);
        }
    }
}

void HumanInterface::controlBacklightLED(int power)
{
    analogWrite(LED_BACKLIGHT_1, power);
    analogWrite(LED_BACKLIGHT_2, power);
}

void HumanInterface::testButtons()
{
    char* button = this->getPressedButton();
    static char* prevButton = nullptr;

    if(button != nullptr && prevButton != button) {
        Serial.printf("[HumanInterface]: %s pressed\n", this->getPressedButton());
    }

    prevButton = button;
}
