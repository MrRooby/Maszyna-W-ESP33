#include <FastLED.h>
#include "led_element.h"

/**
 * @file segment.cpp
 * @brief Implementation of the Segment class for controlling a 7-segment LED display.
 * 
 * This file contains the implementation of the Segment class, which is used to control
 * a 7-segment LED display using the FastLED library. The class provides functionality
 * to display numbers from 0 to 9 on the LED segments.
 * 
 * @author Bartosz Faruga / MrRooby
 * @date 2025
 */
class Segment : public LedElement
{
public:
    Segment();

    /**
     * @brief Construct a new Segment object
     *
     * @param strip Reference to a CFastLED object representing the LED strip
     * @param leds Pointer to the CRGB array representing the LED colors
     * @param startIndex The starting index of the segment in the LED strip
     * @param color The default color of the segment
     */
    Segment(CFastLED strip, CRGB *leds, int startIndex, CRGB color);

    /**
     * @brief Display selected number
     *
     * @param number number from 0 -> 9
     */
    void displayNumber(int number);
};