#pragma once

#include <LittleFS.h>
#include <ArduinoJson.h>
#include <string>
#include "display_manager.h"


class FileSystem
{
private:
    bool mounted = false;
    const char* configPath = "/config.json";
    
    const char* getDisplayElementName(const DisplayElement element);
    
public:
    FileSystem();
    ~FileSystem();

    bool begin(bool formatOnFail = true);

    bool isMounted() const;

    void end();

    bool fileExists(const char* path);

    bool saveColorConfig(const DisplayElement element, const char* colorHEX);

    const char* loadColorConfig(const DisplayElement element);

    bool saveAllColors(JsonDocument& doc);

    bool loadAllColors(JsonDocument& doc);
};

