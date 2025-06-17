#include "encoder.h"

Encoder::Encoder(uint8_t pinA, uint8_t pinB, uint8_t buttonPin)
    : _pinA(pinA), _pinB(pinB), _buttonPin(buttonPin), _position(0), _lastState(0) {}

void Encoder::begin() {
    pinMode(_pinA, INPUT_PULLUP);
    pinMode(_pinB, INPUT_PULLUP);
    pinMode(_buttonPin, INPUT_PULLUP);
    _lastState = digitalRead(_pinA);
}

int32_t Encoder::read() {
    int stateA = digitalRead(_pinA);
    if (stateA != _lastState) {
        if (digitalRead(_pinB) != stateA) {
            _position++;
        } else {
            _position--;
        }
    }
    _lastState = stateA;
    return _position;
}

bool Encoder::isButtonPressed() {
    return digitalRead(_buttonPin) == LOW; // Assuming button is active LOW
}