#include "w_server.h"


void W_Server::initServer()
{
    this->mountWebFiles();
    Serial.println("Web files mounted");

    // this->connectToWifi();
    this->createAccessPoint();
    Serial.println("Acess Point created");

    this->server.serveStatic("/", LittleFS, "/").setDefaultFile("index.html");

    this->server.begin();
    Serial.println("Server Started");
    
    this->ws.onEvent([this](AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
        this->onEvent(server, client, type, arg, data, len);
    });
    
    this->server.addHandler(&ws);
    Serial.println("WS started");
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
        // Serial.println("Received payload");
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

        if (doc.containsKey("a")) {
            int aValue = doc["a"];
            Serial.print("Received A value: ");
            Serial.println(aValue);
        }

        if (doc.containsKey("acc")) {
            int accValue = doc["acc"];
            Serial.print("Received ACC value: ");
            Serial.println(accValue);
        }

        if (doc.containsKey("c")) {
            int cValue = doc["c"];
            Serial.print("Received C value: ");
            Serial.println(cValue);
        }

        if (doc.containsKey("i")) {
            int iValue = doc["i"];
            Serial.print("Received I value: ");
            Serial.println(iValue);
        }

        if (doc.containsKey("s")) {
            int sValue = doc["s"];
            Serial.print("Received S value: ");
            Serial.println(sValue);
        }

        if (doc.containsKey("addrs")) {
            JsonArray addrsArray = doc["addrs"];
            Serial.print("Received addrs values: ");
            for (int addr : addrsArray) {
                Serial.print(addr);
                Serial.print(" ");
            }
            Serial.println();
        }

        if (doc.containsKey("args")) {
            JsonArray argsArray = doc["args"];
            Serial.print("Received args values: ");
            for (int arg : argsArray) {
                Serial.print(arg);
                Serial.print(" ");
            }
            Serial.println();
        }

        if (doc.containsKey("busA")) {
            bool busAValue = doc["busA"];
            Serial.print("Received busA value: ");
            Serial.println(busAValue);
        }

        if (doc.containsKey("busS")) {
            bool busSValue = doc["busS"];
            Serial.print("Received busS value: ");
            Serial.println(busSValue);
        }

        if (doc.containsKey("czyt")) {
            bool czytValue = doc["czyt"];
            Serial.print("Received czyt value: ");
            Serial.println(czytValue);
        }

        if (doc.containsKey("dak")) {
            bool dakValue = doc["dak"];
            Serial.print("Received dak value: ");
            Serial.println(dakValue);
        }

        if (doc.containsKey("iak")) {
            bool iakValue = doc["iak"];
            Serial.print("Received iak value: ");
            Serial.println(iakValue);
        }

        if (doc.containsKey("icc")) {
            bool iccValue = doc["icc"];
            Serial.print("Received icc value: ");
            Serial.println(iccValue);
        }

        if (doc.containsKey("pisz")) {
            bool piszValue = doc["pisz"];
            Serial.print("Received pisz value: ");
            Serial.println(piszValue);
        }

        if (doc.containsKey("vals")) {
            JsonArray valsArray = doc["vals"];
            Serial.print("Received vals values: ");
            for (int val : valsArray) {
                Serial.print(val);
                Serial.print(" ");
            }
            Serial.println();
        }

        if (doc.containsKey("weak")) {
            bool weakValue = doc["weak"];
            Serial.print("Received weak value: ");
            Serial.println(weakValue);
        }

        if (doc.containsKey("wec")) {
            bool wecValue = doc["wec"];
            Serial.print("Received wec value: ");
            Serial.println(wecValue);
        }

        if (doc.containsKey("wei")) {
            bool weiValue = doc["wei"];
            Serial.print("Received wei value: ");
            Serial.println(weiValue);
        }

        if (doc.containsKey("weja")) {
            bool wejaValue = doc["weja"];
            Serial.print("Received weja value: ");
            Serial.println(wejaValue);
        }

        if (doc.containsKey("wyad")) {
            bool wyadValue = doc["wyad"];
            Serial.print("Received wyad value: ");
            Serial.println(wyadValue);
        }

        if (doc.containsKey("wyak")) {
            bool wyakValue = doc["wyak"];
            Serial.print("Received wyak value: ");
            Serial.println(wyakValue);
        }

        if (doc.containsKey("wyc")) {
            bool wycValue = doc["wyc"];
            Serial.print("Received wyc value: ");
            Serial.println(wycValue);
        }
    }
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
}


void W_Server::runServer()
{
    // this->webSocket.loop();

    if(WiFi.isConnected())
    {
        digitalWrite(2, HIGH);
        delay(500);
        digitalWrite(2, LOW);   
        delay(500);
    }
}
