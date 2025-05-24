#include "w_server.h"

W_Server::W_Server() : 
    server(80), 
    ws("/ws"),
    dispMan(500, 500, 60),
    local_IP(192, 168, 4, 1),
    // buttons(),
    gateway(192, 168, 4, 1),
    subnet(255, 255, 255, 0)
{}


void W_Server::initServer()
{
    this->mountWebFiles();

    this->createAccessPoint();
    
    this->server.serveStatic("/", LittleFS, "/").setDefaultFile("index.html");
    
    this->createDNSServer();

    this->server.begin();
    
    this->createWebSocketServer();

    // Pinmode for status LED
    pinMode(2, OUTPUT);
}


void W_Server::onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
    switch (type) {
      case WS_EVT_CONNECT:
        Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
        break;
      case WS_EVT_DISCONNECT:
        Serial.printf("WebSocket client #%u disconnected\n", client->id());
        break;
      case WS_EVT_DATA:
        this->handleWebSocketMessage(arg, data, len);
        break;
      case WS_EVT_PONG:
      case WS_EVT_ERROR:
        break;
    }
}


void W_Server::handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
    AwsFrameInfo *info = (AwsFrameInfo*)arg;
    if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
        data[len] = 0;
        
        StaticJsonDocument<512> doc;
        DeserializationError error = deserializeJson(doc, (char*)data);

        if(error){
            Serial.println("Failed to deserialize JSON!");
            return;
        }
        
        // Value Displays
        if (doc.containsKey("acc")) {
            int accValue = doc["acc"];
            Serial.print("Received ACC value: ");
            Serial.println(accValue);

            // this->dispMan.acc->displayValue(accValue);
        }
        
        if (doc.containsKey("a")) {
            int aValue = doc["a"];
            Serial.print("Received A value: ");
            Serial.println(aValue);
            
            // this->dispMan.a->displayValue(aValue);
        }

        if (doc.containsKey("s")) {
            int sValue = doc["s"];
            Serial.print("Received S value: ");
            Serial.println(sValue);

            // this->dispMan.s->displayValue(sValue);
        }

        if (doc.containsKey("c")) {
            int cValue = doc["c"];
            Serial.print("Received C value: ");
            Serial.println(cValue);
            
            // this->dispMan.c->displayValue(cValue);
        }
        
        if (doc.containsKey("i")) {
            int iValue = doc["i"];
            Serial.print("Received I value: ");
            Serial.println(iValue);

            // this->dispMan.i->displayValue(iValue);
        }


        // Signal Displays
        if (doc.containsKey("il")) {
            bool ilValue = doc["il"];
            Serial.print("Received il value: ");
            Serial.println(ilValue);

            // this->dispMan.il->turnOnLine(ilValue);
        }

        if (doc.containsKey("wel")) {
            bool welValue = doc["wel"];
            Serial.print("Received wel value: ");
            Serial.println(welValue);

            // this->dispMan.wel->turnOnLine(welValue);
        }

        if (doc.containsKey("wyl")) {
            bool wylValue = doc["wyl"];
            Serial.print("Received wyl value: ");
            Serial.println(wylValue);

            // this->dispMan.wyl->turnOnLine(wylValue);
        }

        if (doc.containsKey("wyad")) {
            bool wyadValue = doc["wyad"];
            Serial.print("Received wyad value: ");
            Serial.println(wyadValue);

            // this->dispMan.wyad->turnOnLine(wyadValue);
        }

        if (doc.containsKey("wei")) {
            bool weiValue = doc["wei"];
            Serial.print("Received wei value: ");
            Serial.println(weiValue);

            // this->dispMan.wei->turnOnLine(weiValue);
        }

        if (doc.containsKey("weak")) {
            bool weakValue = doc["weak"];
            Serial.print("Received weak value: ");
            Serial.println(weakValue);

            // this->dispMan.weak->turnOnLine(weakValue);
        }

        if (doc.containsKey("dod")) {
            bool dodValue = doc["dod"];
            Serial.print("Received dod value: ");
            Serial.println(dodValue);

            // this->dispMan.dod->turnOnLine(dodValue);
        }

        if (doc.containsKey("ode")) {
            bool odeValue = doc["ode"];
            Serial.print("Received ode value: ");
            Serial.println(odeValue);

            // this->dispMan.ode->turnOnLine(odeValue);
        }

        if (doc.containsKey("przep")) {
            bool przepValue = doc["przep"];
            Serial.print("Received przep value: ");
            Serial.println(przepValue);

            // this->dispMan.przep->turnOnLine(przepValue);
        }

        if (doc.containsKey("weja")) {
            bool wejaValue = doc["weja"];
            Serial.print("Received weja value: ");
            Serial.println(wejaValue);

            // this->dispMan.weja->turnOnLine(wejaValue);
        }

        if (doc.containsKey("wyak")) {
            bool wyakValue = doc["wyak"];
            Serial.print("Received wyak value: ");
            Serial.println(wyakValue);

            // this->dispMan.wyak->turnOnLine(wyakValue);
        }

        if (doc.containsKey("wea")) {
            bool weaValue = doc["wea"];
            Serial.print("Received wea value: ");
            Serial.println(weaValue);

            // this->dispMan.wea->turnOnLine(weaValue);
        }

        if (doc.containsKey("czyt")) {
            bool czytValue = doc["czyt"];
            Serial.print("Received czyt value: ");
            Serial.println(czytValue);

            // this->dispMan.czyt->turnOnLine(czytValue);
        }

        if (doc.containsKey("pisz")) {
            bool piszValue = doc["pisz"];
            Serial.print("Received pisz value: ");
            Serial.println(piszValue);

            // this->dispMan.pisz->turnOnLine(piszValue);
        }

        if (doc.containsKey("wes")) {
            bool wesValue = doc["wes"];
            Serial.print("Received wes value: ");
            Serial.println(wesValue);

            // this->dispMan.wes->turnOnLine(wesValue);
        }

        if (doc.containsKey("wys")) {
            bool wysValue = doc["wys"];
            Serial.print("Received wys value: ");
            Serial.println(wysValue);

            // this->dispMan.wys->turnOnLine(wysValue);
        }
        
        
        // PaO Displays
        if (doc["addrs"].is<JsonArray>() && doc["args"].is<JsonArray>() && doc["vals"].is<JsonArray>()) {
            JsonArray addrsArray = doc["addrs"].as<JsonArray>();
            JsonArray argsArray = doc["args"].as<JsonArray>();
            JsonArray valsArray = doc["vals"].as<JsonArray>();

            Serial.print("Received addrs, args, and vals values: ");

            for (size_t i = 0; i < 4; i++) {
                int addr = addrsArray[i];
                int arg = argsArray[i];
                int val = valsArray[i];
                Serial.printf("(%d, %d, %d) ", addr, val, arg);
                
                // this->dispMan.pao[i]->displayLine(addr, val, arg);
            }
            Serial.println();
        }

        
        // Bus Displays
        if (doc.containsKey("busA")) {
            bool busAValue = doc["busA"];
            Serial.print("Received busA value: ");
            Serial.println(busAValue);
        
            // this->dispMan.busA->turnOnLine(busAValue);
        }

        if (doc.containsKey("busS")) {
            bool busSValue = doc["busS"];
            Serial.print("Received busS value: ");
            Serial.println(busSValue);
            
            // this->dispMan.busS->turnOnLine(busSValue);
        }
    }
}


void W_Server::sendDataToClient(char *buttonName)
{
    StaticJsonDocument<256> doc;
    doc["type"] = "button_press";
    doc["buttonName"] = buttonName;
    char jsonBuffer[256];
    size_t len = serializeJson(doc, jsonBuffer);

    Serial.printf("Sending WebSocket message: %s\n", jsonBuffer); // Debug log
    this->ws.textAll(jsonBuffer, len);
}


void W_Server::mountWebFiles()
{
    if(!LittleFS.begin(true)){
        Serial.println("Web Files Mount Failed");
        return;
    }
    Serial.println("Web Files Mounted Succesfully");

    server.serveStatic("/", LittleFS, "/").setDefaultFile("index.html");
}


void W_Server::blinkInbuiltLED()
{
    // Blink inbuilt LED without using delay
    static unsigned long lastBlinkTime = 0;
    static bool ledState = LOW;
    unsigned long currentTime = millis();

    if (WiFi.softAPgetStationNum() > 0 && currentTime - lastBlinkTime >= 500) {
        ledState = !ledState;
        digitalWrite(2, ledState);
        lastBlinkTime = currentTime;
    }
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
    Serial.println(WiFi.localIP());
}


void W_Server::createAccessPoint()
{
    Serial.print("Setting up Access Point");
    WiFi.softAP(ssid, password);

    // Configure the IP address if needed
    if (!WiFi.softAPConfig(local_IP, gateway, subnet)) {
        Serial.println("AP Config Failed");
    } else {
        Serial.println("AP Config Success");
    }

    Serial.print("AP IP address: ");
    Serial.println(WiFi.softAPIP());

    Serial.println("Acess Point created");
}


void W_Server::createDNSServer()
{
    dnsServer.start(53, "*", local_IP);
    Serial.println("DNS Server started");
    
    // Catch-all handler for captive portal
    this->server.onNotFound([this](AsyncWebServerRequest *request){
        String url = "http://" + this->local_IP.toString();
        request->redirect(url);
    });
}


void W_Server::createWebSocketServer()
{
    this->ws.onEvent([this](AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len)
                     { this->onEvent(server, client, type, arg, data, len); });

    this->server.addHandler(&ws);

    Serial.println("Web Socket Server Started");
}


void W_Server::runServer()
{
    dnsServer.processNextRequest();

    // char* signal = this->buttons.activeSignal();

    // if(this->lastSignal != signal){
    //     this->sendDataToClient(signal);
    //     this->lastSignal = signal;
    //     Serial.printf("Button pressed");
    // }

    this->blinkInbuiltLED();
}
