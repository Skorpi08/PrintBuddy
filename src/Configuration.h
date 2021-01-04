/**
 * PrinterMonitor Configuration file
 */

#pragma once
#include <Arduino.h>
#include <ESP8266WiFi.h>

/**
 * Basic software settings
 */
#define VERSION                     "1.0"
#define HOSTNAME                    "PrintBuddy-" 
#define CONFIG                      "/conf.txt"         // EEProm config file for general settings
#define PRINTERCONFIG               "/pconf.txt"        // EEProm config file for printer settings
#define DEBUG_MODE_ENABLE           true                // true = Enables debug message on terminal | false = disable all debug messages
#define MAX_PRINTERS                9                   // Limit of configurable printers, please not that many printers slow down the system!
#define PRINTER_SYNC_SEC            60                  // Snyc printer when offline or not printing every x seconds
#define PRINTER_SYNC_SEC_PRINTING   20                  // Snyc printer when printing every x seconds
#define SENSOR_SYNC_SEC             60                  // Sync for sensor in seconds

/**
 * @brief ArduinoJSON Max buffer for responses, used for printers and weather
 * ArduinoJSON Assistant: https://arduinojson.org/v6/assistant/
 * This avoid reallocation of internal memory
 */
#define JSON_MAX_BUFFER             2048

/** Maximum failed printer request responses before error! **/
#define MAX_PRINTER_REQ_FAILED      30

//===========================================================================
//============================== MCU config =================================
//===========================================================================

/**
 * MCU Selector
 */
#define USE_ESP8266_NODEMCU

/**
 * OneWire Interface pin
 */
#ifdef USE_ESP8266_NODEMCU
    #define ON_WIRE_PIN                     0
#else
    #define ON_WIRE_PIN                     D3
#endif

/**
 * I2C Interface pins
 */
#ifdef USE_ESP8266_NODEMCU
    #define I2C_SDA_PIN                     4
    #define I2C_SCL_PIN                     5
#else
    #define I2C_SDA_PIN                     D1
    #define I2C_SCL_PIN                     D2
#endif

/**
 * SPI Interface pins
 */
#ifdef USE_ESP8266_NODEMCU
    #define SPI_SCK                         14
    #define SPI_MISO                        12
    #define SPI_MOSI                        13
    #define SPI_CS                          16
#else
    #define SPI_SCK                         D5
    #define SPI_MISO                        D6
    #define SPI_MOSI                        D7
    #define SPI_CS                          D0
#endif

/**
 * LED Settings
 */

// LED will always flash on bootup or Wifi Errors
#define EXTERNAL_LED            LED_BUILTIN
// true = System LED will Flash on Service Calls; false = disabled LED flashing
#define USE_FLASH               true

//===========================================================================
//======================== Display default config ===========================
//===========================================================================



// Display Settings
//#define USE_NEXTION_DISPLAY
#ifdef USE_NEXTION_DISPLAY
    #ifdef USE_ESP8266_NODEMCU
        #define DISPLAY_TX_PIN                  5
        #define DISPLAY_RX_PIN                  4
    #else
        #define DISPLAY_TX_PIN                  D1
        #define DISPLAY_RX_PIN                  D2
    #endif
    #define DISPLAY_BAUDRATE                    9600
#else
    // I2C Address of your Display (usually 0x3c or 0x3d)
    #define DISPLAY_I2C_DISPLAY_ADDRESS         0x3c
    #ifdef USE_ESP8266_NODEMCU
        #define DISPLAY_SDA_PIN                 4
        #define DISPLAY_SCL_PIN                 5
    #else
        #define DISPLAY_SDA_PIN                 D1
        #define DISPLAY_SCL_PIN                 D2
    #endif
  
    // true = pins at top | false = pins at the bottom
    #define DISPLAY_INVERT_DISPLAY              true
    // true = to use the SH1106 display | false = SSD1306 is used by default and is most common
    #define DISPLAY_SH1106                      true
#endif

//===========================================================================
//======================= Webserver default config ==========================
//===========================================================================

// Device will provide a web interface via http://[ip]:[port]/
#define WEBSERVER_ENABLED           true
// The port you can access this device on over HTTP
#define WEBSERVER_PORT              80
// true = require athentication to change configuration settings / false = no auth
#define WEBSERVER_IS_BASIC_AUTH     true
// User account for the Web Interface
#define WEBSERVER_USERNAME          "admin"
// Password for the Web Interface
#define WEBSERVER_PASSWORD          "password"

//===========================================================================
//====================== Time service default config ========================
//===========================================================================

// true = Show Clock when not printing / false = turn off display when not printing
#define DISPLAYCLOCK                true
// Hour offset from GMT for your timezone
#define TIME_UTCOFFSET              1
#define TIME_HASHOFFSET             "MjI4NzAxMzU5"
// 23:00 millitary 24 hour clock
#define TIME_IS_24HOUR              true
// Minutes between resync with time server
#define TIME_RESYNC_MINUTES_DELAY   15

//===========================================================================
//======================== Weather default config ===========================
//===========================================================================

// true = show weather when not printing / false = no weather
#define DISPLAYWEATHER              true
// Your API Key from http://openweathermap.org/
#define WEATHER_APIKEY              ""
// Default City Location (use http://openweathermap.org/find to find city ID)
#define WEATHER_CITYID              0
// false = Imperial and true = Metric
#define WEATHER_METRIC              true
// Languages: ar, bg, ca, cz, de, el, en, fa, fi, fr, gl, hr, hu, it, ja, kr, la, lt, mk, nl, pl, pt, ro, ru, se, sk, sl, es, tr, ua, vi, zh_cn, zh_tw
#define WEATHER_LANGUAGE            "en"
// Pressure at seelevel
#define SEALEVELPRESSURE_HPA        (1013.25)

//===========================================================================
//============================= OTA config ==================================
//===========================================================================

// this will allow you to load firmware to the device over WiFi (see OTA for ESP8266)
#define ENABLE_OTA          true
// Set an OTA password here -- leave blank if you don't want to be prompted for password
#define OTA_Password        ""





//===========================================================================
//=============================== DEBUG =====================================
//===========================================================================
//#define SIMULATE_CLIENTS_PRINTING                       // Simulate all clients printing
