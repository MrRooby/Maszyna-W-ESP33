#include "leds.h"
#include "segment.h"

CRGB color = CRGB::Red;

Leds leds = Leds(10);
Segment test = Segment(leds.getLeds(), 0, color);
CFastLED fastLED = leds.getFastLED();

CRGB colors[10] = {
  CRGB::Red,
  CRGB::Blue,
  CRGB::Green,
  CRGB::Orange,
  CRGB::Pink,
  CRGB::Chocolate,
  CRGB::Purple,
  CRGB::Aqua,
  CRGB::Yellow,
  CRGB::Gray
};


void setup() {
}

void loop() {
  for(int i = 0; i < 10; i++){
    fastLED.clear();
    int brighntness = 255 / ((i + 1) * 2);
    fastLED.setBrightness(brighntness);
    test.changeColor(colors[i]);
    test.displayNumber(i);
    fastLED.show();
    delay(1000);
  }
}