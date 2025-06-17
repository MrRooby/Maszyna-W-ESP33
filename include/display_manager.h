#pragma once

#include "three_digit_display.h"
#include "signal_line.h"
#include "bus_line.h"
#include "pao_display_line.h"

/**
 * @brief Pin definitions for LED strips
 */
#define LED_PORT_R 19    ///< Right LED strip data pin
#define LED_PORT_L 5    ///< Left  LED strip data pin

/**
 * @brief Manages multiple LED displays and signal lines
 * 
 * This class handles the initialization and control of two LED strips
 * and manages multiple ThreeDigitDisplay and SignalLine instances for 
 * showing values.
 * 
 * @author Bartosz Faruga / MrRooby
 * @date 2025
 */
class DisplayManager {
private:
    CRGB *ledsR;           ///< Array for right LED strip
    CRGB *ledsL;           ///< Array for left LED strip
    CFastLED stripR;       ///< FastLED controller for right strip
    CFastLED stripL;       ///< FastLED controller for left strip
    int numLedsR;          ///< Number of LEDs in right strip
    int numLedsL;          ///< Number of LEDs in left strip
    int brightness = 60;   ///< LED brightness level (0-255)

    /**
     * @brief Initialize the right LED strip
     * 
     * Sets up the WS2812B LED strip on the right side and creates displays
     */
    void initStripR();

    /**
     * @brief Initialize the left LED strip
     * 
     * Sets up the WS2812B LED strip on the left side and creates displays
     */
    void initStripL();

public:
    
    // Small three-part displays
    ThreeDigitDisplay *acc = nullptr;
    ThreeDigitDisplay *a   = nullptr;
    ThreeDigitDisplay *s   = nullptr;
    ThreeDigitDisplay *c   = nullptr;
    ThreeDigitDisplay *i   = nullptr;

    // Signal Lines
    SignalLine *il    = nullptr;
    SignalLine *wel   = nullptr;
    SignalLine *wyl   = nullptr;
    SignalLine *wyad1 = nullptr;
    SignalLine *stop  = nullptr;
    SignalLine *wyad2 = nullptr;
    SignalLine *wei   = nullptr;

    SignalLine *przep1 = nullptr;
    SignalLine *ode1   = nullptr;
    SignalLine *dod1   = nullptr;
    SignalLine *weak   = nullptr;
    
    SignalLine *przep2 = nullptr;
    SignalLine *ode2   = nullptr;
    SignalLine *dod2   = nullptr;
    
    SignalLine *weja  = nullptr;
    SignalLine *wyak  = nullptr;
    SignalLine *wea   = nullptr;

    SignalLine *czyt1  = nullptr;
    SignalLine *czyt2  = nullptr;
    SignalLine *pisz  = nullptr;
    
    SignalLine *wes   = nullptr;
    SignalLine *wys   = nullptr;

    // Bus lines
    BusLine *busA = nullptr;
    BusLine *busS = nullptr;

    // PaO Display Lines
    PaODisplayLine *pao[4];


    /**
     * @brief Construct a new Display Manager
     * 
     * @param numLedsR Number of LEDs in right strip
     * @param numLedsL Number of LEDs in left strip
     * @param brightness Initial brightness level (0-255)
     */
    DisplayManager(int numLedsR, int numLedsL, int brightness);


    /**
     * @brief Clear all LED displays
     * 
     * Turns off all LEDs in both strips
     */
    void clearDisplay();
};