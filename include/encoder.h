#pragma once

#include <Arduino.h>

class Encoder {
public:
    Encoder(uint8_t pinA, uint8_t pinB, uint8_t buttonPin);

    void begin();
    int32_t read();
    bool isButtonPressed();

private:
    uint8_t _pinA, _pinB, _buttonPin;
    int32_t _position;
    int8_t _lastState;
};