#pragma once
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
#include "../../../Global/GlobalDataController.h"
#include "NextionConnection.h"

/**
 * @brief Nextion updater methods
 */
class NextionUpdater {
private:
    DebugController *debugController;
    NextionConnection *connectionHandle;

public:
    NextionUpdater(NextionConnection *connectionHandle, DebugController *debugController);
};
