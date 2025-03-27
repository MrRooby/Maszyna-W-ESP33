#pragma once

#include "three_segment_display.h"
#include "signal_line.h"
#include "bus_line.h"

/**
 * @brief Pin definitions for LED strips
 */
#define LED_PORT_R 5    ///< Right LED strip data pin
#define LED_PORT_L 19   ///< Left LED strip data pin

/**
 * @brief Manages multiple LED displays and signal lines
 * 
 * This class handles the initialization and control of two LED strips
 * and manages multiple ThreeSegmentDisplay instances for showing values.
 * 
 * @author Bartosz Faruga / MrRooby
 * @date 2025
 */
class DisplayManager {
private:
    CRGB *ledsR;           ///< Array for right LED strip
    CRGB *ledsL;           ///< Array for left LED strip
    CFastLED strip_R;      ///< FastLED controller for right strip
    CFastLED strip_L;      ///< FastLED controller for left strip
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
    
    // Small three part displays
    ThreeSegmentDisplay *c = nullptr;
    ThreeSegmentDisplay *i = nullptr;
    ThreeSegmentDisplay *acc = nullptr;
    ThreeSegmentDisplay *a = nullptr;
    ThreeSegmentDisplay *s = nullptr;
    
    // Signal Lines
    SignalLine *wec = nullptr;
    SignalLine *wyc = nullptr;
    SignalLine *wyad = nullptr;
    SignalLine *wei = nullptr;
    SignalLine *weak = nullptr;
    SignalLine *dod= nullptr;
    SignalLine *ode = nullptr;
    SignalLine *przep= nullptr;
    SignalLine *wyak = nullptr;
    SignalLine *weja = nullptr;
    SignalLine *wea = nullptr;
    SignalLine *czyt = nullptr;
    SignalLine *pisz = nullptr;
    SignalLine *wes = nullptr;
    SignalLine *wys = nullptr;

    // Bus lines
    BusLine *busA = nullptr;
    BusLine *busS = nullptr;


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