// (c) 2022 Oleksandr Kolodkin <alexandr.kolodkin@gmail.com>
// This code is licensed under MIT license (see LICENSE.txt for details)

#include <Arduino.h>
#include <LittleFS.h>

#include "Portal.h"
#include "WiFiManager.h"

void setup() {
	pinMode(0, INPUT_PULLUP); // button
	pinMode(12, OUTPUT);      // relay
	pinMode(13, OUTPUT);      // LED

	Serial.begin(115200);

	if (!LittleFS.begin()) {
		Serial.println("An Error has occurred while mounting LittleFS");
	}
}

void loop() {
	manager.reconnect();
}
