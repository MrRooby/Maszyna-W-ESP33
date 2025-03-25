#include <FastLED.h>

class Segment{
    private:
        int startIndex;
        CRGB* leds;
        CRGB segmentColor = CRGB::Red;
        CRGB off = CRGB::Black;

    public:

        Segment();
        
        /**
         * @brief Construct and initialize a new 7 segment display
         * 
         * @param leds main led strip
         * @param startIndex index of the starting 
         *                      segment of the
         *                      display (middle segment) 
         * @param color display segment color
         */
        Segment(CRGB* leds, int startIndex, CRGB color);
        
        /**
         * @brief Display selected number
         * 
         * @param number number from 0 -> 9 
         */
        void displayNumber(int number);

        /**
         * @brief Change the color of the entire segment display
         * 
         * @param color display segment color
         */
        void changeColor(CRGB color);
};