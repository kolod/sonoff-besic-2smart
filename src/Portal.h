// (c) 2022 Oleksandr Kolodkin <alexandr.kolodkin@gmail.com>
// This code is licensed under MIT license (see LICENSE.txt for details)

#include "ESPAsyncWebServer.h"

class Portal {

private:
    AsyncWebServer *server;

public:
    Portal();
};

extern Portal portal;
