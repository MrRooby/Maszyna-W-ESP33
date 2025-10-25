#include "file_system.h"

const char* FileSystem::getDisplayElementName(const DisplayElement element)
{
    const char *elementName = nullptr;
 
    if(element      == DisplayElement::SIGNAL_LINE)   {elementName = "signal_line"; }
    else if(element == DisplayElement::DIGIT_DISPLAY) {elementName = "display"; }
    else if(element == DisplayElement::BUS_LINE)      {elementName = "bus"; }

    return elementName;
}


FileSystem::FileSystem() : mounted(false) {}


FileSystem::~FileSystem() {}


bool FileSystem::begin(bool formatOnFail) {
    if(mounted){
        Serial.println("[FileSystem]: Already mounted");
        return true;
    }

    if(!LittleFS.begin(formatOnFail)){
        Serial.println("[FileSystem][ERROR]: Mount failed");
        return false;
    }

    mounted = true;
    Serial.println("[FileSystem]: Mounted successfully");

    if(!fileExists(configPath)){
        Serial.println("[FileSystem]: Config file not found, creating default");
        // createDefaultConfig();
    }

    return true;
}


bool FileSystem::isMounted() const {
    return mounted;
}


void FileSystem::end() {
    if(mounted) {
        LittleFS.end();
        mounted = false;
        Serial.println("[FileSystem]: Unmounted");
    }
}


bool FileSystem::fileExists(const char *path) {
    if (!mounted) 
        return false;

    return LittleFS.exists(path);
}


bool FileSystem::saveColorConfig(const DisplayElement element, const char *colorHEX) {
    // Load existing config or create new one
    StaticJsonDocument<1024> doc;
    
    File file = LittleFS.open(configPath, "r");
    if (file) {
        DeserializationError error = deserializeJson(doc, file);
        file.close();
        if (error) {
            Serial.println("[FileSystem][ERROR]: Failed to read config, creating new");
        }
    }
    
    // Update or add color value
    const char *elementName = getDisplayElementName(element);

    if(elementName != nullptr){
        doc["colors"][elementName] = colorHEX;
        
        // Save to file
        file = LittleFS.open(configPath, "w");
        if (!file) {
            Serial.println("[FileSystem][ERROR]: Failed to open config for writing");
            return false;
        }
        
        if (serializeJson(doc, file) == 0) {
            Serial.println("[FileSystem][ERROR]: Failed to write config");
            file.close();
            return false;
        }
        
        file.close();
        Serial.printf("[FileSystem]: Saved %s = %s\n", elementName, colorHEX);
    }
    else {
        Serial.println("[FileSystem][ERROR]: Wrong Element Name");
        return false;
    }
    
    return true;
}


const char* FileSystem::loadColorConfig(const DisplayElement element) { 
    File file = LittleFS.open(configPath, "r");
    if (!file) {
        Serial.println("[FileSystem][ERROR]: Config file doesn't exist");
        return "";
    }
    
    StaticJsonDocument<1024> doc;
    DeserializationError error = deserializeJson(doc, file);
    file.close();
    
    if (error) {
        Serial.println("[FileSystem][ERROR]: Failed to parse config");
        return "";
    }
    

    const char *elementName = getDisplayElementName(element);
    
    const char* color = nullptr; 

    if(elementName != nullptr) {
        color = doc["colors"][elementName];
    }

    Serial.printf("[FileSystem]: Element {%s}: {%s} color loaded", elementName, color);
    
    return color;
}


bool FileSystem::saveAllColors(JsonDocument& doc) {
    File file = LittleFS.open(configPath, "w");
    if (!file) {
        Serial.println("[FileSystem][ERROR]: Failed to open config for writing");
        return false;
    }
    
    if (serializeJson(doc, file) == 0) {
        Serial.println("[FileSystem][ERROR]: Failed to write config");
        file.close();
        return false;
    }
    
    file.close();
    Serial.println("[FileSystem]: All colors saved");
    return true;
}


bool FileSystem::loadAllColors(JsonDocument& doc) {
    File file = LittleFS.open(configPath, "r");
    if (!file) {
        Serial.println("[FileSystem][ERROR]: Config file doesn't exist");
        return false;
    }
    
    DeserializationError error = deserializeJson(doc, file);
    file.close();
    
    if (error) {
        Serial.println("[FileSystem][ERROR]: Failed to parse config");
        return false;
    }
    
    return true;
}