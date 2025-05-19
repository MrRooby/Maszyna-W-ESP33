#include "input.h"

// Constructor
InputManager::InputManager(int *buttonPins, int buttonCount) {
    this->buttonPins = buttonPins;
    this->buttonCount = buttonCount;
    
    // Allocate memory for button states
    buttonStates = new bool[this->buttonCount];
    lastButtonStates = new bool[this->buttonCount];
    buttonPressEvents = new bool[this->buttonCount];
    lastDebounceTime = new unsigned long[this->buttonCount];

    // Initialize button states
    for (int i = 0; i < this->buttonCount; i++) {
        pinMode(buttonPins[i], INPUT_PULLUP); // Use internal pull-up resistors
        buttonStates[i] = false;
        lastButtonStates[i] = false;
        buttonPressEvents[i] = false;
        lastDebounceTime[i] = 0;
    }
}

// Destructor
InputManager::~InputManager() {
    delete[] buttonStates;
    delete[] lastButtonStates;
    delete[] buttonPressEvents;
    delete[] lastDebounceTime;
}

// Update button states with debounce logic
void InputManager::update() {
    for (int i = 0; i < buttonCount; i++) {
        bool currentState = !digitalRead(buttonPins[i]); // Active LOW
        unsigned long currentTime = millis();

        // Check if the state has changed
        if (currentState != lastButtonStates[i]) {
            lastDebounceTime[i] = currentTime; // Reset debounce timer
        }

        // Only update the button state if the debounce delay has passed
        if ((currentTime - lastDebounceTime[i]) > debounceDelay) {
            if (currentState != buttonStates[i]) {
                buttonStates[i] = currentState;

                // If the button is pressed, mark it as a press event
                if (currentState) {
                    buttonPressEvents[i] = true;
                }
            }
        }

        lastButtonStates[i] = currentState; // Update last state
    }
}

// Check if a button is currently pressed
bool InputManager::isButtonPressed(int buttonIndex) {
    if (buttonIndex < 0 || buttonIndex >= buttonCount) {
        return false; // Invalid index
    }
    return buttonStates[buttonIndex];
}

// Check if a button was pressed (state change from not pressed to pressed)
bool InputManager::wasButtonPressed(int buttonIndex) {
    if (buttonIndex < 0 || buttonIndex >= buttonCount) {
        return false; // Invalid index
    }

    // Check if a press event occurred and reset the event flag
    if (buttonPressEvents[buttonIndex]) {
        buttonPressEvents[buttonIndex] = false; // Reset the event flag
        return true;
    }
    return false;
}