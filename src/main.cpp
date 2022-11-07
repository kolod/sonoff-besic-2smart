// (c) 2022 Oleksandr Kolodkin <alexandr.kolodkin@gmail.com>
// This code is licensed under MIT license (see LICENSE.txt for details)

#include <Arduino.h>
#include "captive_portal.h"

CaptivePortal *portal = nullptr;

void setup() {
	pinMode(12, OUTPUT);      // relay
	pinMode(0, INPUT_PULLUP); // button

	Serial.begin(115200);

	if(!LittleFS.begin()){
		Serial.println("An Error has occurred while mounting LittleFS");
		return;
	}

	portal = new CaptivePortal();
}

void loop() {
	if (portal != nullptr) portal->loop();
}
