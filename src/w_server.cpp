#include "w_server.h"

void W_Server::initServer()
{
    this->mountWebFiles();

    this->connectToWifi();

    this->server.serveStatic("/", LittleFS, "/").setDefaultFile("index.html");

    this->server.begin();
    this->webSocket.begin();

    this->webSocket.onEvent([this](uint8_t num, WStype_t type, uint8_t *payload, size_t length) {
        this->webSocketEvent(num, type, payload, length);
    });

    Serial.println("WebSocket server started on port 81");
    Serial.println("HTTP Server Started");
}


void W_Server::webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length)
{
    switch (type) {
        case WStype_DISCONNECTED:
            Serial.printf("[%u] Client disconnected\n", num);
            break;
            
        case WStype_CONNECTED:
            {
                IPAddress ip = webSocket.remoteIP(num);
                Serial.printf("[%u] Client connected from %d.%d.%d.%d\n", num, ip[0], ip[1], ip[2], ip[3]);
                
                // Send welcome message
                webSocket.sendTXT(num, "Connected to ESP32");
            }
            break;
            
        case WStype_TEXT:
            Serial.printf("[%u] Received text: %s\n", num, payload);
            handleWebSocketMessage(num, payload);
            break;
            
        case WStype_ERROR:
            Serial.printf("[%u] Error occurred\n", num);
            break;
    }
}


void W_Server::handleWebSocketMessage(uint8_t num, uint8_t *payload){
    StaticJsonDocument<200> doc;
    DeserializationError error = deserializeJson(doc, payload);
    
    if (error) {
      Serial.println("Failed to parse JSON");
      return;
    }
  
    // Check if this is an ACC value message
    if (strcmp(doc["type"], "acc_value") == 0) {
      int accValue = doc["value"];
      Serial.print("Received ACC value: ");
      Serial.println(accValue);
      
      // Here you can use the ACC value
      // For example, output it to a pin:
      // analogWrite(LED_PIN, accValue);
    }
}

void W_Server::mountWebFiles()
{
    if(!LittleFS.begin(true)){
        Serial.println("Web Files Mount Failed");
        return;
    }
    Serial.println("Web Files Mounted Succesfully");
}

void W_Server::connectToWifi(){
    WiFi.begin(this->ssid, this->password);
    while (WiFi.status() != WL_CONNECTED){
        delay(500);
        Serial.println("Connection down!");
    }

    Serial.println("");
    Serial.println("WiFi Connectd!");
    Serial.print("IP Address: ");
    Serial.print(WiFi.localIP());
}


void W_Server::runServer()
{
    this->webSocket.loop();
}
