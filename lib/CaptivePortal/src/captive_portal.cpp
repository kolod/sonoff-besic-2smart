// (c) 2022 Oleksandr Kolodkin <alexandr.kolodkin@gmail.com>
// This code is licensed under MIT license (see LICENSE.txt for details)

#include "captive_portal.h"

CaptivePortal::CaptivePortal() {
    IPAddress apIP(192, 168, 4, 1);

    WiFi.mode(WIFI_AP);
    WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
    WiFi.softAP("Sonoff Basic CaptivePortal");

    dnsServer = new DNSServer();
    dnsServer->start(53, "*", WiFi.softAPIP());

    webServer = new AsyncWebServer(80);
    webServer->serveStatic("/", LittleFS, "/portal/")
        .setDefaultFile("index.html")
        .setFilter([] (AsyncWebServerRequest *request) -> bool {
            return true;
        });

    webServer->onNotFound([] (AsyncWebServerRequest *request) {
        AsyncWebServerResponse *response = request->beginResponse(301);
        response->addHeader("Location", "http://192.168.4.1/index.html");
        request->send(response);
    });

    webServer->on("/update", HTTP_GET, [](AsyncWebServerRequest *request) {
        auto output = request->getParam("output")->value();
        auto state = request->getParam("state")->value();

        Serial.println(output);
        Serial.println(state);

        if (output == "relay1") {
            if (state == "0") {
                Serial.println("Off");
                digitalWrite(12, LOW);
            } else if (state == "1") {
                Serial.println("On");
                digitalWrite(12, HIGH);
            }
        }
    });

    webServer->begin();
}

void CaptivePortal::loop() {
    dnsServer->processNextRequest();
}
