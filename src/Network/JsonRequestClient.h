#pragma once
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <base64.h>
#include "Debug.h"
#include "../Global/DebugController.h"

#define PRINTER_REQUEST_GET     0
#define PRINTER_REQUEST_POST    1

class JsonRequestClient {
private:
    DebugController *debugController;
    String lastError = "";
    static StaticJsonDocument<JSON_MAX_BUFFER> lastJsonDocument;

public:
    JsonRequestClient(DebugController *debugController);
    JsonDocument *requestJson(int requestType, String server, int port, String encodedAuth, String httpPath, String apiPostBody, bool withResponse);
    String getLastError();
    void resetLastError();

private:
    WiFiClient requestWifiClient(int requestType, String server, int port, String encodedAuth, String httpPath, String apiPostBody);
};
