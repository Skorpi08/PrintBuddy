#pragma once
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "../DataStructs/DisplayDataStruct.h"

/**
 * @brief Functions for all display clients
 */
class BaseDisplayClient {
public:
  virtual String getType();
  virtual void preSetup();
  virtual void postSetup(bool isConfigChange);
  virtual void firstLoopCompleted();;
  virtual void handleUpdate();
  virtual bool isInTransitionMode();

  virtual void flipDisplayUpdate();
  virtual void showBootScreen();
  virtual void showApAccessScreen(String apSsid, String apIp);
  virtual void showWebserverSplashScreen(bool isEnabled);

  virtual bool isUpdateable();
  virtual void updateFirmware();
};
