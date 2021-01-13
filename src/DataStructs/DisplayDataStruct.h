#pragma once
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <base64.h>

#define DISPLAY_CLIENT_NEXTION_4832K035   (int)0
#define DISPLAY_CLIENT_OLED_SH1106        (int)1
#define DISPLAY_CLIENT_OLED_SSD1306       (int)2

typedef struct {
    int     displayType;
    bool    invertDisplay;
    bool    showWeatherSensorSplited;
    int     automaticSwitchDelay;
    bool    automaticSwitchEnabled;
    bool    automaticSwitchActiveOnlyEnabled;
    int     automaticInactiveOff;
} DisplayDataStruct;
