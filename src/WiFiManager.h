// (c) 2022 Oleksandr Kolodkin <alexandr.kolodkin@gmail.com>
// This code is licensed under MIT license (see LICENSE.txt for details)

#include <Arduino.h>
#include <DNSServer.h>

#define WIFI_RECONNECT_DELAY 250 // ms
#define LED_BLINKING_DELEY 10000 // sec
#define LED_PIN 13

class WiFiManager{

private:
    uint32_t mLastReconnectTime;
    uint32_t mLastLedToggleTime;
    DNSServer *dnsServer;

public:
    WiFiManager();
    void reconnect();
    void startAccessPoint();
};

extern WiFiManager manager;
