// #include <Arduino.h>

// // #include "w_server.h"

// // W_Server *server;
// const int sPins[4] = {21, 18, 19, 5};
// const int comPin = 23;

// int readChannel(int ch){
//   // ustaw adres
//   for(int b=0;b<4;b++){
//     digitalWrite(sPins[b], (ch>>b)&1);
//   }
//   delay(2); // poczekaj na ustabilizowanie
//   return digitalRead(comPin); // LOW => button pressed (przy INPUT_PULLUP)
// }


// void setup(){
//   for(int i=0;i<4;i++){
//     pinMode(sPins[i], OUTPUT);
//     digitalWrite(sPins[i], LOW);
//   }
//   pinMode(comPin, INPUT_PULLUP);
//   // EN must be tied to GND in wiring
//   pinMode(2, OUTPUT);
//   Serial.begin(9600);

//     // server = new W_Server();
    
//     // server->initServer();
//     Serial.println("Init");
// }


// void loop(){
//     // Serial.println("Test");
//   for(int ch=0; ch<16; ch++){
//     int val = readChannel(ch);
//     if(val == LOW){ // guzik wciśnięty (przy przycisk do GND + INPUT_PULLUP)
//       delay(30); // simple debounce
//       if(readChannel(ch) == LOW){
//         Serial.print("Button on channel ");
//         if(ch == 15)
//         digitalWrite(2, HIGH);
//         Serial.print(ch);
//         Serial.println(" pressed");
//         // tu możesz zrobić co potrzeba — i poczekać aż puści
//         while(readChannel(ch) == LOW) delay(10); // czekaj aż puści
//       }
//     }
//   }
//   delay(5); // krótka pauza między skanami