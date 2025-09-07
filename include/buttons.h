#ifndef BUTTONS_H
#define BUTTONS_H

#include <Arduino.h>
// #include <Adafruit_MPR121.h>
#include <utility>
#include <string>
#include <unordered_map>

/**
 * @class Buttons
 * @brief Handles input from capacitive touch buttons using the Adafruit MPR121 sensor.
 *
 * This class manages the initialization and reading of touch signals from one or more
 * Adafruit MPR121 capacitive touch sensor modules. It provides methods to detect which
 * button is currently being touched and to test the touch functionality.
 *
 * The class supports mapping touch pins to signal names for both left and right sets of buttons.
 * @author Bartosz Faruga / MrRooby
 * @date 2025
 */
class Buttons {
private:
    // Adafruit_MPR121 muxL = Adafruit_MPR121();
    // Adafruit_MPR121 muxR = Adafruit_MPR121();
    
    std::pair<int, char*> signalsL[8] = {
        {0, "il"},
        {1, "wel"},
        {2, "wyl"},
        {3, "wyad"},
        {4, "wei"},
        {5, "weak"},
        {6, "dod"},
        {7, "ode"} };
    
    std::pair<int, char*> signalsR[8] = {
        {0, "przep"},
        {1, "wyak"},
        {2, "weja"}, 
        {3, "wea"},
        {4, "czyt"},
        {5, "pisz"},
        {6, "wes"},
        {7, "wys"} };

public:
    /**
     * @brief Constructs a new Buttons object and initializes the multiplexer.
     *
     * This constructor sets up the necessary hardware interface for button input
     * by initializing the MPR121 multiplexer. If the multiplexer is not found,
     * the constructor will halt execution and print an error message.
     */
    Buttons();

    /**
     * @brief Destructor for the Buttons class.
     */
    ~Buttons();

    /**
     * @brief Checks which button signal is currently active.
     *
     * Scans the connected MPR121 touch sensor for any active (touched) pins.
     * Returns the signal name associated with the first detected active pin.
     *
     * @return Pointer to a character array representing the active signal name, or nullptr if no signal is active.
     */
    char* activeSignal();
};

#endif // BUTTONS_H