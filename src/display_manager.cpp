#include "display_manager.h"

DisplayManager::DisplayManager(int numOfLEDs, int brightness)
{
    leds = Leds(brightness, numOfLEDs);

    this->initAcc(0);

    fastLED = leds.getFastLED();
}


void DisplayManager::initAcc(int index_start)
{
    for(int i = 0; i < 3; i++){
        acc[i] = Segment(leds.getLeds(), (index_start + (i * 7)), CRGB::Red);
    }
}


void DisplayManager::updateAccDisplay(int value)
{

    if(value < 0){
        value = 0;
    }
    else if(value > 999){
        value = 999;
    }

    int huns = ( value / 100 ) % 10;
    int tens = ( value / 10 ) % 10;
    int ones = value % 10;
    
    clearDisplay();
    
    acc[0].displayNumber(huns);
    acc[1].displayNumber(tens);
    acc[2].displayNumber(ones);
    
    fastLED.show();
}

void DisplayManager::clearDisplay() {
    fastLED.clear();
}