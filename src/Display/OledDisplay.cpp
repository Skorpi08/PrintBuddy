#include "OledDisplay.h"

/**
 * @brief Construct a new Oled Display:: Oled Display object
 * @param typeName 
 * @param oledDisplay 
 * @param globalDataController 
 * @param debugController 
 */
OledDisplay::OledDisplay(String typeName, OLEDDisplay *oledDisplay, GlobalDataController *globalDataController, DebugController *debugController) {
    this->typeName = typeName;
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
    if (this->globalDataController->getDisplaySettings()->invertDisplay) {
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
        this->ui->enableAutoTransition();

        if (this->globalDataController->getDisplaySettings()->invertDisplay) {
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
    this->numPages = 1;

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
        this->numPages = baseFrameCnt;
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
    this->debugController->printLn("Setup active mode");
    this->ui->disableAutoTransition();
    static OledDisplay* obj = this;
    int frameCnt = 0;
    PrinterDataStruct *printerClientSettings = this->globalDataController->getPrinterSettings();

    for(int i=0; i<this->globalDataController->getNumPrinters(); i++) {
        if (printerClientSettings[i].isPrinting) {
            this->frameToPrinterHandle[frameCnt] = i;
            this->frames[frameCnt] = [](OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) { obj->drawPrinterState(display, state, x, y); };
            frameCnt++;
        }
    }

    if (frameCnt > 0) {
        this->numPages = frameCnt;
        this->ui->setFrames(this->frames, frameCnt);
        this->ui->setOverlays(this->overlays, 1);
        if (frameCnt > 1) {
            this->ui->enableAutoTransition();
        }
    } else {
        this->setupFramesForInactiveMode();
    }
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
 * @brief Handles page switch from loading to main when main loop ist one time completed
 */
void OledDisplay::firstLoopCompleted() {
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
    if (this->globalDataController->getDisplaySettings()->invertDisplay) {
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
        
    // Show clock, weather, printer states
    } else if (this->globalDataController->isAnyPrinterPrinting() ||
        this->globalDataController->getClockSettings()->show
        || this->globalDataController->getWeatherSettings()->show
        || (this->globalDataController->getSensorSettings()->showOnDisplay && this->globalDataController->getSensorSettings()->activated)
    ) {
        if (!this->displayOn && this->globalDataController->isAnyPrinterPrinting()) {
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
        }

        if (!this->globalDataController->isAnyPrinterPrinting() && !this->isClockOn) {
            this->debugController->printLn("PrintBuddy is inactive.");
            this->setupFramesForInactiveMode();
            this->isClockOn = true;
        } else if (this->globalDataController->isAnyPrinterPrinting()
            && (this->isClockOn || (this->numPrintersPrinting != this->globalDataController->numPrintersPrinting()) )) {
            this->debugController->printLn("PrintBuddy is active.");
            this->setupFramesForActiveMode();
            this->numPrintersPrinting = this->globalDataController->numPrintersPrinting();
            this->isClockOn = false;
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

/**
 * @brief Draw printer frame
 * 
 * @param display 
 * @param state 
 * @param x 
 * @param y 
 */
void OledDisplay::drawPrinterState(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {
    // Select the correct printer for this frame
    if (this->lastFixedFrame < 0) {
        this->lastFixedFrame = state->currentFrame;
    }
    PrinterDataStruct refPrinter = this->globalDataController->getPrinterSettings()[this->frameToPrinterHandle[state->currentFrame - this->frameToPrinterHandleOffset]];
    if ((x > 0) && (state->frameState == FrameState::IN_TRANSITION)) {
        int nextFrame = state->currentFrame + 1;
        if ((this->numPrintersPrinting + this->frameToPrinterHandleOffset) <= nextFrame) {
            nextFrame = 0;
        }
        if (nextFrame >= this->frameToPrinterHandleOffset) {
            refPrinter = this->globalDataController->getPrinterSettings()[this->frameToPrinterHandle[nextFrame - this->frameToPrinterHandleOffset]];
        }
    }

    // Draw printer state data
    display->setTextAlignment(TEXT_ALIGN_LEFT);
    display->setFont(ArialMT_Plain_10);
    display->drawString(x, 13 + y, String(refPrinter.customName));

    // State
    int yPos = 24 + y;
    display->setTextAlignment(TEXT_ALIGN_RIGHT);
    display->setFont(ArialMT_Plain_24);
    int progressPercent = refPrinter.progressCompletion;
    if (progressPercent > 99) {
        progressPercent = 99;
    }
    display->drawString(display->width() + x, yPos, String(progressPercent) + "%");

    // Time
    display->setFont(ArialMT_Plain_10);
    display->setTextAlignment(TEXT_ALIGN_LEFT);

    int val = refPrinter.progressPrintTimeLeft;
    int hours = this->globalDataController->numberOfHours(val);
    int minutes = this->globalDataController->numberOfMinutes(val);
    int seconds = this->globalDataController->numberOfSeconds(val);
    String timeLeft = this->globalDataController->zeroPad(hours) + ":" + 
        this->globalDataController->zeroPad(minutes) + ":" + 
        this->globalDataController->zeroPad(seconds);
    val = refPrinter.progressPrintTime;
    hours = this->globalDataController->numberOfHours(val);
    minutes = this->globalDataController->numberOfMinutes(val);
    seconds = this->globalDataController->numberOfSeconds(val);
    String timePrint = this->globalDataController->zeroPad(hours) + ":" + 
        this->globalDataController->zeroPad(minutes) + ":" + 
        this->globalDataController->zeroPad(seconds);

    display->drawString(x, yPos, "D: " + timePrint);
    display->drawString(x, yPos + 11, "L: " + timeLeft);

    // Temps
    int blockWidth = display->width() / 2;
    int tOff = 10;
    int splitBlock = (blockWidth - tOff) / 2;
    yPos = display->height() - 12 + y;
    display->setFont(ArialMT_Plain_10);
    display->setTextAlignment(TEXT_ALIGN_RIGHT);
    display->drawRect(x, yPos, blockWidth, 12);
    display->drawRect(x + blockWidth, yPos, blockWidth, 12);

    display->fillRect(x, yPos, tOff, 12);
    display->fillRect(x + splitBlock + tOff, yPos, splitBlock, 12);    
    display->drawString(x + splitBlock + tOff - 2, yPos - 1, String(refPrinter.toolTemp, 0));
    if (refPrinter.bedTemp != 0) {
        display->fillRect(x + blockWidth, yPos, tOff, 12);
        display->fillRect(x + blockWidth + splitBlock + tOff, yPos, splitBlock, 12);
        display->drawString(x + blockWidth + splitBlock + tOff - 2, yPos - 1, String(refPrinter.bedTemp, 0));
    }

    display->setColor(OLEDDISPLAY_COLOR::BLACK);
    display->setTextAlignment(TEXT_ALIGN_LEFT);
    display->drawString(x + splitBlock + tOff + 2, yPos - 1, String(refPrinter.toolTargetTemp, 0));
    display->drawString(x + 2, yPos - 1, "T");
    if (refPrinter.bedTemp != 0) {
        display->drawString(x + blockWidth + splitBlock + tOff + 2, yPos - 1, String(refPrinter.bedTargetTemp, 0));
        display->drawString(x + blockWidth + 2, yPos - 1, "B");
    }

    // Split blocks!
    display->fillRect(x + blockWidth - 1, yPos, 2, 12);

    // Reset settings!
    display->setColor(OLEDDISPLAY_COLOR::WHITE);
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

    // Draw pages blobs
    if (this->numPages > 1) {
        int xPos = 110 - (this->numPages * 6);
        for(int i=0; i<this->numPages; i++) {
            if (i == state->currentFrame) {
                display->fillCircle(xPos, 5, 2);
            } else {
                display->drawCircle(xPos, 5, 2);
            }
            xPos += 6;
        }
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