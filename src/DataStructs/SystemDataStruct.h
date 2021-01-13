#pragma once
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <base64.h>

typedef struct {
    bool    useLedFlash;
    int     webserverPort;
    String  webserverUsername;
    String  webserverPassword;
    bool    hasBasicAuth;
    int     clockWeatherResyncMinutes;
    String  version;
    String  lastError;
    String  lastOk;
} SystemDataStruct;
