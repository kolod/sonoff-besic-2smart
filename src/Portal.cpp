// (c) 2022 Oleksandr Kolodkin <alexandr.kolodkin@gmail.com>
// This code is licensed under MIT license (see LICENSE.txt for details)

#include <LittleFS.h>

#include "Portal.h"
#include "Prefs.h"
#include "AsyncJson.h"
#include "ArduinoJson.h"

Portal::Portal() {
    server = new AsyncWebServer(80);

    server->onNotFound([] (AsyncWebServerRequest *request) {
        request->redirect("http://192.168.4.1/index.html");
    });

    server->serveStatic("/", LittleFS, "/portal/").setDefaultFile("index.html");

    server->on("/scan", HTTP_GET, [](AsyncWebServerRequest *request) {
        AsyncResponseStream *response = request->beginResponseStream("application/json");
        DynamicJsonDocument doc(1024);

        int n = WiFi.scanComplete();
        if (n == -2){
            WiFi.scanNetworks(true);
        } else if (n) {
            for (int i = 0; i < n; ++i) doc[WiFi.SSID(i)] = WiFi.encryptionType(i);
            WiFi.scanDelete();
        }

        serializeJson(doc, *response);
        request->send(response);
    });

    server->on("/setwifi", HTTP_GET, [](AsyncWebServerRequest *request) {
        prefs.putString("ssid", request->getParam("ssid")->value());
        prefs.putString("pass", request->getParam("pass")->value());
        ESP.restart();
    });

    server->on("/settings", HTTP_GET, [](AsyncWebServerRequest *request) {
        DynamicJsonDocument doc(1024);
        doc["email"] = prefs.getString("email", "");
        doc["token"] = prefs.getString("token", "");
        doc["host"] = prefs.getString("host", "cloud.2smart.com");
        doc["port"] = prefs.getString("port", "1883");
        doc["product_id"] = prefs.getString("product_id", "1666444669639535");
        doc["device_id"] = prefs.getString("device_id", "");
        doc["firmware"] = prefs.getString("firmware", "1");

        AsyncResponseStream *response = request->beginResponseStream("application/json");
        serializeJson(doc, *response);
        request->send(response);
    });

    server->on("/setcredentials", HTTP_GET, [](AsyncWebServerRequest *request) {
        prefs.putString("email", request->getParam("mail")->value());
        prefs.putString("token", request->getParam("token")->value());
        prefs.putString("host", request->getParam("hostname")->value());
        prefs.putString("brokerPort", request->getParam("port")->value());
        prefs.putString("product_id", request->getParam("product_id")->value());
        prefs.putString("device_id", request->getParam("device_id")->value());
    });

    server->on("/update", HTTP_GET, [](AsyncWebServerRequest *request) {
        auto output = request->getParam("output")->value();
        if (output == "relay1") {
            auto state = request->getParam("state")->value();
            if (state == "0") {
                digitalWrite(12, LOW);
            } else if (state == "1") {
                digitalWrite(12, HIGH);
            }
        }
    });

    server->begin();
}

Portal portal;
