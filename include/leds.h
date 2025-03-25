#include <FastLED.h>

#define LED_PORT 5

class Leds{
    private:
        CRGB* leds;        
        CRGB color;
        int numLeds;

        CFastLED fastLED = FastLED;
        
    public:

        Leds();

        ~Leds();
        
        /**
         * @brief Initialize entire led strip
         * 
         * @param brightness desired brightness of the entrire strip
         */
        Leds(byte brightness, int numLeds);

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