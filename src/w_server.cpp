#include "w_server.h"

AsyncWebServer *W_Server::server = nullptr;


W_Server::W_Server(DisplayManager *dispMan, HumanInterface *humInter) : 
    ws(new AsyncWebSocket("/ws")),
    dnsServer(new DNSServer()),
    localIP(192, 168, 4, 1),
    gateway(192, 168, 4, 1),
    subnet(255, 255, 255, 0),
    lastSignal(nullptr),
    loading(true)
{
    this->dispMan = dispMan;
    this->humInter = humInter; 

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
    Serial.println("Destructor: Cleaning up WebSocket clients...");
    ws->cleanupClients();

    Serial.println("Destructor: Stopping DNS server...");
    dnsServer->stop();
    
    Serial.println("Destructor: Ending HTTP server...");
    server->end();

    Serial.println("Destructor: Disconnecting WiFi AP...");
    WiFi.softAPdisconnect(true);
    
    delete ws;
    Serial.println("WebSocket deleted");
    
    delete dnsServer;
    Serial.println("DNS deleted");
    
    dispMan = nullptr;
    humInter = nullptr;
    lastSignal = nullptr;
    
    server = nullptr;
    ws = nullptr;
    dnsServer = nullptr;
    
    this->humInter->controlOnboardLED(TOP, LOW);
    Serial.println("Destructor: Done.");
}


void W_Server::initServer()
{
    this->humInter->controlOnboardLED(TOP, HIGH);
    
    this->createAccessPoint();
    this->createDNSServer();
    
    this->mountWebFiles();
    this->createWebServer();
    
    this->server->begin();
    
    this->createWebSocketServer();
}


void W_Server::createWebServer()
{
    const String localIPURL = "http://" + this->localIP.toString();

	// Required
	server->on("/connecttest.txt", [](AsyncWebServerRequest *request) { request->redirect("http://logout.net"); });	    // windows 11 captive portal workaround
    server->on("/wpad.dat", [](AsyncWebServerRequest *request) { request->send(404); });							

    // Background responses: Probably not all are Required, but some are. Others might speed things up?
    server->on("/generate_204", [](AsyncWebServerRequest *request) { request->redirect("http://192.168.4.1"); });		  // android captive portal redirect
    server->on("/redirect", [](AsyncWebServerRequest *request) { request->redirect("http://192.168.4.1"); });			  // microsoft redirect
    server->on("/hotspot-detect.html", [](AsyncWebServerRequest *request) { request->redirect("http://192.168.4.1"); });  // apple call home
    server->on("/canonical.html", [](AsyncWebServerRequest *request) { request->redirect("http://192.168.4.1"); }); 	  // firefox captive portal call home
    server->on("/success.txt", [](AsyncWebServerRequest *request) { request->send(200); });					              // firefox captive portal call home
    server->on("/ncsi.txt", [](AsyncWebServerRequest *request) { request->redirect("http://192.168.4.1"); });	    	  // windows call home

    // return 404 to webpage icon
    server->on("/favicon.ico", [](AsyncWebServerRequest *request) { request->send(404); });	// webpage icon

    // Serve Basic HTML Page
	server->on("/", HTTP_ANY, [](AsyncWebServerRequest *request) {
		request->send(LittleFS, "/index.html", "text/html");
		Serial.println("Served Basic HTML Page");
	});


    // the catch all
    server->onNotFound([](AsyncWebServerRequest *request) {
        request->redirect("http://192.168.4.1");
        Serial.print("onnotfound ");
        Serial.print(request->host());	// This gives some insight into whatever was being requested on the serial monitor
        Serial.print(" ");
        Serial.print(request->url());
    });
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

    /////////////////////////////////////// Color hex values ///////////////////////////////////////
    // TODO

    //////////////////////////////////////// Display values ////////////////////////////////////////
    if (field == "acc" && this->dispMan->acc){
        Serial.printf("Partial update: acc = %d\n", intValue);
        this->dispMan->acc->displayValue(intValue);
    }
    else if (field == "a" && this->dispMan->a){
        Serial.printf("Partial update: a = %d\n", intValue);
        this->dispMan->a->displayValue(intValue);
    }
    else if (field == "s" && this->dispMan->s){
        Serial.printf("Partial update: s = %d\n", intValue);
        this->dispMan->s->displayValue(intValue);
    }
    else if (field == "c" && this->dispMan->c){
        Serial.printf("Partial update: c = %d\n", intValue);
        this->dispMan->c->displayValue(intValue);
    }
    else if (field == "i" && this->dispMan->i){
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
            Serial.printf("Partial update: pao[%d] addr: %d, arg: %d, val: %d\n", (int)i, addr, arg, val);
            this->dispMan->pao[i]->displayLine(addr, val, arg);
        }
    }

    //////////////////////////////////////// Signal values /////////////////////////////////////////
    else if (field == "il" && this->dispMan->il){
        Serial.printf("Partial update: il = %d\n", boolValue);
        this->dispMan->il->turnOnLine(boolValue);
    }
    else if (field == "wel" && this->dispMan->wel){
        Serial.printf("Partial update: wel = %d\n", boolValue);
        this->dispMan->wel->turnOnLine(boolValue);
    }
    else if (field == "wyl" && this->dispMan->wyl){
        Serial.printf("Partial update: wyl = %d\n", boolValue);
        this->dispMan->wyl->turnOnLine(boolValue);
    }
    else if (field == "wyad" && this->dispMan->wyad1 && this->dispMan->wyad2){
        Serial.printf("Partial update: wyad = %d\n", boolValue);
        this->dispMan->wyad1->turnOnLine(boolValue);
        this->dispMan->wyad2->turnOnLine(boolValue);
    }
    else if (field == "wei" && this->dispMan->wei){
        Serial.printf("Partial update: wei = %d\n", boolValue);
        this->dispMan->wei->turnOnLine(boolValue);
    }
    else if (field == "weak" && this->dispMan->weak){
        Serial.printf("Partial update: weak = %d\n", boolValue);
        this->dispMan->weak->turnOnLine(boolValue);
    }
    else if (field == "dod" && this->dispMan->dod1 && this->dispMan->dod2){
        Serial.printf("Partial update: dod = %d\n", boolValue);
        this->dispMan->dod1->turnOnLine(boolValue);
        this->dispMan->dod2->turnOnLine(boolValue);
    }
    else if (field == "ode" && this->dispMan->ode1 && this->dispMan->ode2){
        Serial.printf("Partial update: ode = %d\n", boolValue);
        this->dispMan->ode1->turnOnLine(boolValue);
        this->dispMan->ode2->turnOnLine(boolValue);
    }
    else if (field == "przep" && this->dispMan->przep1 && this->dispMan->przep2){
        Serial.printf("Partial update: przep = %d\n", boolValue);
        this->dispMan->przep1->turnOnLine(boolValue);
        this->dispMan->przep2->turnOnLine(boolValue);
    }
    else if (field == "weja" && this->dispMan->weja){
        Serial.printf("Partial update: weja = %d\n", boolValue);
        this->dispMan->weja->turnOnLine(boolValue);
    }
    else if (field == "wyak" && this->dispMan->wyak){
        Serial.printf("Partial update: wyak = %d\n", boolValue);
        this->dispMan->wyak->turnOnLine(boolValue);
    }
    else if (field == "wea" && this->dispMan->wea){
        Serial.printf("Partial update: wea = %d\n", boolValue);
        this->dispMan->wea->turnOnLine(boolValue);
    }
    else if (field == "czyt" && this->dispMan->czyt1 && this->dispMan->czyt2){
        Serial.printf("Partial update: czyt = %d\n", boolValue);
        this->dispMan->czyt1->turnOnLine(boolValue);
        this->dispMan->czyt2->turnOnLine(boolValue);
    }
    else if (field == "pisz" && this->dispMan->pisz){
        Serial.printf("Partial update: pisz = %d\n", boolValue);
        this->dispMan->pisz->turnOnLine(boolValue);
    }
    else if (field == "wes" && this->dispMan->wes){
        Serial.printf("Partial update: wes = %d\n", boolValue);
        this->dispMan->wes->turnOnLine(boolValue);
    }
    else if (field == "wys" && this->dispMan->wys){
        Serial.printf("Partial update: wys = %d\n", boolValue);
        this->dispMan->wys->turnOnLine(boolValue);
    }
    else if (field == "busA" && this->dispMan->busA){
        Serial.printf("Partial update: busA= %d\n", boolValue);
        this->dispMan->busA->turnOnLine(boolValue);
    }
    else if (field == "busS" && this->dispMan->busS){
        Serial.printf("Partial update: busS= %d\n", boolValue);
        this->dispMan->busS->turnOnLine(boolValue);
    }
    else if (field == "stop" && this->dispMan->stop){
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
        Serial.print("Received ACC value: ");
        Serial.println(accValue);
        this->dispMan->acc->displayValue(accValue);
    }
    if (dataObj.containsKey("a") && this->dispMan->a){
        int aValue = dataObj["a"];
        Serial.print("Received A value: ");
        Serial.println(aValue);
        this->dispMan->a->displayValue(aValue);
    }
    if (dataObj.containsKey("s") && this->dispMan->s){
        int sValue = dataObj["s"];
        Serial.print("Received S value: ");
        Serial.println(sValue);
        this->dispMan->s->displayValue(sValue);
    }
    if (dataObj.containsKey("c") && this->dispMan->c){
        int cValue = dataObj["c"];
        Serial.print("Received C value: ");
        Serial.println(cValue);
        this->dispMan->c->displayValue(cValue);
    }
    if (dataObj.containsKey("i") && this->dispMan->i){
        int iValue = dataObj["i"];
        Serial.print("Received I value: ");
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
    if(!LittleFS.begin(true)){
        Serial.println("Web Files Mount Failed");
        return;
    }
    Serial.println("Web Files Mounted Succesfully");

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
        // Optionally, turn off LED if no stations are connected
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
            Serial.printf("Signal value sent: %s", String(signal).c_str());
            Serial.println();
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


void W_Server::connectToWifi(){
    WiFi.begin(this->ssid, this->password);
    while (WiFi.status() != WL_CONNECTED){
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
    WiFi.mode(WIFI_MODE_AP);
    WiFi.softAPConfig(localIP, gateway, subnet);
    WiFi.softAP(ssid, password, WIFI_CHANNEL, 0, MAX_CLIENTS);

    // Disable AMPDU RX on the ESP32 WiFi to fix a bug on Android
	esp_wifi_stop();
	esp_wifi_deinit();
	wifi_init_config_t my_config = WIFI_INIT_CONFIG_DEFAULT();
	my_config.ampdu_rx_enable = false;
	esp_wifi_init(&my_config);
	esp_wifi_start();
	vTaskDelay(100 / portTICK_PERIOD_MS);  // Add a small delay

    Serial.print("AP IP address: ");
    Serial.println(WiFi.softAPIP());
    Serial.println("Acess Point created");
}


void W_Server::createDNSServer()
{
	// Set the TTL for DNS response and start the DNS server
	dnsServer->setTTL(3600);
	dnsServer->start(53, "*", localIP);
}


void W_Server::createWebSocketServer()
{
    this->ws->onEvent([this](AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len)
                     { this->onEvent(server, client, type, arg, data, len); });

    this->server->addHandler(ws);

    Serial.println("Web Socket Server Started");
}


void W_Server::runServer()
{
    dnsServer->processNextRequest();
    delay(30);

    this->handleLoadingAnimation();

    if(WiFi.softAPgetStationNum() > 0)
        this->sendSignalValue();

    this->runningServerLED();

    this->dispMan->refreshDisplay();
}
