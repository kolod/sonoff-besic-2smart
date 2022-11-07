// (c) 2022 Oleksandr Kolodkin <alexandr.kolodkin@gmail.com>
// This code is licensed under MIT license (see LICENSE.txt for details)

#ifdef ESP32
    #include <WiFi.h>
    #include <AsyncTCP.h>
#elif defined(ESP8266)
    #include <ESP8266WiFi.h>
    #include <ESPAsyncTCP.h>
#endif
#include "ESPAsyncWebServer.h"
#include "LittleFS.h"
#include <DNSServer.h>

class CaptivePortal {
private:
    DNSServer *dnsServer;
    AsyncWebServer *webServer;

public:
    CaptivePortal();
    void loop();
};
