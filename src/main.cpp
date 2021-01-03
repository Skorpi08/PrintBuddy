#include "Includes.h"
#include <ArduinoOTA.h>

String lastMinute = "xx";
String lastSecond = "xx";

void configModeCallback(WiFiManager *myWiFiManager);
void handleSubroutineLoop();

/**
 * @brief Setup/Initialize ESP
 */
void setup() {
    // Init defaults and reset data
    LittleFS.begin();
    debugController.setup();

    // Init interfaces (SPI & I2C)
    i2cInterface->begin(I2C_SDA_PIN, I2C_SCL_PIN);
    SPI.pins(SPI_SCK, SPI_MISO, SPI_MOSI, SPI_CS);

    // Init all instances (Printers Clients)
    globalDataController.registerPrinterClient(PRINTER_CLIENT_REPETIER, &printerClient3);
    globalDataController.registerPrinterClient(PRINTER_CLIENT_OCTOPRINT, &printerClient2);
    globalDataController.registerPrinterClient(PRINTER_CLIENT_KLIPPER, &printerClient1);
    globalDataController.registerPrinterClient(PRINTER_CLIENT_DUET, &printerClient0);

    // Init all instances (Sensor Clients)
    globalDataController.registerSensorClient(SENSOR_CLIENT_BME280_I2C, &sensorClient0, i2cInterface, &SPI, SPI_CS, ON_WIRE_PIN);
    globalDataController.registerSensorClient(SENSOR_CLIENT_BME280_SPI, &sensorClient1, i2cInterface, &SPI, SPI_CS, ON_WIRE_PIN);
    globalDataController.registerSensorClient(SENSOR_CLIENT_BME680_I2C, &sensorClient2, i2cInterface, &SPI, SPI_CS, ON_WIRE_PIN);
    globalDataController.registerSensorClient(SENSOR_CLIENT_BME680_SPI, &sensorClient3, i2cInterface, &SPI, SPI_CS, ON_WIRE_PIN);
    globalDataController.registerSensorClient(SENSOR_CLIENT_HTU21D_I2C, &sensorClient4, i2cInterface, &SPI, SPI_CS, ON_WIRE_PIN);
    globalDataController.registerSensorClient(SENSOR_CLIENT_BMP085_180_I2C, &sensorClient5, i2cInterface, &SPI, SPI_CS, ON_WIRE_PIN);
    globalDataController.registerSensorClient(SENSOR_CLIENT_DHT11_WIRE, &sensorClient6, i2cInterface, &SPI, SPI_CS, ON_WIRE_PIN);
    globalDataController.registerSensorClient(SENSOR_CLIENT_DHT21_WIRE, &sensorClient7, i2cInterface, &SPI, SPI_CS, ON_WIRE_PIN);
    globalDataController.registerSensorClient(SENSOR_CLIENT_DHT22_WIRE, &sensorClient8, i2cInterface, &SPI, SPI_CS, ON_WIRE_PIN);

    // Init all instances (Displays)
    globalDataController.setDisplayClient(&displayClient);

    // Init 
    globalDataController.setup();
    displayClient.preSetup();
    displayClient.showBootScreen();

    // WiFiManager - Local intialization. Once its business is done, there is no need to keep it around
    WiFiManager wifiManager;
    wifiManager.setDebugOutput(DEBUG_MODE_ENABLE);
    //wifiManager.resetSettings();    // Uncomment for testing wifi manager
    wifiManager.setAPCallback(configModeCallback);
    String hostname(HOSTNAME);
    hostname += String(ESP.getChipId(), HEX);
    WiFi.hostname(hostname);
    if (!wifiManager.autoConnect((const char *)hostname.c_str())) { // new addition
        delay(3000);
        WiFi.disconnect(true);
        ESP.reset();
        delay(5000);
    }
    
    displayClient.postSetup(false);

    // print the received signal strength:
    debugController.print("Signal Strength (RSSI): ");
    debugController.print(EspController::getWifiQuality());
    debugController.printLn("%");

    if (ENABLE_OTA) {
        ArduinoOTA.onStart([]() {
            debugController.printLn("Start");
        });
        ArduinoOTA.onEnd([]() {
            debugController.printLn("\nEnd");
        });
        ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
            debugController.printF("Progress: %u%%\r", (progress / (total / 100)));
        });
        ArduinoOTA.onError([](ota_error_t error) {
            debugController.printF("Error[%u]: ", error);
            if (error == OTA_AUTH_ERROR) debugController.printLn("Auth Failed");
            else if (error == OTA_BEGIN_ERROR) debugController.printLn("Begin Failed");
            else if (error == OTA_CONNECT_ERROR) debugController.printLn("Connect Failed");
            else if (error == OTA_RECEIVE_ERROR) debugController.printLn("Receive Failed");
            else if (error == OTA_END_ERROR) debugController.printLn("End Failed");
        });
        ArduinoOTA.setHostname((const char *)hostname.c_str()); 
        if (String(OTA_Password) != "") {
            ArduinoOTA.setPassword((OTA_Password));
        }
        ArduinoOTA.begin();
    }

    debugController.printLn("local ip");
    debugController.printLn(WiFi.localIP().toString());

#if WEBSERVER_ENABLED
    webServer.setup();
    displayClient.showWebserverSplashScreen(true);
#else
    displayClient.showWebserverSplashScreen(false);
#endif

    globalDataController.flashLED(5, 100);
    webServer.findMDNS();
    debugController.printLn("*** Leaving setup()");
}

/**
 * @brief Loop trough all
 */
void loop() {
    if (displayClient.isInTransitionMode()) {
        handleSubroutineLoop();
        return;
    }

    // Handle update of time
    if(timeClient.handleSync(globalDataController.getSystemSettings()->clockWeatherResyncMinutes) 
        && globalDataController.getWeatherSettings()->show
    ) {
        // We sync time? Ok, sync weather also!
        debugController.printLn("Updating weather...");
        weatherClient.updateWeather();
        handleSubroutineLoop();
    }

    // Sensor update?
    if (globalDataController.getSensorSettings()->activated
         && (timeClient.getSecondsFromLast(globalDataController.getSensorSettings()->lastSyncEpoch) > SENSOR_SYNC_SEC)
    ) {
        debugController.printLn("Sync sensor...");
        globalDataController.getSensorSettings()->lastSyncEpoch = timeClient.getCurrentEpoch();
        globalDataController.syncSensor();
        handleSubroutineLoop();
    }

    // Sync only if we have printers
    if (globalDataController.getNumPrinters() > 0) {
        PrinterDataStruct *presetPrinters = globalDataController.getPrinterSettings();
        long cEpoch = timeClient.getCurrentEpoch();
        for(int i=0; i<globalDataController.getNumPrinters(); i++) {
            int secFromLastSyn = -1;
            if (presetPrinters[i].lastSyncEpoch > 0) {
                secFromLastSyn = timeClient.getSecondsFromLast(presetPrinters[i].lastSyncEpoch);
            }
            if ((secFromLastSyn < 0) ||
                (!presetPrinters[i].isPrinting && (secFromLastSyn >= PRINTER_SYNC_SEC)) ||
                (presetPrinters[i].isPrinting && (secFromLastSyn >= PRINTER_SYNC_SEC_PRINTING))
             ) {
                globalDataController.ledOnOff(true);
                presetPrinters[i].lastSyncEpoch = cEpoch;
                globalDataController.syncPrinter(&presetPrinters[i]);
                globalDataController.ledOnOff(false);
            }
            // Handle some web between the printers to avoid blocking!
            handleSubroutineLoop();
        }
    }
    handleSubroutineLoop();
}

/**
 * @brief Functions to avoid permantent blocking between longer routines
 */
void handleSubroutineLoop() {
    // Handle Display
    displayClient.handleUpdate();

    // Handle all Web stuff
    if (WEBSERVER_ENABLED) {
        webServer.handleClient();
    }
    if (ENABLE_OTA) {
        ArduinoOTA.handle();
    }
}

/**
 * @brief AP Mode for WiFi configuration
 * @param myWiFiManager 
 */
void configModeCallback(WiFiManager *myWiFiManager) {
    debugController.printLn("Entered config mode");
    debugController.printLn(WiFi.softAPIP().toString());
    displayClient.showApAccessScreen(myWiFiManager->getConfigPortalSSID(), WiFi.softAPIP().toString());
    debugController.printLn("Wifi Manager");
    debugController.printLn("Please connect to AP");
    debugController.printLn(myWiFiManager->getConfigPortalSSID());
    debugController.printLn("To setup Wifi Configuration");
    globalDataController.flashLED(20, 50);
}
