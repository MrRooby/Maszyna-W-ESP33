#pragma once

#include "three_digit_display.h"
#include "signal_line.h"
#include "bus_line.h"
#include "pao_display_line.h"
#include "pins.h"
#include <unordered_map>

#define LED_COUNT_R 341
#define LED_COUNT_L 249

enum DisplayElement {
    DIGIT_DISPLAY,
    SIGNAL_LINE,
    BUS_LINE
};

/**
 * @brief Manages multiple LED displays and signal lines
 * 
 * This class handles the initialization and control of two LED strips
 * and manages multiple ThreeDigitDisplay, PaODisplayLine, BusLine and SignalLine instances for 
 * showing values.
 * 
 * @author Bartosz Faruga / MrRooby
 * @date 2025
 */
class DisplayManager {
private:
    NeoPixelBus<NeoGrbFeature, NeoEsp32Rmt0Ws2812xMethod> *stripR; ///< NeoPixelBus for left strip
    NeoPixelBus<NeoGrbFeature, NeoEsp32Rmt1Ws2812xMethod> *stripL; ///< NeoPixelBus for left strip
    
    int timeBetweenAnimationFramesMilliseconds = 200;
    unsigned long lastUpdate = 0;

    RgbColor signalLineColor = RgbColor(0, 0, 32);
    RgbColor displayColor = RgbColor(32, 0, 0);
    RgbColor busColor = RgbColor(0, 32, 0);

    const unsigned long BLINK_INTERVAL = 500;
    unsigned long lastBlinkTime = 0;
    bool blinkState = false;

    long lastRefreshTime = 0;

    std::unordered_map<std::string, SignalLine*> signalLineMap;

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
    
    RgbColor hexToRgbColor(std::string colorHEX);
    
    void setDisplayColor(std::string signalLineColorHEX = "", 
                         std::string displayColorHEX = "", 
                         std::string busColorHEX = "");

public:
    
    // Small three-part displays
    ThreeDigitDisplay *acc = nullptr;
    ThreeDigitDisplay *a   = nullptr;
    ThreeDigitDisplay *s   = nullptr;
    ThreeDigitDisplay *c   = nullptr;
    ThreeDigitDisplay *i   = nullptr;

    // Signal Lines
    SignalLine *il     = nullptr;
    SignalLine *wel    = nullptr;
    SignalLine *wyl    = nullptr;
    SignalLine *wyad1  = nullptr;
    SignalLine *stop   = nullptr;
    SignalLine *wyad2  = nullptr;
    SignalLine *wei    = nullptr;

    SignalLine *przep1 = nullptr;
    SignalLine *ode1   = nullptr;
    SignalLine *dod1   = nullptr;
    SignalLine *weak   = nullptr;
    
    SignalLine *przep2 = nullptr;
    SignalLine *ode2   = nullptr;
    SignalLine *dod2   = nullptr;
    
    SignalLine *weja   = nullptr;
    SignalLine *wyak   = nullptr;
    SignalLine *wea    = nullptr;

    SignalLine *czyt1  = nullptr;
    SignalLine *czyt2  = nullptr;
    SignalLine *pisz   = nullptr;
    
    SignalLine *wes    = nullptr;
    SignalLine *wys    = nullptr;

    // Bus lines
    BusLine *busA = nullptr;
    BusLine *busS = nullptr;

    // PaO Display Lines
    PaODisplayLine *pao[4];



    /**
     * @brief Constructs a DisplayManager object to control two LED strips.
     *
     * Initializes the right and left LED strips with the specified number of LEDs and brightness.
     * Also sets up all display elements and signal lines for both strips.
     *
     * @param numLedsR Number of LEDs on the right LED strip.
     * @param numLedsL Number of LEDs on the left LED strip.
     * @param brightness Brightness level for the LED strips.
     */
    DisplayManager(std::string signalLineColorHEX = "", 
                   std::string displayColorHEX = "", 
                   std::string busColorHEX = "");


    ~DisplayManager();

    void loadingAnimation();

    /**
     * @brief Clear all LED displays
     * 
     * Turns off all LEDs in both strips
     */
    void clearDisplay();

    void refreshDisplay();

    void changeDisplayColor(std::string signalLineColorHEX = "", 
                            std::string displayColorHEX = "", 
                            std::string busColorHEX = "");

    /**
     * @brief Get signal line by name for controlling
     * @param signalName Name of the signal line
     * @return Pointer to SignalLine or nullptr if not found
     */
    SignalLine* getSignalLine(const std::string& signalName);
    
    /**
     * @brief Turn signal line on/off
     * @param signalName Name of the signal line
     * @param state True to turn on, false to turn off
     */
    void setSignalLineState(const std::string& signalName, bool state);

    RgbColor getElementColor(const DisplayElement element);

    void blinkingAnimation(LedElement *display, const DisplayElement type);

    void ledTest(int updateSpeedMillis, bool printInSerial = false);
};