// (c) 2022 Oleksandr Kolodkin <alexandr.kolodkin@gmail.com>
// This code is licensed under MIT license (see LICENSE.txt for details)


#if defined(ESP32)
    #include <WiFi.h>
    #include <AsyncTCP.h>
#elif defined(ESP8266)
    #include <ESP8266WiFi.h>
    #include <ESPAsyncTCP.h>
#endif

#include "WiFiManager.h"
#include "Prefs.h"

WiFiManager::WiFiManager() {
    dnsServer = nullptr;
    mLastLedToggleTime = mLastReconnectTime = millis();
}

void WiFiManager::startAccessPoint() {
    IPAddress apIP(192, 168, 4, 1);

    WiFi.mode(WIFI_AP);
    WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
    WiFi.softAP("Sonoff Basic");

    dnsServer = new DNSServer();
    dnsServer->start(53, "*", WiFi.softAPIP());
}

void WiFiManager::reconnect() {

    if (dnsServer != nullptr) {
        dnsServer->processNextRequest();
        return;
    }

    if (WiFi.status() == WL_CONNECTED) {
        if (dnsServer != nullptr) delete dnsServer;
        digitalWrite(LED_PIN, HIGH);
        return;
    }

    if (WiFi.status() == WL_WRONG_PASSWORD) {
        startAccessPoint();
        return;
    }

    if (prefs.getString("ssid").isEmpty()) {
        startAccessPoint();
        return;
    }

    if ((millis() - mLastReconnectTime) > WIFI_RECONNECT_DELAY) {
        WiFi.mode(WIFI_STA);
        WiFi.begin(prefs.getString("ssid").c_str(), prefs.getString("pass").c_str());
        mLastReconnectTime = millis();
    } else if ((millis() - mLastLedToggleTime) > LED_BLINKING_DELEY) {
        digitalWrite(LED_PIN, !digitalRead(LED_PIN));
        mLastLedToggleTime = millis();
    }
}

WiFiManager manager;
