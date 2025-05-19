#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include <Arduino.h>

class InputManager {
private:
    int *buttonPins;             // Array of button pins
    int buttonCount;             // Number of buttons
    bool *buttonStates;          // Current button states
    bool *lastButtonStates;      // Previous button states
    bool *buttonPressEvents;
    unsigned long *lastDebounceTime; // Last debounce time for each button
    const unsigned long debounceDelay = 50; // Debounce delay in milliseconds

public:
    InputManager(int *buttonPins, int buttonCount);

    ~InputManager();

    void update();

    bool isButtonPressed(int buttonIndex);

    bool wasButtonPressed(int buttonIndex);
};

#endif // INPUT_MANAGER_H