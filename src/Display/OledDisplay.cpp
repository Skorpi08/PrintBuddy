#include "OledDisplay.h"


/**
 * @brief Construct a new Oled Display:: Oled Display object
 * @param oledDisplay 
 * @param globalDataController 
 * @param debugController 
 */
OledDisplay::OledDisplay(OLEDDisplay *oledDisplay, GlobalDataController *globalDataController, DebugController *debugController) {
    this->globalDataController = globalDataController;
    this->debugController = debugController;
    this->oledDisplay = oledDisplay;
    this->ui = new OLEDDisplayUi(oledDisplay);
}

/**
 * @brief Step 1 initialize 
 * This step is called to initialize the display on boot process, before any data is displayed
 */
void OledDisplay::preSetup() {
    this->oledDisplay->init();
    if (this->globalDataController->getSystemSettings()->invertDisplay) {
        this->oledDisplay->flipScreenVertically(); // connections at top of OLED display
    }
    this->oledDisplay->clear();
    this->oledDisplay->display();
}

/**
 * @brief Step 2 initialize
 * This step is called after boot and on configuration changes
 * @param isConfigChange        True when config is changed
 */
void OledDisplay::postSetup(bool isConfigChange) {
    if (!this->isInitialized && isConfigChange) {
        return;
    }

    if (!this->isInitialized) {
        static OledDisplay* obj = this;
        this->overlays[0] = [](OLEDDisplay *display, OLEDDisplayUiState* state) { obj->transitionStateSetter(state); obj->drawInformationHeaderOverlay(display, state); };
        this->setupFramesForInactiveMode();
        this->ui->setFrameAnimation(SLIDE_LEFT);
        this->ui->setTargetFPS(30);
        this->ui->disableAllIndicators();
        this->ui->setFrames(frames, (3));
        this->ui->setOverlays(overlays, 1);
        this->ui->enableAutoTransition();

        if (this->globalDataController->getSystemSettings()->invertDisplay) {
            this->oledDisplay->flipScreenVertically();
        }
    } else  {
        this->displayOn = true;
        this->displayOffEpoch = 0;
        this->inTransition = false;
        this->setupFramesForInactiveMode();
        this->isClockOn = true;
    }
    this->isInitialized = true;
}

/**
 * @brief Setup frames for inactive mode
 */
void OledDisplay::setupFramesForInactiveMode()
{
    this->debugController->printLn("Setup inactive mode");
    static OledDisplay* obj = this;
    
    if (this->globalDataController->getClockSettings()->show) {
        this->baseFrame[0] = [](OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) { obj->transitionStateSetter(state); obj->drawClock(display, state, x, y); };
        this->ui->setFrames(this->baseFrame, 1);
    }
    this->ui->setOverlays(this->overlays, 0);
    this->ui->disableAutoTransition();

    baseFrameCnt = 0;
    if (this->globalDataController->getWeatherSettings()->show && this->globalDataController->getWeatherSettings()->cityId != 0) {
        this->debugController->printLn("inactive mode - with weather");
        this->baseFrame[baseFrameCnt] = [](OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) { obj->transitionStateSetter(state); obj->drawWeatherOutdoor(display, state, x, y); };
        baseFrameCnt++;
    }
    if (this->globalDataController->getSensorSettings()->showOnDisplay && this->globalDataController->getSensorSettings()->activated) {
        this->debugController->printLn("inactive mode - with indoor");
        this->baseFrame[baseFrameCnt] = [](OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) { obj->transitionStateSetter(state); obj->drawWeatherIndoor(display, state, x, y); };
        baseFrameCnt++;
    }

    if (baseFrameCnt > 0) {
        this->ui->setFrames(this->baseFrame, baseFrameCnt);
        this->ui->setOverlays(this->overlays, 1);
        if (baseFrameCnt > 1) {
            this->ui->enableAutoTransition();
        }
    }
}

/**
 * @brief Setup frames for active mode
 */
void OledDisplay::setupFramesForActiveMode() {
    //this->ui->setFrames(this->frames, 3);
    this->ui->setOverlays(this->overlays, 1);


    




    /*this->frames[0] = [](OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) { obj->drawScreen1(display, state, x, y); };
    this->frames[1] = [](OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) { obj->drawScreen2(display, state, x, y); };
    this->frames[2] = [](OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) { obj->drawScreen3(display, state, x, y); };
    
    this->clockOverlay[0] = [](OLEDDisplay *display, OLEDDisplayUiState* state) { obj->drawClockHeaderOverlay(display, state); };
        */



}

/**
 * @brief Check if we are currently in transition mode
 * @return true 
 * @return false 
 */
bool OledDisplay::isInTransitionMode() {
    return this->inTransition;
}

/**
 * @brief Handle update screen
 */
void OledDisplay::handleUpdate() {
    this->checkDisplay();
    this->ui->update();
}

/**
 * @brief Set Display rotation
 */
void OledDisplay::flipDisplayUpdate() {
    this->ui->init();
    if (this->globalDataController->getSystemSettings()->invertDisplay) {
        this->oledDisplay->flipScreenVertically(); // connections at top of OLED display
    }
    this->ui->update();
}

/**
 * @brief Show boot screen
 */
void OledDisplay::showBootScreen() {
    this->drawImage(0, 0, 128, 64, OledLogo);       
    this->oledDisplay->setTextAlignment(TEXT_ALIGN_CENTER);
    this->oledDisplay->setFont(ArialMT_Plain_10);
    this->oledDisplay->drawString(105, 10, "Print");
    this->oledDisplay->drawString(105, 20, "Buddy");
    this->oledDisplay->drawString(105, 46, "V" + this->globalDataController->getSystemSettings()->version);
    this->oledDisplay->display();
}

/**
 * @brief Show AP Mode screen to configure wifi
 * @param apSsid        SSID to connect
 * @param apIp          IP 
 */
void OledDisplay::showApAccessScreen(String apSsid, String apIp) {
    this->oledDisplay->clear();
    this->oledDisplay->setTextAlignment(TEXT_ALIGN_CENTER);
    this->oledDisplay->setFont(ArialMT_Plain_10);
    this->oledDisplay->drawString(64,  0, "Wifi Manager");
    this->oledDisplay->drawString(64, 10, "Please connect to AP");
    this->oledDisplay->drawString(64, 25, ">> " + apSsid + " <<");
    this->oledDisplay->drawString(64, 40, "To setup Wifi connection");
    this->oledDisplay->drawString(64, 50, "open " + apIp);
    this->oledDisplay->display();
}

/**
 * @brief Show Splash screen
 * @param isEnabled     True if webinterface enabled
 */
void OledDisplay::showWebserverSplashScreen(bool isEnabled) {
    if (isEnabled) {
        String webAddress = "http://" + WiFi.localIP().toString() + ":" + String(this->globalDataController->getSystemSettings()->webserverPort) + "/";
        this->debugController->printLn("Use this URL : " + webAddress);
        this->oledDisplay->clear();
        this->oledDisplay->setTextAlignment(TEXT_ALIGN_CENTER);
        this->oledDisplay->setFont(ArialMT_Plain_10);
        this->oledDisplay->drawString(64, 0, "Web Interface On");
        this->oledDisplay->drawString(64, 13, "You May Connect to IP");
        this->oledDisplay->setFont(ArialMT_Plain_16);
        this->oledDisplay->drawString(64, 30, WiFi.localIP().toString());
        this->oledDisplay->drawString(64, 46, "Port: " + String(this->globalDataController->getSystemSettings()->webserverPort));
        this->oledDisplay->display();
    } else {
        this->debugController->printLn("Web Interface is Disabled");
        this->oledDisplay->clear();
        this->oledDisplay->setTextAlignment(TEXT_ALIGN_CENTER);
        this->oledDisplay->setFont(ArialMT_Plain_10);
        this->oledDisplay->drawString(64, 10, "Web Interface is Off");
        this->oledDisplay->drawString(64, 20, "Enable in Configuration.h");
        this->oledDisplay->display(); 
    }
}

/**
 * @brief Switching between states 
 */
void OledDisplay::checkDisplay() {
    //BasePrinterClient *printerClient = this->globalDataController->getPrinterClient();

    if (!this->displayOn && this->globalDataController->getClockSettings()->show) {
        this->enableDisplay(true);
    }

    // Switch all off if we have nothing to display
    if (this->displayOn
        && !this->globalDataController->isAnyPrinterPrinting()
        && !this->globalDataController->getClockSettings()->show
        && !this->globalDataController->getWeatherSettings()->show
        && !this->globalDataController->getSensorSettings()->showOnDisplay
    ) {
        this->oledDisplay->clear();
        this->oledDisplay->display();
        this->oledDisplay->setFont(ArialMT_Plain_16);
        this->oledDisplay->setTextAlignment(TEXT_ALIGN_CENTER);
        this->oledDisplay->setContrast(255); // default is 255
        this->oledDisplay->drawString(64, 5, "Printer Offline\nSleep Mode...");
        this->oledDisplay->display();
        delay(5000);
        this->enableDisplay(false);
        this->debugController->printLn("Printer is offline going down to sleep...");
        return;

    // 
    } else if (!this->displayOn && !this->globalDataController->getClockSettings()->show) {
        /*
        if (printerClient->isOperational()) {
            // Wake the Screen up
            this->enableDisplay(true);
            this->oledDisplay->clear();
            this->oledDisplay->display();
            this->oledDisplay->setFont(ArialMT_Plain_16);
            this->oledDisplay->setTextAlignment(TEXT_ALIGN_CENTER);
            this->oledDisplay->setContrast(255); // default is 255
            this->oledDisplay->drawString(64, 5, "Printer Online\nWake up...");
            this->oledDisplay->display();
            this->debugController->printLn("Printer is online waking up...");
            delay(5000);
            return;
        } */

    // Show clock and weather
    } else if (this->globalDataController->getClockSettings()->show
        || this->globalDataController->getWeatherSettings()->show
        || (this->globalDataController->getSensorSettings()->showOnDisplay && this->globalDataController->getSensorSettings()->activated)
    ) {
        if (!this->globalDataController->isAnyPrinterPrinting() && !this->isClockOn) {
            this->debugController->printLn("PrintBuddy is inactive.");
            this->setupFramesForInactiveMode();
            this->isClockOn = true;
        } else if (this->globalDataController->isAnyPrinterPrinting() && this->isClockOn) {
            this->debugController->printLn("PrintBuddy is active.");
            this->setupFramesForActiveMode();
            isClockOn = false;
        }
    }
}

/**
 * @brief Enable or disable dispay when not in use
 * @param enable 
 */
void OledDisplay::enableDisplay(boolean enable) {
    this->displayOn = enable;
    TimeClient * timeClient = this->globalDataController->getTimeClient();
    if (enable) {
        if (timeClient->getMinutesFromLast(this->displayOffEpoch) >= this->globalDataController->getSystemSettings()->clockWeatherResyncMinutes) {
            // The display has been off longer than the minutes between refresh -- need to get fresh data
            timeClient->resetLastEpoch();
            this->displayOffEpoch = 0;  // reset
        }
        this->oledDisplay->displayOn();
        this->debugController->printLn("Display was turned ON: " + timeClient->getFormattedTime());
    } else {
        this->oledDisplay->displayOff();
        this->debugController->printLn("Display was turned OFF: " + timeClient->getFormattedTime());
        this->displayOffEpoch = timeClient->getLastEpoch();
    }
}





















void OledDisplay::drawScreen1(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {
    //BasePrinterClient *printerClient = this->globalDataController->getPrinterClient();

    //String bed = printerClient->getValueRounded(printerClient->getTempBedActual());
    //String tool = printerClient->getValueRounded(printerClient->getTempToolActual());
    display->setTextAlignment(TEXT_ALIGN_CENTER);
    display->setFont(ArialMT_Plain_16);
    /*if (bed != "0") {
        display->drawString(29 + x, 0 + y, "Tool");
        display->drawString(89 + x, 0 + y, "Bed");
    } else {
        display->drawString(64 + x, 0 + y, "Tool Temp");
    }
    display->setTextAlignment(TEXT_ALIGN_LEFT);
    display->setFont(ArialMT_Plain_24);
    if (bed != "0") {
        display->setTextAlignment(TEXT_ALIGN_LEFT);
        display->drawString(12 + x, 14 + y, tool + "°");
        display->drawString(74 + x, 14 + y, bed + "°");
    } else {
        display->setTextAlignment(TEXT_ALIGN_CENTER);
        display->drawString(64 + x, 14 + y, tool + "°");
    }*/
}

void OledDisplay::drawScreen2(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {
    //BasePrinterClient *printerClient = this->globalDataController->getPrinterClient();

    display->setTextAlignment(TEXT_ALIGN_CENTER);
    display->setFont(ArialMT_Plain_16);

    display->drawString(64 + x, 0 + y, "Time Remaining");
    //display->setTextAlignment(TEXT_ALIGN_LEFT);
    display->setFont(ArialMT_Plain_24);
    /*int val = printerClient->getProgressPrintTimeLeft().toInt();
    int hours = this->globalDataController->numberOfHours(val);
    int minutes = this->globalDataController->numberOfMinutes(val);
    int seconds = this->globalDataController->numberOfSeconds(val);

    String time = this->globalDataController->zeroPad(hours) + ":" + 
        this->globalDataController->zeroPad(minutes) + ":" + 
        this->globalDataController->zeroPad(seconds);
    display->drawString(64 + x, 14 + y, time); */
}

void OledDisplay::drawScreen3(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {
    /*BasePrinterClient *printerClient = this->globalDataController->getPrinterClient();

    display->setTextAlignment(TEXT_ALIGN_CENTER);
    display->setFont(ArialMT_Plain_16);

    display->drawString(64 + x, 0 + y, "Printing Time");
    //display->setTextAlignment(TEXT_ALIGN_LEFT);
    display->setFont(ArialMT_Plain_24);
    int val = printerClient->getProgressPrintTime().toInt();
    int hours = this->globalDataController->numberOfHours(val);
    int minutes = this->globalDataController->numberOfMinutes(val);
    int seconds = this->globalDataController->numberOfSeconds(val);

    String time = this->globalDataController->zeroPad(hours) + ":" + 
        this->globalDataController->zeroPad(minutes) + ":" + 
        this->globalDataController->zeroPad(seconds);
    display->drawString(64 + x, 14 + y, time); */
}






void OledDisplay::drawHeaderOverlay(OLEDDisplay *display, OLEDDisplayUiState* state) {
    TimeClient * timeClient = this->globalDataController->getTimeClient();
    //BasePrinterClient *printerClient = this->globalDataController->getPrinterClient();

    display->setColor(WHITE);
    display->setFont(ArialMT_Plain_16);
    String displayTime = timeClient->getAmPmHours() + ":" + timeClient->getMinutes();
    if (this->globalDataController->getClockSettings()->is24h) {
        displayTime = timeClient->getHours() + ":" + timeClient->getMinutes();
    }
    display->setTextAlignment(TEXT_ALIGN_LEFT);
    display->drawString(0, 48, displayTime);
  
    if (!this->globalDataController->getClockSettings()->is24h) {
        String ampm = timeClient->getAmPm();
        display->setFont(ArialMT_Plain_10);
        display->drawString(39, 54, ampm);
    }

    display->setFont(ArialMT_Plain_16);
    display->setTextAlignment(TEXT_ALIGN_LEFT);
    //String percent = String(printerClient->getProgressCompletion()) + "%";
    //display->drawString(64, 48, percent);
    
    // Draw indicator to show next update
    //int updatePos = (printerClient->getProgressCompletion().toFloat() / float(100)) * 128;
    display->drawRect(0, 41, 128, 6);
    //display->drawHorizontalLine(0, 42, updatePos);
    //display->drawHorizontalLine(0, 43, updatePos);
    //display->drawHorizontalLine(0, 44, updatePos);
    //display->drawHorizontalLine(0, 45, updatePos);
    
    this->drawRssi(display);
}

void OledDisplay::drawClockHeaderOverlay(OLEDDisplay *display, OLEDDisplayUiState* state) {

    TimeClient * timeClient = this->globalDataController->getTimeClient();
    //BasePrinterClient *printerClient = this->globalDataController->getPrinterClient();

    display->setColor(WHITE);
    display->setFont(ArialMT_Plain_16);
    display->setTextAlignment(TEXT_ALIGN_LEFT);
    int printerStateDrawXPos = 0;
    if (!this->globalDataController->getClockSettings()->is24h) {
        display->drawString(0, 48, timeClient->getAmPm());
        display->setTextAlignment(TEXT_ALIGN_CENTER);
        printerStateDrawXPos = 64;
    }
    /*if (printerClient->isPSUoff()) {
        display->drawString(printerStateDrawXPos, 47, "psu off");
    } else if (printerClient->getState() != PRINTER_STATE_OFFLINE) {
        display->drawString(printerStateDrawXPos, 47, "online");
    } else {
        display->drawString(printerStateDrawXPos, 47, "offline");
    }*/
    display->setTextAlignment(TEXT_ALIGN_LEFT);
    display->drawRect(0, 43, 128, 2);
    
    this->drawRssi(display);
}






















/**
 * @brief Draw clock frame (if we only show clock in pause mode)
 * 
 * @param display 
 * @param state 
 * @param x 
 * @param y 
 */
void OledDisplay::drawClock(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {
    TimeClient * timeClient = this->globalDataController->getTimeClient();
    display->setTextAlignment(TEXT_ALIGN_CENTER);
    String displayTime = timeClient->getAmPmHours() + ":" + timeClient->getMinutes();
    if (this->globalDataController->getClockSettings()->is24h) {
        displayTime = timeClient->getHours() + ":" + timeClient->getMinutes(); 
    }
    display->setFont(ArialMT_Plain_24);
    display->drawString(64 + x, 20 + y, displayTime);
    this->drawRssi(display);
}

/**
 * @brief Draw outdoor weather
 * @param display 
 * @param state 
 * @param x 
 * @param y 
 */
void OledDisplay::drawWeatherOutdoor(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {
    OpenWeatherMapClient *weatherClient = this->globalDataController->getWeatherClient();

    display->setTextAlignment(TEXT_ALIGN_LEFT);
    display->setFont(ArialMT_Plain_10);
    display->drawString(0 + x, 13 + y, weatherClient->getCity(0));

    display->setTextAlignment(TEXT_ALIGN_LEFT);
    display->setFont(ArialMT_Plain_24);
    display->drawString(0 + x, 24 + y, weatherClient->getTempRounded(0) + weatherClient->getTempSymbol());

    display->setFont((const uint8_t*)Meteocons_Plain_42);
    display->drawString(84 + x, 14 + y, weatherClient->getWeatherIcon(0));

    display->setTextAlignment(TEXT_ALIGN_LEFT);
    display->setFont(ArialMT_Plain_10);
    display->drawString(0 + x, 50 + y, weatherClient->getCondition(0));
}

/**
 * @brief Draw indoor sensor weather
 * @param display 
 * @param state 
 * @param x 
 * @param y 
 */
void OledDisplay::drawWeatherIndoor(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {
    SensorDataStruct *sensorSettings = this->globalDataController->getSensorSettings();
    BaseSensorClient *sensorClient = this->globalDataController->getSensorClient(sensorSettings);

    display->setTextAlignment(TEXT_ALIGN_LEFT);
    display->setFont(ArialMT_Plain_10);
    display->drawString(0 + x, 13 + y, "Indoor");

    display->setFont(ArialMT_Plain_24);
    display->setTextAlignment(TEXT_ALIGN_LEFT);
    display->drawString(0 + x, 24 + y, sensorClient->getTemperatureRounded(sensorSettings)  + "°C");

    if (sensorClient->hasHumidity()) {
        display->setTextAlignment(TEXT_ALIGN_RIGHT);
        display->drawString(126 + x, 24 + y, sensorClient->getHumidityRounded(sensorSettings)  + "%");
    }

    if (sensorClient->hasAirQuality()) {
        display->setTextAlignment(TEXT_ALIGN_LEFT);
        display->setFont(ArialMT_Plain_10);
        display->drawString(0 + x, 50 + y, "Air quality: " + sensorClient->airQualityAsString(sensorSettings));
    }   
}

/**
 * @brief Draw basic information header (time and rssi) on the first 11px from top
 * @param display 
 * @param state 
 */
void OledDisplay::drawInformationHeaderOverlay(OLEDDisplay *display, OLEDDisplayUiState* state) {
    TimeClient * timeClient = this->globalDataController->getTimeClient();
    display->setColor(WHITE);
    display->setFont(ArialMT_Plain_10);  
    
    if (!this->globalDataController->getClockSettings()->is24h) {
        String displayTime = timeClient->getAmPmHours() + ":" + timeClient->getMinutes();
        display->setTextAlignment(TEXT_ALIGN_RIGHT);
        display->drawString(25, 0, displayTime);
        String ampm = timeClient->getAmPm();
        display->setTextAlignment(TEXT_ALIGN_LEFT);
        display->drawString(27, 0, ampm);
    } else {
        String displayTime = timeClient->getHours() + ":" + timeClient->getMinutes();
        display->setTextAlignment(TEXT_ALIGN_LEFT);
        display->drawString(0, 0, displayTime);
    }
    display->drawHorizontalLine(0, 11, 128);
    this->drawRssi(display);
}


/**
 * @brief Draw an image (internal function useless, wrong bit ordering)
 * 
 * @param xMove             X start position
 * @param yMove             Y start position
 * @param width             Image width
 * @param height            Image height
 * @param xbm               Image
 */
void OledDisplay::drawImage(int16_t xMove, int16_t yMove, int16_t width, int16_t height, const uint8_t *xbm) {
    int16_t widthInXbm = (width + 7) / 8;
    uint8_t data = 0;
    uint8_t dataOrg = 0;
    int shift = 7;
    for(int16_t y = 0; y < height; y++) {
        for(int16_t x = 0; x < width; x++ ) {
            if (x & 7) {
                shift--;
                data = dataOrg >> shift;
            } else {
                dataOrg = pgm_read_byte(xbm + (x / 8) + y * widthInXbm);
                shift = 7;
                data = dataOrg >> shift;
            }
            if (data & 0x01) {
                this->oledDisplay->setPixel(xMove + x, yMove + y);
            }
        }
    }
}

/**
 * @brief Subfunction to draw WiFi Rssi
 * @param display           Handle to display
 */
void OledDisplay::drawRssi(OLEDDisplay *display) { 
    int8_t quality = EspController::getWifiQuality();
    for (int8_t i = 0; i < 4; i++) {
        for (int8_t j = 0; j < 2 * (i + 2); j++) {
            if (quality > i * 25 || j == 0) {
                display->setPixel(113 + 4 * i, 9 - j);
            }
        }
    }
}

/**
 * @brief Check if we are in transition mode
 * @param state 
 */
void OledDisplay::transitionStateSetter(OLEDDisplayUiState* state) {
    this->inTransition = state->frameState == FrameState::IN_TRANSITION;
}