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

    if ((millis() - lastDebounceTime) > debounceDelayMilliseconds) {
        debouncedState = currentState;
    }

    for (int i = 0; i < 16; ++i) {
        if (debouncedState & (1 << i)) {
            return buttons.at(i);
        }
    }
    
    return nullptr;
}

bool HumanInterface::getWiFiSwitchState()
{
    if(digitalRead(WIFI_SWITCH) == LOW){
        return true;
    }
    
    return false;
}

void HumanInterface::controlOnboardLED(OnboardLED led, bool choice)
{
    if(choice){
        if(led == TOP){
            digitalWrite(ONBOARD_LED_TOP, HIGH);
        }
        else if(led == BOTTOM){
            digitalWrite(ONBOARD_LED_BOTTOM, HIGH);
        }
    }
    else{
        if(led == TOP){
            digitalWrite(ONBOARD_LED_TOP, LOW);
        }
        else if(led == BOTTOM){
            digitalWrite(ONBOARD_LED_BOTTOM, LOW);
        }
    }
}

void HumanInterface::controlBacklightLED(bool choice)
{
    if(choice){
        digitalWrite(LED_BACKLIGHT_1, HIGH);
        digitalWrite(LED_BACKLIGHT_2, HIGH);
    }
    else{
        digitalWrite(LED_BACKLIGHT_1, LOW);
        digitalWrite(LED_BACKLIGHT_2, LOW);
    }
}
