#include "human_interface.h"

// --- ISR Variables & Function (Global/Static scope for interrupt access) ---
static volatile long isrEncCounter = 0;
static volatile uint8_t isrLastState = 0;
static const int8_t isrEncoderTable[16] = {0, 1, -1, 0, -1, 0, 0, 1, 1, 0, 0, -1, 0, -1, 1, 0};

void IRAM_ATTR handleEncoderInterrupt() {
    uint8_t currState = (digitalRead(ENC_CLK) << 1) | digitalRead(ENC_DT);
    uint8_t index = (isrLastState << 2) | currState;
    int8_t val = isrEncoderTable[index];
    
    if (val != 0) {
        isrEncCounter += val;
    }
    isrLastState = currState;
}

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

    // Inicjalizacja stanu początkowego dla ISR
    isrLastState = (digitalRead(ENC_CLK) << 1) | digitalRead(ENC_DT);
    
    // Uruchomienie przerwań na obu pinach (CHANGE - zbocze narastające i opadające)
    attachInterrupt(digitalPinToInterrupt(ENC_CLK), handleEncoderInterrupt, CHANGE);
    attachInterrupt(digitalPinToInterrupt(ENC_DT), handleEncoderInterrupt, CHANGE);
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
    static unsigned long lastDebounceTime = 0;
    static bool lastReading = HIGH;
    static bool stableState = HIGH;
    
    bool currentReading = digitalRead(ENC_SW);
    unsigned long now = millis();
    
    // If the reading changed, reset debounce timer
    if (currentReading != lastReading) {
        lastDebounceTime = now;
    }
    
    // If enough time has passed since last change, accept the new state
    if ((now - lastDebounceTime) >= DEBOUNCE_BUTTON_MILLIS) {
        stableState = currentReading;
    }
    
    lastReading = currentReading;

    return stableState;
}

EncoderState HumanInterface::getEncoderState()
{
    long currentCounter;

    // Pobierz wartość licznika z ISR w sekcji krytycznej (atomowo)
    noInterrupts();
    currentCounter = isrEncCounter;
    interrupts();

    // Sprawdź czy licznik zmienił się o pełny krok (zazwyczaj 4 impulsy na 1 klik)
    // Używamy this->encLastCounter do śledzenia, co już obsłużyliśmy w pętli głównej
    if (currentCounter / 4 != this->encLastCounter / 4) {
        long diff = (currentCounter / 4) - (this->encLastCounter / 4);
        
        // Aktualizujemy nasz lokalny licznik do aktualnego stanu
        this->encLastCounter = currentCounter;
        
        if (diff > 0) return EncoderState::UP;
        else          return EncoderState::DOWN;
    }

    return EncoderState::IDLE;
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
