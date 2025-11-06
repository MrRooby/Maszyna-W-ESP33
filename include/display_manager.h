#pragma once

#include "three_digit_display.h"
#include "signal_line.h"
#include "bus_line.h"
#include "pao_display_line.h"
#include "pins.h"
#include <unordered_map>

#define LED_COUNT_R 1000  ///< Maximum LED count for the right LED strip
#define LED_COUNT_L 1000  ///< Maximum LED count for the left LED strip

/**
 * @enum DisplayElement
 * @brief Enumeration of display element types for color configuration
 */
enum DisplayElement {
    DIGIT_DISPLAY, 
    SIGNAL_LINE,  
    BUS_LINE     
};

/**
 * @file display_manager.h
 * @brief Central manager for all LED displays and signal lines
 * 
 * Coordinates control of two NeoPixel LED strips (left and right) containing multiple
 * display types:
 * @li **Three-Digit Displays** - For showing accumulator, address, stack, counter, instruction
 * @li **Signal Lines** - For indicating machine signal states
 * @li **Bus Lines** - For showing data bus activity
 * @li **PAO Display Lines** - For showing Program-Address-Operand instruction format
 * 
 * Provides centralized color management, animation control, and display refresh capabilities.
 * 
 * @author Bartosz Faruga / MrRooby
 * @date 2025
 */
class DisplayManager {
private:
    NeoPixelBus<NeoGrbFeature, NeoEsp32Rmt0Ws2812xMethod> *stripR;  ///< NeoPixelBus controller for the right LED strip (RMT Channel 0)
    NeoPixelBus<NeoGrbFeature, NeoEsp32Rmt1Ws2812xMethod> *stripL;  ///< NeoPixelBus controller for the left LED strip (RMT Channel 1)
    
    const int timeBetweenAnimationFramesMilliseconds = 200;         ///< Time interval between animation frames in milliseconds
    unsigned long lastUpdate = 0;                                   ///< Timestamp of the last animation update

    RgbColor signalLineColor = RgbColor(0, 100, 0);                 ///< Default color for signal line (green)
    RgbColor displayColor    = RgbColor(100, 0, 0);                 ///< Default color for display (red)
    RgbColor busColor        = RgbColor(0, 0, 100);                 ///< Default color for bus line (blue)

    const unsigned long BLINK_INTERVAL = 500;                       ///< Interval for blinking animation in milliseconds
    unsigned long lastBlinkTime = 0;                                ///< Timestamp of the last blink state change
    bool blinkState = false;                                        ///< Current blink state (true = on, false = off)

    long lastRefreshTime = 0;                                       ///< Timestamp of the last display refresh 

    std::unordered_map<std::string, SignalLine*> signalLineMap;     ///< Map of signal line names to their corresponding SignalLine objects

    /**
     * @brief Initialize the right LED strip (RMT Channel 0)
     * 
     * Sets up the WS2812B LED strip on the right side with all connected displays
     * and signal lines. Automatically populates the signalLineMap.
     * 
     * @note Called during constructor
     * @see initStripL()
     */
    void initStripR();

    /**
     * @brief Initialize the left LED strip (RMT Channel 1)
     * 
     * Sets up the WS2812B LED strip on the left side with all connected displays
     * and signal lines. Automatically populates the signalLineMap.
     * 
     * @note Called during constructor
     * @see initStripR()
     */
    void initStripL();
    
    /**
     * @brief Convert a hex color string to an RgbColor object
     * 
     * Parses hex color strings in the format "#RRGGBB" and converts them to RgbColor.
     * 
     * **Examples:**
     * @li "#FF0000" → RgbColor(255, 0, 0) [red]
     * @li "#00FF00" → RgbColor(0, 255, 0) [green]
     * @li "#0000FF" → RgbColor(0, 0, 255) [blue]
     * 
     * @param colorHEX Hex color string (e.g., "#FF5733")
     * 
     * @return RgbColor object representing the parsed color
     * 
     * @note Invalid hex strings WILL RESULT in pink color: #880011
     */
    RgbColor hexToRgbColor(std::string colorHEX);
    
    /**
     * @brief Set colors for all display element types
     * 
     * Applies color configuration to all displays of each type simultaneously.
     * Updates signalLineColor, displayColor, and busColor internal states.
     * 
     * @param signalLineColorHEX Hex color string for signal lines (e.g., "#00FF00")
     * @param displayColorHEX Hex color string for digit displays (e.g., "#FF0000")
     * @param busColorHEX Hex color string for bus lines (e.g., "#0000FF")
     * 
     * @note Colors are immediately applied to all corresponding displays
     * @see hexToRgbColor()
     */
    void setDisplayColor(std::string signalLineColorHEX, std::string displayColorHEX, std::string busColorHEX);

public:
    
    // =====================================================================
    // @defgroup ThreeDigitDisplays Three-Digit Display Controls
    // @brief Displays for various 3-digit machine registers
    // @{
    // =====================================================================
    ThreeDigitDisplay *acc = nullptr;
    ThreeDigitDisplay *a   = nullptr;
    ThreeDigitDisplay *s   = nullptr;
    ThreeDigitDisplay *c   = nullptr;
    ThreeDigitDisplay *i   = nullptr;
    /** @} */

    // =====================================================================
    // @defgroup ControlSignalLines Control Signal Lines
    // @brief Signal lines indicating control and data flow signals
    // @{
    // =====================================================================
    SignalLine *il    = nullptr;
    SignalLine *wel   = nullptr;
    SignalLine *wyl   = nullptr;
    SignalLine *wyad1 = nullptr;
    SignalLine *stop  = nullptr;
    SignalLine *wyad2 = nullptr;
    SignalLine *wei   = nullptr;
    SignalLine *przep = nullptr;
    SignalLine *ode   = nullptr;
    SignalLine *dod   = nullptr;
    SignalLine *weak  = nullptr;
    SignalLine *weja  = nullptr;
    SignalLine *wyak  = nullptr;
    SignalLine *wea   = nullptr;
    SignalLine *czyt  = nullptr;
    SignalLine *pisz  = nullptr;
    SignalLine *wes   = nullptr;
    SignalLine *wys   = nullptr;
    /** @} */

    // =====================================================================
    // @defgroup BusLines Data Bus Lines
    // @brief Bus indicator lines for data flow visualization
    // @{
    // =====================================================================
    BusLine *busA = nullptr;
    BusLine *busS = nullptr;
    /** @} */
    
    // =====================================================================
    // @defgroup PAODisplays PAO Display Lines
    // @brief Program-Address-Operand instruction format displays
    // @{
    // =====================================================================
    PaODisplayLine *pao[4];
    /** @} */

    /**
     * @brief Construct a new Display Manager
     * 
     * Initializes both LED strips and all display components. Sets initial colors
     * from provided hex color strings. If colors are not provided, uses default values.
     * 
     * @param signalLineColorHEX Hex color string for signal lines (e.g., "#00FF00").
     *                           If nullptr, uses default green (0, 100, 0).
     * @param displayColorHEX Hex color string for digit displays (e.g., "#FF0000").
     *                        If nullptr, uses default red (100, 0, 0).
     * @param busColorHEX Hex color string for bus lines (e.g., "#0000FF").
     *                    If nullptr, uses default blue (0, 0, 100).
     * 
     * @note Constructor automatically calls initStripR() and initStripL()
     * @see initStripR()
     * @see initStripL()
     */
    DisplayManager(const char *signalLineColorHEX = nullptr, 
                   const char *displayColorHEX    = nullptr, 
                   const char *busColorHEX        = nullptr);

    /**
     * @brief Destructor
     * 
     * Cleans up all dynamically allocated LED strip and display objects.
     */
    ~DisplayManager();

    /**
     * @brief Start a loading animation on all displays
     * 
     * Runs synchronized loading animations across all displays simultaneously.
     * Useful for indicating system startup, data loading, or processing states.
     * 
     * @note This typically runs in a loop; call refreshDisplay() after for updates
     * @see refreshDisplay()
     */
    void loadingAnimation();

    /**
     * @brief Turn off all LEDs in both strips
     * 
     * Disables all LEDs on both the left and right LED strips by setting all
     * pixels to black (0, 0, 0). Useful for clearing the display between modes.
     * 
     * @see refreshDisplay()
     */
    void clearDisplay();

    /**
     * @brief Update the LED strips with current display state
     * 
     * Sends all pending updates to both NeoPixel LED strips. Should be called
     * regularly to reflect changes in display state on the actual hardware.
     */
    void refreshDisplay();

    /**
     * @brief Change the color of all display element types
     * 
     * Updates color configuration for signal lines, digit displays, and bus lines.
     * Any parameter set to nullptr retains the previous color value.
     * 
     * @param signalLineColorHEX Hex color string for signal lines (e.g., "#00FF00"),
     *                           or nullptr to keep current color.
     * @param displayColorHEX Hex color string for digit displays (e.g., "#FF0000"),
     *                        or nullptr to keep current color.
     * @param busColorHEX Hex color string for bus lines (e.g., "#0000FF"),
     *                    or nullptr to keep current color.
     * 
     * @note Changes apply immediately to all corresponding displays
     * @see setDisplayColor()
     */
    void changeDisplayColor(const char *signalLineColorHEX = nullptr, 
                            const char *displayColorHEX    = nullptr, 
                            const char *busColorHEX        = nullptr);

    /**
     * @brief Retrieve a signal line by its name
     * 
     * Looks up a signal line in the internal map by name and returns a pointer to it.
     * Useful for dynamic signal control without maintaining individual pointers.
     * 
     * **Valid signal names:**
     * @li "IL", "WEL", "WYL", "WYAD1", "STOP", "WYAD2", "WEI"
     * @li "PRZEP", "ODE", "DOD", "WEAK"
     * @li "WEJA", "WYAK", "WEA"
     * @li "CZYT", "PISZ"
     * @li "WES", "WYS"
     * 
     * @param signalName Name of the signal line to retrieve
     * 
     * @return Pointer to the SignalLine object, or nullptr if not found
     * 
     * @see setSignalLineState()
     */
    SignalLine* getSignalLine(const std::string& signalName);
    
    /**
     * @brief Control a signal line's on/off state by name
     * 
     * Finds a signal line by name and turns it on or off. Combines
     * getSignalLine() and turnOnLine() operations for convenience.
     * 
     * @param signalName Name of the signal line to control
     * @param state true to turn the signal on, false to turn it off
     * 
     * @note Does nothing if the signal name is not found
     * @see getSignalLine()
     */
    void setSignalLineState(const std::string& signalName, bool state);

    /**
     * @brief Get the current color of a display element type
     * 
     * Retrieves the RgbColor currently assigned to the specified display element type.
     * 
     * @param element The DisplayElement type (DIGIT_DISPLAY, SIGNAL_LINE, or BUS_LINE)
     * 
     * @return RgbColor object representing the current color for that element type
     * 
     * @see changeDisplayColor()
     */
    RgbColor getElementColor(const DisplayElement element);

    /**
     * @brief Display a blinking animation on a specific display element
     * 
     * Causes a display element to blink on and off at regular intervals. Useful for
     * highlighting active displays or indicating error states.
     * 
     * @param display Pointer to the LedElement to animate
     * @param type The DisplayElement type to determine blinking color
     * 
     * @note Blinking interval is controlled by BLINK_INTERVAL constant
     * @see loadingAnimation()
     */
    void blinkingAnimation(LedElement *display, const DisplayElement type);

    /**
     * @brief Run an LED strip test animation
     * 
     * Cycles through all LEDs on both strips.
     * Useful for verifying LED functionality and identifying faulty segments.
     * 
     * @param updateSpeedMillis Time in milliseconds between color changes
     * @param printInSerial If true, prints debug information to serial output
     * @param reset If true, resets the animation to the first frame
     * 
     * @note Best called with delays in a loop to see the animation
     * @see clearDisplay()
     */
    void ledTestAnimation(int updateSpeedMillis, bool printInSerial = false, bool reset = false);
    
    /**
     * @brief Set all LEDs on both strips to a specific color
     * 
     * Lights up all LEDs on both the left and right strips with the same RGB color.
     * Useful for status indication or uniform display mode.
     * 
     * @param red Red channel intensity (0-255). Default: 50
     * @param green Green channel intensity (0-255). Default: 0
     * @param blue Blue channel intensity (0-255). Default: 0
     * 
     * @note Changes require a refreshDisplay() call to appear on hardware
     * @see refreshDisplay()
     */
    void controlAllLEDs(int red = 50, int green = 0, int blue = 0);

    void showIP(IPAddress ip);
};