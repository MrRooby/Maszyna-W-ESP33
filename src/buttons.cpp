#include "buttons.h"

// Constructor
Buttons::Buttons() {
    this->initMux();
}

// Destructor
Buttons::~Buttons() {
}

void Buttons::initMux()
{
    // Default address is 0x5A, if tied to 3.3V its 0x5B
    // If tied to SDA its 0x5C and if SCL then 0x5D
    if (!this->muxL.begin(0x5A)/* && !this->muxR.begin(0x5B)*/ )
    {
        Serial.println("MPR121 not found, check wiring?");
        while (1);
    }
    Serial.println("MPR121 found!");
}

char* Buttons::activeSignal() {
    uint16_t touchedL = this->muxL.touched();
    // uint16_t touchedR = this->muxR.touched();

    for (auto it : this->signalsL){
        int pin = it.first;
        char *signal = it.second;

        if (touchedL & (1 << pin)){
            return signal; // Return the first detected active signal
        }
    }

    // for (auto it : this->signalsR){
    //     int pin = it.first;
    //     char *signal = it.second;

    //     if (touchedR & (1 << pin)){
    //         return signal; // Return the first detected active signal
    //     }
    // }

    return nullptr; // No active signal detected
}

void Buttons::test()
{
    uint16_t touched = this->muxL.touched();
    for (int i = 0; i < 12; i++) { // Assuming 12 pins on the MPR121
        if (touched & (1 << i)) {
            Serial.print("Pin ");
            Serial.print(i);
            Serial.println(" is touched.");
        }
    }

    delay(200);
}