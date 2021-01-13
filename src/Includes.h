#pragma once
#include <Arduino.h>
#include <WiFiManager.h>
#include <SPI.h>
#include "Global/GlobalDataController.h"
#include "Global/DebugController.h"
#include "Configuration.h"
#include "Network/WebServer.h"
#include "Network/TimeClient.h"
#include "Network/OpenWeatherMapClient.h"
#include "Network/JsonRequestClient.h"
#include "Clients/RepetierClient.h"
#include "Clients/KlipperClient.h"
#include "Clients/DuetClient.h"
#include "Clients/OctoPrintClient.h"

#include "Sensors/BME280SensorI2C.h"
#include "Sensors/BME280SensorSPI.h"
#include "Sensors/BME680SensorI2C.h"
#include "Sensors/BME680SensorSPI.h"
#include "Sensors/HTU21DI2C.h"
#include "Sensors/BMP180I2C.h"
#include "Sensors/DHT11Wire.h"
#include "Sensors/DHT21Wire.h"
#include "Sensors/DHT22Wire.h"

#include "Display/NextionDisplay.h"
#include <SSD1306Wire.h>
#include <SH1106Wire.h>
#include "Display/OledDisplay.h"

// Initilize all needed data
DebugController debugController(DEBUG_MODE_ENABLE);
JsonRequestClient jsonRequestClient(&debugController);
TimeClient timeClient(TIME_UTCOFFSET, &debugController);
OpenWeatherMapClient weatherClient(WEATHER_APIKEY, WEATHER_CITYID, 1, WEATHER_METRIC, WEATHER_LANGUAGE, &debugController, &jsonRequestClient);
GlobalDataController globalDataController(&timeClient, &weatherClient, &debugController);
WebServer webServer(&globalDataController, &debugController);

// Register all printer clients
DuetClient printerClient0(&globalDataController, &debugController, &jsonRequestClient);
KlipperClient printerClient1(&globalDataController, &debugController, &jsonRequestClient);
OctoPrintClient printerClient2(&globalDataController, &debugController, &jsonRequestClient);
RepetierClient printerClient3(&globalDataController, &debugController, &jsonRequestClient);

// Register all sensor types
BME280SensorI2C sensorClient0(&globalDataController, &debugController, &jsonRequestClient);
BME280SensorSPI sensorClient1(&globalDataController, &debugController, &jsonRequestClient);
BME680SensorI2C sensorClient2(&globalDataController, &debugController, &jsonRequestClient);
BME680SensorSPI sensorClient3(&globalDataController, &debugController, &jsonRequestClient);
HTU21DI2C sensorClient4(&globalDataController, &debugController, &jsonRequestClient);
BMP180I2C sensorClient5(&globalDataController, &debugController, &jsonRequestClient);
DHT11Wire sensorClient6(&globalDataController, &debugController, &jsonRequestClient);
DHT21Wire sensorClient7(&globalDataController, &debugController, &jsonRequestClient);
DHT22Wire sensorClient8(&globalDataController, &debugController, &jsonRequestClient);

// Initialize I2C (SPI in main routine)
TwoWire *i2cInterface = &Wire;

// Construct correct display client
SoftwareSerial displaySerialPort(DISPLAY_RX_PIN, DISPLAY_TX_PIN);
SH1106Wire  displaySH1106(DISPLAY_I2C_DISPLAY_ADDRESS, DISPLAY_SDA_PIN, DISPLAY_SCL_PIN);
SSD1306Wire displaySSD1306(DISPLAY_I2C_DISPLAY_ADDRESS, DISPLAY_SDA_PIN, DISPLAY_SCL_PIN);
NextionDisplay displayClient1(&displaySerialPort, &globalDataController, &debugController);
OledDisplay displayClient2("OLED SH1106", &displaySH1106, &globalDataController, &debugController);
OledDisplay displayClient3("OLED SSD1306", &displaySSD1306, &globalDataController, &debugController);