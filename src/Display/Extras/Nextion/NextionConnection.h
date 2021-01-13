#pragma once
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
#include "../../../Global/GlobalDataController.h"

/**
 * @brief Nextion connection base methods
 */
class NextionConnection {
private:
    DebugController *debugController;
    SoftwareSerial *serialPort;

public:
    NextionConnection(SoftwareSerial *serialPort, DebugController *debugController);
    void setBaudrate(int baudrate);
    void resetDevice();
    void switchToPage(int pageId);
    void sendCommandValueTxt(String var, String value);
    void sendCommandValueInt(String var, int value);
    void sendCommand(String cmd);
    void sendCommand(const char* cmd);
};
