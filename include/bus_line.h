#include "led_element.h"

class BusLine : public LedElement {
    private:
        int length = 0;

    public:

        BusLine();

        BusLine(CFastLED strip, CRGB *leds, int startIndex, int length, CRGB color);

        void turnOnLine(bool choice);
};