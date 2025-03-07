#include <Arduino.h>
#include "w_server.h"
#include "leds.h"
#include "segment.h"

CRGB color = CRGB::Red;

Leds leds = Leds(60);
Segment test = Segment(leds.getLeds(), 0, color);
CFastLED fastLED = leds.getFastLED();

W_Server server;

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

void setup(){
    Serial.begin(115200);
    delay(1000);
    server.initServer();
}

void loop(){
    server.runServer();

    for (int i = 0; i < 10; i++){
        fastLED.clear();
        test.displayNumber(i);
        fastLED.show();
        delay(1000);
    }
}