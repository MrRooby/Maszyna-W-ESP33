#include "w_server.h"

AsyncWebServer *W_Server::server = nullptr;


W_Server::W_Server(DisplayManager *dispMan, HumanInterface *humInter, FileSystem *fileSystem) : 
    ws(new AsyncWebSocket("/ws")),
    dnsServer(new DNSServer()),
    dispMan(dispMan),
    humInter(humInter),
    fileSystem(fileSystem)
{
    this->localURL  = "http://" + LOCAL_IP.toString();

    // Workaround because deleting pointer to server causes crashing, therefore changed
    // to static member
    if(server == nullptr){
        server = new AsyncWebServer(80);
    }

    this->initServer();

    this->dispMan->clearDisplay();
}


W_Server::~W_Server()
{
    Serial.println("[W_SERVER]: Destructor: Cleaning up WebSocket clients...");
    ws->cleanupClients();

    Serial.println("[W_SERVER]: Destructor: Stopping DNS server...");
    dnsServer->stop();
    
    Serial.println("[W_SERVER]: Destructor: Ending HTTP server...");
    server->end();

    Serial.println("[W_SERVER]: Destructor: Disconnecting WiFi AP...");
    WiFi.softAPdisconnect(true);
    
    Serial.println("[W_SERVER]: Destructor: WebSocket deleted");
    delete ws;
    
    Serial.println("[W_SERVER]: Destructor: DNS deleted");
    delete dnsServer;
    
    dispMan    = nullptr;
    humInter   = nullptr;
    fileSystem = nullptr;
    
    lastSignal = nullptr;
    
    server     = nullptr;
    ws         = nullptr;
    dnsServer  = nullptr;
    
    this->humInter->controlOnboardLED(TOP, LOW);

    Serial.println("[W_SERVER]: Destructor: DONE");
}


void W_Server::initServer()
{
    this->humInter->controlOnboardLED(TOP, HIGH);
    
    this->createAccessPoint();
    this->createDNSServer();
    
    this->mountWebFiles();
    // this->createWebServer();
    
    this->server->begin();
    
    this->createWebSocketServer();
}


void W_Server::createWebServer()
{
    // !!! TODO jeśli coś nie działa z captive portalem to pewnie tutaj jest błąd
    // Required
    server->on("/connecttest.txt",     [](AsyncWebServerRequest *request) { 
        Serial.println("[W_SERVER]: connecttest.txt accessed");
        request->redirect("http://logout.net"); 
    });	            
    server->on("/wpad.dat",            [](AsyncWebServerRequest *request) { 
        Serial.println("[W_SERVER]: wpad.dat accessed");
        request->send(404); 
    });

    // Background responses
    server->on("/generate_204",        [](AsyncWebServerRequest *request) { 
        Serial.println("[W_SERVER]: generate_204 accessed");
        request->send(204); 
    });	                                
    server->on("/redirect",            [](AsyncWebServerRequest *request) { 
        Serial.println("[W_SERVER]: redirect accessed");
        request->redirect("http://192.168.4.1"); 
    });	        
    server->on("/hotspot-detect.html", [](AsyncWebServerRequest *request) { 
        Serial.println("[W_SERVER]: hotspot-detect.html accessed");
        request->redirect("http://192.168.4.1"); 
    });            
    server->on("/canonical.html",      [](AsyncWebServerRequest *request) { 
        Serial.println("[W_SERVER]: canonical.html accessed");
        request->redirect("http://192.168.4.1"); 
    }); 	        
    server->on("/success.txt",         [](AsyncWebServerRequest *request) { 
        Serial.println("[W_SERVER]: success.txt accessed");
        request->send(200, "text/plain", "success"); 
    });		
    server->on("/ncsi.txt",            [](AsyncWebServerRequest *request) { 
        Serial.println("[W_SERVER]: ncsi.txt accessed");
        request->send(200, "text/plain", "Microsoft NCSI"); 
    });	

    // return 404 to webpage icon
    server->on("/favicon.ico",         [](AsyncWebServerRequest *request) { 
        request->send(404); 
    });	

    server->on("/static/hotspot.txt", [](AsyncWebServerRequest *request) { 
        Serial.println("[W_SERVER]: static/hotspot.txt accessed");
        request->send(200, "text/plain", ""); 
    });
    
    // Serve Basic HTML Page - DEBUG VERSION
    server->on("/", HTTP_ANY, [this](AsyncWebServerRequest *request) {
        Serial.println("[W_SERVER]: / (root) accessed");
        if(LittleFS.exists("/index.html")) {
            Serial.println("[W_SERVER]: index.html EXISTS - sending it");
            request->send(LittleFS, "/index.html", "text/html");
        } else {
            Serial.println("[W_SERVER][ERROR]: index.html NOT FOUND!");
            request->send(200, "text/html", "<h1>No index.html found!</h1>");
        }
    });

    server->serveStatic("/", LittleFS, "/").setDefaultFile("index.html");

    // the catch all
    server->onNotFound([](AsyncWebServerRequest *request) {
        Serial.print("[W_SERVER]: onnotfound - URL: ");
        Serial.println(request->url());
        request->redirect("http://192.168.4.1");
    });
}


void W_Server::onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
    switch (type) {
        case WS_EVT_CONNECT:
            Serial.print("[W_SERVER]: ");
            Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
            break;

        case WS_EVT_DISCONNECT:
            Serial.print("[W_SERVER]: ");
            Serial.printf("WebSocket client #%u disconnected\n", client->id());
            break;

        case WS_EVT_DATA:
            this->handleWebSocketMessage(arg, data, len);
            break;

        case WS_EVT_PONG:
            // TODO: Websocket event case "PONG"

        case WS_EVT_ERROR:
            // TODO: Websocket event case "ERROR"
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
            Serial.println("[W_SERVER][ERROR]: Failed to deserialize JSON!");
            return;
        }
        
        // Check for message type
        String type = doc["type"] | "";

        if (type == "reg-update") {
            this->processPartialWebSocketData(doc);
        }
        else if (type == "mem-update") {
            this->processFullWebSocketData(doc);
        }
        else if (type == "color-update") {
            Serial.println("[W_SERVER]: Color Update Received");
            this->updateColors(doc);
        }
        else if (type == "ping"){
            Serial.println("[W_SERVER]: WebSocket Connection Active");
        }
        else {
            Serial.printf("[W_SERVER][ERROR]: Invalid message type: {%s}\n", type);
        }
    }
}


void W_Server::processPartialWebSocketData(StaticJsonDocument<512> doc) {
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


    //////////////////////////////////////// Display values ////////////////////////////////////////
    if (field == "acc" && this->dispMan->acc){
        Serial.print("[W_SERVER]: ");
        Serial.printf("Partial update: acc = %d\n", intValue);
        this->dispMan->acc->displayValue(intValue);
    }
    else if (field == "a" && this->dispMan->a){
        Serial.print("[W_SERVER]: ");
        Serial.printf("Partial update: a = %d\n", intValue);
        this->dispMan->a->displayValue(intValue);
    }
    else if (field == "s" && this->dispMan->s){
        Serial.print("[W_SERVER]: ");
        Serial.printf("Partial update: s = %d\n", intValue);
        this->dispMan->s->displayValue(intValue);
    }
    else if (field == "c" && this->dispMan->c){
        Serial.print("[W_SERVER]: ");
        Serial.printf("Partial update: c = %d\n", intValue);
        this->dispMan->c->displayValue(intValue);
    }
    else if (field == "i" && this->dispMan->i){
        Serial.print("[W_SERVER]: ");
        Serial.printf("Partial update: i = %d\n", intValue);
        this->dispMan->i->displayValue(intValue);
    }

    ////////////////////////////////////////// PaO values //////////////////////////////////////////
    else if (field == "addrs" && doc["addrs"].is<JsonArray>() && doc["args"].is<JsonArray>() && doc["vals"].is<JsonArray>() && this->dispMan->pao) {
        JsonArray addrsArray = doc["addrs"].as<JsonArray>();
        JsonArray argsArray = doc["args"].as<JsonArray>();
        JsonArray valsArray = doc["vals"].as<JsonArray>();
        for (size_t i = 0; i < 4; i++) {
            int addr = addrsArray[i];
            int arg = argsArray[i];
            int val = valsArray[i];
            Serial.print("[W_SERVER]: ");
            Serial.printf("Partial update: pao[%d] addr: %d, arg: %d, val: %d\n", (int)i, addr, arg, val);
            this->dispMan->pao[i]->displayLine(addr, val, arg);
        }
    }

    //////////////////////////////////////// Signal values /////////////////////////////////////////
    else if (field == "il" && this->dispMan->il){
        Serial.print("[W_SERVER]: ");
        Serial.printf("Partial update: il = %d\n", boolValue);
        this->dispMan->il->turnOnLine(boolValue);
    }
    else if (field == "wel" && this->dispMan->wel){
        Serial.print("[W_SERVER]: ");
        Serial.printf("Partial update: wel = %d\n", boolValue);
        this->dispMan->wel->turnOnLine(boolValue);
    }
    else if (field == "wyl" && this->dispMan->wyl){
        Serial.print("[W_SERVER]: ");
        Serial.printf("Partial update: wyl = %d\n", boolValue);
        this->dispMan->wyl->turnOnLine(boolValue);
    }
    else if (field == "wyad" && this->dispMan->wyad1 && this->dispMan->wyad2){
        Serial.print("[W_SERVER]: ");
        Serial.printf("Partial update: wyad = %d\n", boolValue);
        this->dispMan->wyad1->turnOnLine(boolValue);
        this->dispMan->wyad2->turnOnLine(boolValue);
    }
    else if (field == "wei" && this->dispMan->wei){
        Serial.print("[W_SERVER]: ");
        Serial.printf("Partial update: wei = %d\n", boolValue);
        this->dispMan->wei->turnOnLine(boolValue);
    }
    else if (field == "weak" && this->dispMan->weak){
        Serial.print("[W_SERVER]: ");
        Serial.printf("Partial update: weak = %d\n", boolValue);
        this->dispMan->weak->turnOnLine(boolValue);
    }
    else if (field == "dod" && this->dispMan->dod){
        Serial.print("[W_SERVER]: ");
        Serial.printf("Partial update: dod = %d\n", boolValue);
        this->dispMan->dod->turnOnLine(boolValue);
    }
    else if (field == "ode" && this->dispMan->ode){
        Serial.print("[W_SERVER]: ");
        Serial.printf("Partial update: ode = %d\n", boolValue);
        this->dispMan->ode->turnOnLine(boolValue);
    }
    else if (field == "przep" && this->dispMan->przep){
        Serial.print("[W_SERVER]: ");
        Serial.printf("Partial update: przep = %d\n", boolValue);
        this->dispMan->przep->turnOnLine(boolValue);
    }
    else if (field == "weja" && this->dispMan->weja){
        Serial.print("[W_SERVER]: ");
        Serial.printf("Partial update: weja = %d\n", boolValue);
        this->dispMan->weja->turnOnLine(boolValue);
    }
    else if (field == "wyak" && this->dispMan->wyak){
        Serial.print("[W_SERVER]: ");
        Serial.printf("Partial update: wyak = %d\n", boolValue);
        this->dispMan->wyak->turnOnLine(boolValue);
    }
    else if (field == "wea" && this->dispMan->wea){
        Serial.print("[W_SERVER]: ");
        Serial.printf("Partial update: wea = %d\n", boolValue);
        this->dispMan->wea->turnOnLine(boolValue);
    }
    else if (field == "czyt" && this->dispMan->czyt){
        Serial.print("[W_SERVER]: ");
        Serial.printf("Partial update: czyt = %d\n", boolValue);
        this->dispMan->czyt->turnOnLine(boolValue);
    }
    else if (field == "pisz" && this->dispMan->pisz){
        Serial.print("[W_SERVER]: ");
        Serial.printf("Partial update: pisz = %d\n", boolValue);
        this->dispMan->pisz->turnOnLine(boolValue);
    }
    else if (field == "wes" && this->dispMan->wes){
        Serial.print("[W_SERVER]: ");
        Serial.printf("Partial update: wes = %d\n", boolValue);
        this->dispMan->wes->turnOnLine(boolValue);
    }
    else if (field == "wys" && this->dispMan->wys){
        Serial.print("[W_SERVER]: ");
        Serial.printf("Partial update: wys = %d\n", boolValue);
        this->dispMan->wys->turnOnLine(boolValue);
    }
    else if (field == "busA" && this->dispMan->busA){
        Serial.print("[W_SERVER]: ");
        Serial.printf("Partial update: busA= %d\n", boolValue);
        this->dispMan->busA->turnOnLine(boolValue);
    }
    else if (field == "busS" && this->dispMan->busS){
        Serial.print("[W_SERVER]: ");
        Serial.printf("Partial update: busS= %d\n", boolValue);
        this->dispMan->busS->turnOnLine(boolValue);
    }
    else if (field == "stop" && this->dispMan->stop){
        Serial.print("[W_SERVER]: ");
        Serial.printf("Partial update: stop= %d\n", boolValue);
        this->dispMan->stop->turnOnLine(boolValue);
    }
}


void W_Server::processFullWebSocketData(StaticJsonDocument<512> doc)
{
    //////////////////////////////////////// Full data update ////////////////////////////////////////
    JsonObject dataObj = doc["data"];

    if (dataObj.containsKey("acc") && this->dispMan->acc){
        int accValue = dataObj["acc"];
        Serial.print("[W_SERVER]: Received ACC value: ");
        Serial.println(accValue);
        this->dispMan->acc->displayValue(accValue);
    }
    if (dataObj.containsKey("a") && this->dispMan->a){
        int aValue = dataObj["a"];
        Serial.print("[W_SERVER]: Received A value: ");
        Serial.println(aValue);
        this->dispMan->a->displayValue(aValue);
    }
    if (dataObj.containsKey("s") && this->dispMan->s){
        int sValue = dataObj["s"];
        Serial.print("[W_SERVER]: Received S value: ");
        Serial.println(sValue);
        this->dispMan->s->displayValue(sValue);
    }
    if (dataObj.containsKey("c") && this->dispMan->c){
        int cValue = dataObj["c"];
        Serial.print("[W_SERVER]: Received C value: ");
        Serial.println(cValue);
        this->dispMan->c->displayValue(cValue);
    }
    if (dataObj.containsKey("i") && this->dispMan->i){
        int iValue = dataObj["i"];
        Serial.print("[W_SERVER]: Received I value: ");
        Serial.println(iValue);
        this->dispMan->i->displayValue(iValue);
    }
    if (dataObj["addrs"].is<JsonArray>() && dataObj["args"].is<JsonArray>() && dataObj["vals"].is<JsonArray>() && this->dispMan->pao){
        JsonArray addrsArray = dataObj["addrs"].as<JsonArray>();
        JsonArray argsArray = dataObj["args"].as<JsonArray>();
        JsonArray valsArray = dataObj["vals"].as<JsonArray>();
        for (size_t i = 0; i < 4; i++){
            int addr = addrsArray[i];
            int arg = argsArray[i];
            int val = valsArray[i];
            Serial.print("[W_SERVER]: ");
            Serial.printf("addr: %d, arg: %d, val: %d\n", addr, arg, val);

            this->dispMan->pao[i]->displayLine(addr, val, arg);
        }
    }
}


void W_Server::sendDataToClient(char *buttonNum){
    StaticJsonDocument<256> doc;
    doc["type"] = "button_press";
    doc["buttonName"] = buttonNum;

    char jsonBuffer[256];
    size_t len = serializeJson(doc, jsonBuffer);

    this->ws->textAll(jsonBuffer, len);
}


void W_Server::mountWebFiles()
{
    if(!fileSystem->begin()){
        Serial.println("[W_SERVER][ERROR]: Web Files Mount Failed");
        return;
    }
    Serial.println("[W_SERVER]: Web Files Mounted Succesfully");

    server->serveStatic("/", LittleFS, "/").setDefaultFile("index.html");
}


void W_Server::runningServerLED(){
    static unsigned long lastToggleTime = 0;
    static bool ledState = false;

    if (WiFi.softAPgetStationNum() > 0){
        unsigned long currentMillis = millis();
        if (currentMillis - lastToggleTime >= 500){
            ledState = !ledState;
            humInter->controlOnboardLED(BOTTOM, ledState ? true : false);
            lastToggleTime = currentMillis;
        }
    }
    else{
        // Turn off LED if no stations are connected
        humInter->controlOnboardLED(BOTTOM, false);
        ledState = false;
    }
}


void W_Server::sendSignalValue()
{
    char* signal = humInter->getPressedButton();
    if(this->lastSignal != signal){
        if(signal != nullptr){
            this->sendDataToClient(signal);
            Serial.print("[W_SERVER]: Signal value sent: ");
            Serial.println(String(signal).c_str());
        }
        this->lastSignal = signal;
    }
}


void W_Server::handleLoadingAnimation()
{
    int currentClientCount = WiFi.softAPgetStationNum();
    
    if (currentClientCount > 0) {
        if (this->loading) {
            this->dispMan->clearDisplay();
            this->loading = false;

            this->dispMan->showIP(this->LOCAL_IP);
        }
    } 
    else {
        if (!this->loading && lastClientCount > 0) {
            this->loading = true;
        }
        
        if (this->loading) {
            this->dispMan->loadingAnimation();
        }
    }
    
    lastClientCount = currentClientCount;
}


void W_Server::updateColors(StaticJsonDocument<512> doc)
{
    JsonObject data = doc["data"];

    if(dispMan){
        if (data.isNull()) {
            Serial.println("[W_SERVER]: updateColors: data is NULL");
        } else {
            char buf[512];
            size_t n = serializeJson(data, buf, sizeof(buf));
            // Serial.print("[W_SERVER]: data JSON: ");
            // if (n > 0) Serial.println(buf); else Serial.println("(<empty>)");

            Serial.println("[W_SERVER]: data contains keys:");
            for (JsonPair kv : data) {
                // Serialize value to string for safe printing
                char vbuf[256];
                size_t vn = serializeJson(kv.value(), vbuf, sizeof(vbuf));

                Serial.print("  ");
                Serial.print(kv.key().c_str());
                Serial.print(" : ");
                if (vn > 0) Serial.println(vbuf); else Serial.println("(<empty>)");
            }
        }
        if(data["colorType"] == "signal_line_hex"){
            const char *colorHEX = data["hex"];
            dispMan->changeDisplayColor(colorHEX, "", "");
            fileSystem->saveColorConfig(DisplayElement::SIGNAL_LINE, colorHEX);
            Serial.printf("[W_SERVER] Signal Line Color: {%s}\n", colorHEX);
        }
        if(data["colorType"] == "display_hex"){
            const char *colorHEX = data["hex"];
            dispMan->changeDisplayColor("", colorHEX, "");
            fileSystem->saveColorConfig(DisplayElement::DIGIT_DISPLAY, colorHEX);
            Serial.printf("[W_SERVER] Display Color: {%s}\n", colorHEX);
        }
        if(data["colorType"] == "bus_hex"){
            const char *colorHEX = data["hex"];
            dispMan->changeDisplayColor("", "", colorHEX);
            fileSystem->saveColorConfig(DisplayElement::BUS_LINE, colorHEX);
            Serial.printf("[W_SERVER] Bus Color: {%s}\n", colorHEX);
        }
    }
    else {
        Serial.println("[W_SERVER][ERROR]: Couldn't update colors. DisplayManager is NULL");
    }
}


void W_Server::connectToWifi(){
    WiFi.begin(this->ssid, this->password);
    while (WiFi.status() != WL_CONNECTED){
    Serial.println("[W_SERVER]: Connection down!");
    }

    Serial.println("[W_SERVER]: ");
    Serial.println("[W_SERVER]: WiFi Connected!");
    Serial.print("[W_SERVER]: IP Address: ");
    Serial.println(WiFi.localIP());
}


void W_Server::createAccessPoint()
{
    Serial.print("[W_SERVER]: Setting up Access Point");
    WiFi.mode(WIFI_MODE_AP);
    WiFi.softAPConfig(LOCAL_IP, GATEWAY, SUBNET_MASK);
    WiFi.softAP(ssid, password, WIFI_CHANNEL, 0, MAX_CLIENTS);

    // Disable AMPDU RX on the ESP32 WiFi to fix a bug on Android
	esp_wifi_stop();
	esp_wifi_deinit();
	wifi_init_config_t my_config = WIFI_INIT_CONFIG_DEFAULT();
	my_config.ampdu_rx_enable = false;
	esp_wifi_init(&my_config);
	esp_wifi_start();
	vTaskDelay(100 / portTICK_PERIOD_MS);  // Add a small delay

    Serial.print("[W_SERVER]: AP IP address: ");
    Serial.println(WiFi.softAPIP());
    Serial.println("[W_SERVER]: Acess Point created");
}


void W_Server::createDNSServer()
{
	// Set the TTL for DNS response and start the DNS server
	dnsServer->setTTL(3600);
	dnsServer->start(53, "*", LOCAL_IP);
    Serial.println("[W_SERVER]: DNS Server started on port 53");
}


void W_Server::createWebSocketServer()
{
    this->ws->onEvent([this](AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len)
                     { this->onEvent(server, client, type, arg, data, len); });

    this->server->addHandler(ws);

    Serial.println("[W_SERVER]: Web Socket Server Started");
}


void W_Server::runServer()
{
    dnsServer->processNextRequest();
    dnsServer->processNextRequest();
    dnsServer->processNextRequest();
    delay(5);

    int stationCount = WiFi.softAPgetStationNum();
    static int lastCount = -1;
    if (stationCount != lastCount) {
        Serial.printf("[W_SERVER]: Connected stations: %d\n", stationCount);
        lastCount = stationCount;
    }

    this->handleLoadingAnimation();

    if(WiFi.softAPgetStationNum() > 0){
        this->sendSignalValue();
    }

    this->runningServerLED();

    this->dispMan->refreshDisplay();
}
