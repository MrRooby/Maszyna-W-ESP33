#include <FastLED.h>

#define NUM_OF_LEDS 7
#define LED_PORT 5

class Leds{
    private:
        CRGB leds[NUM_OF_LEDS];        
        CRGB color;

        CFastLED fastLED = FastLED;
        
    public:
        /**
         * @brief Initialize entire led strip
         * 
         * @param brightness desired brightness of the entrire strip
         */
        Leds(byte brightness);

        /**
         * @brief Get the leds strip
         * 
         * @return the entire led strip 
         */
        CRGB* getLeds();
        
        /**
         * @brief Get the initialized FastLED object
         * 
         * @return FastLED object 
         */
        CFastLED getFastLED();
};