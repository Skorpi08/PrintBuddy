#pragma once
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "../Global/GlobalDataController.h"
#include <OLEDDisplayUi.h>
#include <OLEDDisplay.h>
#include "../../include/OledLogo.h"
#include "../../include/WeatherStationFonts.h"
#include "BaseDisplayClient.h"

class OledDisplay : public BaseDisplayClient {
private:
    GlobalDataController *globalDataController;
    DebugController *debugController;
    OLEDDisplay *oledDisplay;
    OLEDDisplayUi *ui;
    int numPages = 0;
    boolean displayOn = true;
    long displayOffEpoch = 0;
    bool inTransition = false;
    bool isInitialized = false;

    OverlayCallback overlays[1];
    FrameCallback baseFrame[2];
    int baseFrameCnt = 0;



    FrameCallback frames[3];
    
    boolean isClockOn = false;
    
    OverlayCallback clockOverlay[1];

public:
    OledDisplay(OLEDDisplay *oledDisplay, GlobalDataController *globalDataController, DebugController *debugController);
    void preSetup();
    void postSetup(bool isConfigChange);
    void handleUpdate();
    void flipDisplayUpdate();
    void showBootScreen();
    void showApAccessScreen(String apSsid, String apIp);
    void showWebserverSplashScreen(bool isEnabled);
    bool isInTransitionMode();


    void checkDisplay();
    void enableDisplay(boolean enable);
    void drawScreen1(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y);
    void drawScreen2(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y);
    void drawScreen3(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y);
    void drawClock(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y);
    
    void drawHeaderOverlay(OLEDDisplay *display, OLEDDisplayUiState* state);
    void drawClockHeaderOverlay(OLEDDisplay *display, OLEDDisplayUiState* state);
    void drawRssi(OLEDDisplay *display);


    void drawWeatherOutdoor(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y);
    void drawWeatherIndoor(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y);
    void drawInformationHeaderOverlay(OLEDDisplay *display, OLEDDisplayUiState* state);

private:
    void setupFramesForInactiveMode();
    void setupFramesForActiveMode();
    void drawImage(int16_t xMove, int16_t yMove, int16_t width, int16_t height, const uint8_t *xbm);
    void transitionStateSetter(OLEDDisplayUiState* state);
};