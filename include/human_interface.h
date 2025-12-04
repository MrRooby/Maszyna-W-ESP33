#pragma once

#include <Arduino.h>
#include "pins.h"
#include <map>

/**
 * @file human_interface.h
 * @brief Hardware interface for user input controls on the ESP32 machine
 * 
 * Manages all human-computer interaction including:
 * @li **Button Matrix** - 16 buttons via 4-to-16 multiplexer + 1 special button
 * @li **Rotary Encoder** - With push button and long-press detection
 * @li **Mode Switch** - WiFi mode selector
 * @li **Status LEDs** - On-board indicators (top/bottom) and backlight control
 * 
 * Provides debouncing for all inputs to ensure reliable signal detection.
 * 
 * @author Bartosz Faruga / MrRooby
 * @date 2025
 */

 /**
 * @enum OnboardLED
 * @brief On-board status LED positions
 */
enum OnboardLED {
    TOP,
    BOTTOM
};

/**
 * @enum EncoderState
 * @brief Rotary encoder rotation direction states
 */
enum EncoderState {
    IDLE = -1,
    UP = 1,
    DOWN = 2,
};

/**
 * @class HumanInterface
 * @brief Central input/output interface for user controls
 * 
 * Handles all GPIO interactions including button scanning, encoder processing,
 * and LED control. Implements debouncing for reliable input detection.
 */
class HumanInterface
{
private:
    uint16_t lastButtonState = 0;                           ///< Current raw button state from multiplexer (16-bit bitmask)
    uint16_t debouncedState = 0;                            ///< Debounced button state after DEBOUNCE_BUTTON_MILLIS delay
    unsigned long lastDebounceTime = 0;                     ///< Timestamp of last button state change
    const uint16_t DEBOUNCE_BUTTON_MILLIS = 20;             ///< Button debounce delay in milliseconds

    bool lastEncButtonState = LOW;                          ///< Last read encoder button state
    bool debouncedEncButtonState = LOW;                     ///< Debounced encoder button state
    unsigned long lastEncButtonDebounceTime = 0;            ///< Timestamp of last encoder button debounce update

    const unsigned int DEBOUNCE_ENC_MILLIS = 15;            ///< Encoder input debounce delay in milliseconds
    unsigned long lastEncTime;                              ///< Timestamp of last encoder state change
    int lastCLK = 0;                                        ///< Last CLK pin state (used for edge detection)
    const uint16_t LONG_PRESS_TIME = 500;                   ///< Time threshold for long press detection in milliseconds
    unsigned long pressStartTime = 0;                       ///< Timestamp when encoder button was first pressed

    const int ONBOARD_LED_BRIGHTNESS = 64;                  ///< PWM brightness level for on-board LEDs (0-255)
    
    /// @brief Button name mapping from multiplexer channel to signal names
    /// Maps channels 0-15 (multiplexer) and 16 (WYS special button) to signal command names
    const std::map<int, char*> buttons = {
        { 1,  "PRZEP" },
        { 2,  "ODE" },
        { 3,  "WYAD" },
        { 4,  "DOD" },
        { 5,  "WEL" },
        { 6,  "WYL" },
        { 7,  "WEAK" },
        { 8,  "WEA" },
        { 10, "PISZ" },
        { 11, "CZYT" },
        { 12, "WYS" },
        { 13, "IL" },
        { 14, "WYAK" },
        { 15, "WES" },
        
        
        
        { 16, "WEI" },
        { 0,  "WEJA" },
        { 9,  "TAKT" },
        
    };

    /*
     * @brief Initialize the multiplexer (4-to-16 decoder) GPIO pins
     * 
     * Sets up MUX_COM as INPUT_PULLUP and MUX_S0-S3 as OUTPUT pins
     * for button matrix scanning.
     * 
     * @note Called automatically in constructor
     */
    void setupMux();     

    /**
     * @brief Initialize the rotary encoder GPIO pins
     * 
     * Sets up ENC_CLK, ENC_DT, and ENC_SW as INPUT_PULLUP for encoder input.
     * 
     * @note Called automatically in constructor
     */
    void setupEncoder();   
    
    /**
     * @brief Initialize mode switch and special button GPIO pins
     * 
     * Sets up WIFI_SWITCH and WYS_BTN as INPUT_PULLUP.
     * 
     * @note Called automatically in constructor
     */
    void setupSwitches();

    /**
     * @brief Initialize on-board status LED GPIO pins
     * 
     * Sets up ONBOARD_LED_TOP and ONBOARD_LED_BOTTOM as OUTPUT pins for PWM control.
     * 
     * @note Called automatically in constructor
     */
    void setupOnboardLEDs();

    /**
     * @brief Initialize backlight LED control GPIO pins
     * 
     * Sets up LED_BACKLIGHT_1 and LED_BACKLIGHT_2 as OUTPUT pins for PWM control.
     * 
     * @note Called automatically in constructor
     */
    void setupBacklightLEDs();
         
public:

    /**
     * @brief Construct a new Human Interface controller
     * 
     * Initializes all GPIO pins and hardware interfaces:
     * @li Multiplexer for button matrix scanning
     * @li Rotary encoder with push button
     * @li Mode switches
     * @li Status LEDs (on-board and backlight)
     * 
     * Automatically sets initial LED states (backlight off, status LEDs off).
     */
    HumanInterface();

    /**
     * @brief Get the currently pressed button
     * 
     * Scans the button matrix via multiplexer and returns the name of the first
     * pressed button found. Implements debouncing to filter out noise.
     * 
     * **Scanning priority:**
     * @li Checks special WYS button first (highest priority)
     * @li Then scans multiplexer channels 0-15 in order
     * 
     * @return Pointer to the button name string (e.g., "IL", "WEL", "WYS"),
     *         or nullptr if no button is pressed
     * 
     * @note Returns only the first pressed button; if multiple buttons are pressed,
     *       only the first one in scan order is reported
     * @see buttons
     */
    char* getPressedButton();

    /**
     * @brief Check if WiFi mode switch is enabled
     * 
     * Reads the WIFI_SWITCH GPIO pin to determine if the device should operate
     * in WiFi server mode or local mode.
     * 
     * @return true if WiFi is enabled (switch is LOW), false otherwise
     */
    bool WiFiEnabled();

    /**
     * @brief Get the current debounced state of the encoder push button
     * 
     * Returns true when the encoder button transitions from unpressed to pressed.
     * Implements debouncing with DEBOUNCE_ENC_MILLIS delay.
     * 
     * @return true if encoder button is currently pressed (falling edge),
     *         false otherwise
     * 
     * @note This is an edge detector, not a level detector
     */
    bool getEncoderButtonState();

    /**
     * @brief Detect long press on the encoder button
     * 
     * Returns true if the encoder button has been held down for at least
     * LONG_PRESS_TIME milliseconds (500ms by default).
     * 
     * @return true if long press is detected and maintained,
     *         false when button is released or LONG_PRESS_TIME not yet reached
     * 
     * @note Resets the press timer when button is released
     */
    bool getEncoderButtonLongPress();

    /**
     * @brief Get the current rotary encoder rotation direction
     * 
     * Detects encoder rotation by monitoring the CLK and DT pins.
     * Uses edge detection (falling edge on CLK) to determine direction.
     * 
     * **Rotation detection logic:**
     * @li CLK goes HIGH → LOW AND DT is HIGH: Clockwise (UP)
     * @li CLK goes HIGH → LOW AND DT is LOW: Counter-clockwise (DOWN)
     * 
     * @return EncoderState value:
     *         @li IDLE (-1) - No rotation detected
     *         @li UP (1) - Clockwise rotation
     *         @li DOWN (2) - Counter-clockwise rotation
     * 
     * @note Implements debouncing with DEBOUNCE_ENC_MILLIS delay
     */
    EncoderState getEncoderState();

    /**
     * @brief Control an on-board status LED
     * 
     * Turns one of the on-board indicator LEDs on or off using PWM.
     * LED brightness is fixed at ONBOARD_LED_BRIGHTNESS (64) when on.
     * 
     * @param led The LED to control (TOP or BOTTOM)
     * @param choice true to turn on, false to turn off
     * 
     * @note Uses analogWrite for PWM control
     * @see controlBacklightLED()
     */
    void controlOnboardLED(OnboardLED led, bool choice);
    
    /**
     * @brief Control the backlight LED brightness
     * 
     * Sets the brightness level for both backlight LED channels using PWM.
     * Both LED_BACKLIGHT_1 and LED_BACKLIGHT_2 are controlled to the same level.
     * 
     * @param power PWM brightness level (0-255).
     *              @li 0 = off
     *              @li 255 = full brightness
     * 
     * @note Uses analogWrite for PWM control
     * @see controlOnboardLED()
     */
    void controlBacklightLED(int power);

    /**
     * @brief Test function to monitor button presses via serial output
     * 
     * Continuously monitors button input and prints button names to serial console
     * when pressed. Useful for debugging button matrix and verifying button mapping.
     * 
     * **Serial output example:**
     * ```
     * [HumanInterface]: IL pressed
     * [HumanInterface]: WEL pressed
     * [HumanInterface]: TAKT pressed
     * ```
     * 
     * @note Should be called in a loop; prints only on state change
     */
    void testButtons();
};