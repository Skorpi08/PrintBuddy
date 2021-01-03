#pragma once
#include <Arduino.h>
#include <ESP8266WiFi.h>

class BaseDisplayClient {
public:
  virtual void preSetup();
  virtual void postSetup(bool isConfigChange);
  virtual void handleUpdate();
  virtual bool isInTransitionMode();

  
  virtual void flipDisplayUpdate();
  virtual void showBootScreen();
  virtual void showApAccessScreen(String apSsid, String apIp);
  virtual void showWebserverSplashScreen(bool isEnabled);
};
