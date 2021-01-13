#pragma once
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
#include "../Global/GlobalDataController.h"
#include "BaseDisplayClient.h"
#include "Extras/Nextion/NextionConnection.h"

class NextionDisplay : public BaseDisplayClient {
private:
    GlobalDataController *globalDataController;
    DebugController *debugController;
    NextionConnection nextionConnection;
    boolean displayOn = true;
    long    displayOffEpoch = 0;
    long    lastSyncEpochBasic = 0;
    long    lastSyncEpochExtended = 0;
    int     lastActivePrinters = 0;
    
public:
    NextionDisplay(SoftwareSerial *serialPort, GlobalDataController *globalDataController, DebugController *debugController);
    void preSetup();
    void postSetup(bool isConfigChange);
    void firstLoopCompleted();
    void handleUpdate();
    void flipDisplayUpdate();
    void showBootScreen();
    void showApAccessScreen(String apSsid, String apIp);
    void showWebserverSplashScreen(bool isEnabled);

    void syncStateData();
    void syncWeatherData();
    void syncSettingsData();
    void syncPrintersData();
    String getWeatherIconShortId();

    String getType() { return "Nextion NX4832K035"; };
    bool isInTransitionMode() { return false; };
    bool isUpdateable() { return true; };
    void updateFirmware() {};
};
