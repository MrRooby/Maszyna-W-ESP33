#include "w_server.h"

W_Server::W_Server() : 
    server(80), 
    ws("/ws"),
    dispMan(500, 500, 60),
    local_IP(192, 168, 4, 1),
    gateway(192, 168, 4, 1),
    subnet(255, 255, 255, 0)
{
    this->buttons = new Buttons();
}


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
        
        // Check for message type
        String type = doc["type"] | "";

        if (type == "reg-update"){
            this->processPartialWebSocketData(doc);
        }
        else if (type == "mem-update"){
            this->processFullWebSocketData(doc);
        }
        else {
            Serial.println("Invalid message type");
        }
    }
}


void W_Server::processPartialWebSocketData(StaticJsonDocument<512> doc){
    String field = doc["field"] | "";

    // Determine if value is int - display values or bool - signal lines
    bool isBool = doc["value"].is<bool>();
    bool boolValue = false;
    int intValue = 0;
    if (isBool) {
        boolValue = doc["value"].as<bool>();
        intValue = boolValue ? 1 : 0;
    } else {
        intValue = doc["value"].as<int>();
        boolValue = (intValue != 0);
    }


    // Display values
    if (field == "acc" && this->dispMan.acc){
        Serial.printf("Partial update: acc = %d\n", intValue);
        this->dispMan.acc->displayValue(intValue);
    }
    else if (field == "a" && this->dispMan.a){
        Serial.printf("Partial update: a = %d\n", intValue);
        this->dispMan.a->displayValue(intValue);
    }
    else if (field == "s" && this->dispMan.s){
        Serial.printf("Partial update: s = %d\n", intValue);
        this->dispMan.s->displayValue(intValue);
    }
    else if (field == "c" && this->dispMan.c){
        Serial.printf("Partial update: c = %d\n", intValue);
        this->dispMan.c->displayValue(intValue);
    }
    else if (field == "i" && this->dispMan.i){
        Serial.printf("Partial update: i = %d\n", intValue);
        this->dispMan.i->displayValue(intValue);
    }

    // PaO values
    else if (field == "addrs" && doc["addrs"].is<JsonArray>() && doc["args"].is<JsonArray>() && doc["vals"].is<JsonArray>() && this->dispMan.pao) {
        JsonArray addrsArray = doc["addrs"].as<JsonArray>();
        JsonArray argsArray = doc["args"].as<JsonArray>();
        JsonArray valsArray = doc["vals"].as<JsonArray>();
        for (size_t i = 0; i < 4; i++) {
            int addr = addrsArray[i];
            int arg = argsArray[i];
            int val = valsArray[i];
            Serial.printf("Partial update: pao[%d] addr: %d, arg: %d, val: %d\n", (int)i, addr, arg, val);
            this->dispMan.pao[i]->displayLine(addr, val, arg);
        }
    }

    // Signal values
    else if (field == "il" && this->dispMan.il){
        Serial.printf("Partial update: il = %d\n", boolValue);
        this->dispMan.il->turnOnLine(boolValue);
    }
    else if (field == "wel" && this->dispMan.wel){
        Serial.printf("Partial update: wel = %d\n", boolValue);
        this->dispMan.wel->turnOnLine(boolValue);
    }
    else if (field == "wyl" && this->dispMan.wyl){
        Serial.printf("Partial update: wyl = %d\n", boolValue);
        this->dispMan.wyl->turnOnLine(boolValue);
    }
    else if (field == "wyad" && this->dispMan.wyad){
        Serial.printf("Partial update: wyad = %d\n", boolValue);
        this->dispMan.wyad->turnOnLine(boolValue);
    }
    else if (field == "wei" && this->dispMan.wei){
        Serial.printf("Partial update: wei = %d\n", boolValue);
        this->dispMan.wei->turnOnLine(boolValue);
    }
    else if (field == "weak" && this->dispMan.weak){
        Serial.printf("Partial update: weak = %d\n", boolValue);
        this->dispMan.weak->turnOnLine(boolValue);
    }
    else if (field == "dod" && this->dispMan.dod1 && this->dispMan.dod2){
        Serial.printf("Partial update: dod = %d\n", boolValue);
        this->dispMan.dod1->turnOnLine(boolValue);
        this->dispMan.dod2->turnOnLine(boolValue);
    }
    else if (field == "ode" && this->dispMan.ode1 && this->dispMan.ode2){
        Serial.printf("Partial update: ode = %d\n", boolValue);
        this->dispMan.ode1->turnOnLine(boolValue);
        this->dispMan.ode2->turnOnLine(boolValue);
    }
    else if (field == "przep" && this->dispMan.przep1 && this->dispMan.przep2){
        Serial.printf("Partial update: przep = %d\n", boolValue);
        this->dispMan.przep1->turnOnLine(boolValue);
        this->dispMan.przep2->turnOnLine(boolValue);
    }
    else if (field == "weja" && this->dispMan.weja){
        Serial.printf("Partial update: weja = %d\n", boolValue);
        this->dispMan.weja->turnOnLine(boolValue);
    }
    else if (field == "wyak" && this->dispMan.wyak){
        Serial.printf("Partial update: wyak = %d\n", boolValue);
        this->dispMan.wyak->turnOnLine(boolValue);
    }
    else if (field == "wea" && this->dispMan.wea){
        Serial.printf("Partial update: wea = %d\n", boolValue);
        this->dispMan.wea->turnOnLine(boolValue);
    }
    else if (field == "czyt" && this->dispMan.czyt1 && this->dispMan.czyt2){
        Serial.printf("Partial update: czyt = %d\n", boolValue);
        this->dispMan.czyt1->turnOnLine(boolValue);
        this->dispMan.czyt2->turnOnLine(boolValue);
    }
    else if (field == "pisz" && this->dispMan.pisz){
        Serial.printf("Partial update: pisz = %d\n", boolValue);
        this->dispMan.pisz->turnOnLine(boolValue);
    }
    else if (field == "wes" && this->dispMan.wes){
        Serial.printf("Partial update: wes = %d\n", boolValue);
        this->dispMan.wes->turnOnLine(boolValue);
    }
    else if (field == "wys" && this->dispMan.wys){
        Serial.printf("Partial update: wys = %d\n", boolValue);
        this->dispMan.wys->turnOnLine(boolValue);
    }
    else if (field == "busA" && this->dispMan.busA){
        Serial.printf("Partial update: busA= %d\n", boolValue);
        this->dispMan.busA->turnOnLine(boolValue);
    }
    else if (field == "busS" && this->dispMan.busS){
        Serial.printf("Partial update: busS= %d\n", boolValue);
        this->dispMan.busS->turnOnLine(boolValue);
    }
}


void W_Server::processFullWebSocketData(StaticJsonDocument<512> doc)
{
    // Full data update
    JsonObject dataObj = doc["data"];

    if (dataObj.containsKey("acc") && this->dispMan.acc){
        int accValue = dataObj["acc"];
        Serial.print("Received ACC value: ");
        Serial.println(accValue);
        this->dispMan.acc->displayValue(accValue);
    }
    if (dataObj.containsKey("a") && this->dispMan.a){
        int aValue = dataObj["a"];
        Serial.print("Received A value: ");
        Serial.println(aValue);
        this->dispMan.a->displayValue(aValue);
    }
    if (dataObj.containsKey("s") && this->dispMan.s){
        int sValue = dataObj["s"];
        Serial.print("Received S value: ");
        Serial.println(sValue);
        this->dispMan.s->displayValue(sValue);
    }
    if (dataObj.containsKey("c") && this->dispMan.c){
        int cValue = dataObj["c"];
        Serial.print("Received C value: ");
        Serial.println(cValue);
        this->dispMan.c->displayValue(cValue);
    }
    if (dataObj.containsKey("i") && this->dispMan.i){
        int iValue = dataObj["i"];
        Serial.print("Received I value: ");
        Serial.println(iValue);
        this->dispMan.i->displayValue(iValue);
    }
    if (dataObj["addrs"].is<JsonArray>() && dataObj["args"].is<JsonArray>() && dataObj["vals"].is<JsonArray>() && this->dispMan.pao){
        JsonArray addrsArray = dataObj["addrs"].as<JsonArray>();
        JsonArray argsArray = dataObj["args"].as<JsonArray>();
        JsonArray valsArray = dataObj["vals"].as<JsonArray>();
        for (size_t i = 0; i < 4; i++){
            int addr = addrsArray[i];
            int arg = argsArray[i];
            int val = valsArray[i];
            Serial.printf("addr: %d, arg: %d, val: %d\n", addr, arg, val);

            this->dispMan.pao[i]->displayLine(addr, val, arg);
        }
    }
}


void W_Server::sendDataToClient(char *buttonNum){
    StaticJsonDocument<256> doc;
    doc["type"] = "button_press";
    doc["buttonName"] = buttonNum;

    char jsonBuffer[256];
    size_t len = serializeJson(doc, jsonBuffer);

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


void W_Server::runningServerLED(){
    // Blink inbuilt LED when server is running
    static unsigned long lastToggleTime = 0;
    static bool ledState = false;

    if (WiFi.softAPgetStationNum() > 0){
        unsigned long currentMillis = millis();
        if (currentMillis - lastToggleTime >= 500){
            ledState = !ledState;
            digitalWrite(2, ledState ? HIGH : LOW);
            lastToggleTime = currentMillis;
        }
    }
    else{
        // Optionally, turn off LED if no stations are connected
        digitalWrite(2, LOW);
        ledState = false;
    }
}


void W_Server::sendSignalValue()
{
    char* signal = buttons->activeSignal();
    if(this->lastSignal != signal){
        if(signal != nullptr){
            this->sendDataToClient(signal);
            Serial.printf("Signal value sent: %s", String(signal).c_str());
            Serial.println();
        }
        this->lastSignal = signal;
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

    // this->sendSignalValue();

    // Blink inbuilt LED when server is running
    this->runningServerLED();
}
