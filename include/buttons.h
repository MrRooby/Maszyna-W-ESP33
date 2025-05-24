#ifndef BUTTONS_H
#define BUTTONS_H

#include <Arduino.h>
#include <Adafruit_MPR121.h>
#include <utility>
#include <string>
#include <unordered_map>

class Buttons {
private:
    Adafruit_MPR121 muxL = Adafruit_MPR121();
    Adafruit_MPR121 muxR = Adafruit_MPR121();
    
    std::pair<int, char*> signalsL[8] = {
        {0, "il"},
        {1, "wel"},
        {2, "wyl"},
        {3, "wyad"},
        {4, "wei"},
        {5, "weak"},
        {6, "dod"},
        {7, "ode"} };
    
    std::pair<int, char*> signalsR[8] = {
        {0, "przep"},
        {1, "wyak"},
        {2, "weja"}, 
        {3, "wea"},
        {4, "czyt"},
        {5, "pisz"},
        {6, "wes"},
        {7, "wys"} };

public:
    Buttons();

    ~Buttons();

    void initMux();

    char* activeSignal();

    void test();

    // bool isTouched(char* signal);
};

#endif // BUTTONS_H