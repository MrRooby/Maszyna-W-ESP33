#include "leds.h"
#include "segment.h"

CRGB color = CRGB::Red;
Leds leds = Leds(color, 30);
Segment test = Segment(leds.getLeds(), 0, color);

void setup() {
}

void loop() {
  for(int i = 0; i < 10; i++){
    FastLED.clear();
    test.displayNumber(i);
    FastLED.show();
    delay(1000);
  }
}