#include <Arduino.h>
#include "pins.h"

// Tablica przejść dla enkodera (Gray code)
// Indeks to: (stary_stan << 2) | nowy_stan
// Wartości: 1 (prawo), -1 (lewo), 0 (brak ruchu/błąd)
const int8_t encoder_table[16] = {0, 1, -1, 0, -1, 0, 0, 1, 1, 0, 0, -1, 0, -1, 1, 0};

uint8_t lastState = 0;
long counter = 0;
long lastCounter = 0;

void setup() {
    Serial.begin(115200);
    
    pinMode(ENC_CLK, INPUT_PULLUP);
    pinMode(ENC_DT, INPUT_PULLUP);
    pinMode(ENC_SW, INPUT_PULLUP);

    // Inicjalizacja stanu początkowego: (CLK << 1) | DT
    lastState = (digitalRead(ENC_CLK) << 1) | digitalRead(ENC_DT);
    
    Serial.println("=== TEST ENKODERA (METODA TABLICOWA) ===");
}

void loop() {
    // 1. Odczyt bieżącego stanu (2 bity)
    uint8_t currentState = (digitalRead(ENC_CLK) << 1) | digitalRead(ENC_DT);
    
    // 2. Jeśli stan się zmienił
    if (currentState != lastState) {
        // 3. Oblicz indeks w tablicy przejść
        uint8_t index = (lastState << 2) | currentState;
        int8_t val = encoder_table[index];
        
        // 4. Zaktualizuj licznik tylko jeśli przejście jest poprawne
        if (val != 0) {
            counter += val;
        }
        
        lastState = currentState;
    }

    // 5. Wypisywanie wyników (tylko gdy licznik się zmienił o pełny krok)
    // Enkodery zazwyczaj generują 4 zmiany stanu na 1 fizyczny "klik" (ząbek).
    // Dzielimy przez 4 (lub 2 zależnie od modelu), aby widzieć logiczne kroki.
    if (counter / 4 != lastCounter / 4) {
        long diff = (counter / 4) - (lastCounter / 4);
        
        if (diff > 0) Serial.print("PRAWO (CW)  >> ");
        else          Serial.print("LEWO  (CCW) << ");
        
        Serial.print("Pozycja: ");
        Serial.println(counter / 4);
        
        lastCounter = counter;
    }

    // Obsługa przycisku
    if (digitalRead(ENC_SW) == LOW) {
        Serial.println("[BUTTON] Wciśnięty");
        delay(200); // Prosty debounce dla
    }

}