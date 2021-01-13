#include "NextionDisplay.h"

/**
 * @brief Construct a new Nextion Display:: Nextion Displayobject
 * @param serialPort 
 * @param globalDataController 
 * @param debugController 
 */
NextionDisplay::NextionDisplay(SoftwareSerial *serialPort, GlobalDataController *globalDataController, DebugController *debugController)
: nextionConnection(serialPort, debugController) {
    this->debugController = debugController;
    this->globalDataController = globalDataController;
}

/**
 * @brief Step 1 initialize 
 * This step is called to initialize the display on boot process, before any data is displayed
 */
void NextionDisplay::preSetup() {
#ifdef DISPLAY_BAUDRATE
    this->nextionConnection.setBaudrate(DISPLAY_BAUDRATE);
#endif
    this->nextionConnection.resetDevice();
}

/**
 * @brief Step 2 initialize
 * This step is called after boot and on configuration changes
 * @param isConfigChange        True when config is changed
 */
void NextionDisplay::postSetup(bool isConfigChange) {
    DisplayDataStruct *displaySettings = this->globalDataController->getDisplaySettings();

    this->nextionConnection.sendCommandValueInt("showPrinter", 1);
    this->nextionConnection.sendCommandValueInt("showWeatherSensorSplited", displaySettings->showWeatherSensorSplited ? 1 : 0);
    this->nextionConnection.sendCommandValueInt("autoPrinterSwitchDelay", displaySettings->automaticSwitchDelay);
    this->nextionConnection.sendCommandValueInt("autoPrinterSwitchEnable", displaySettings->automaticSwitchEnabled ? 1 : 0);
    this->nextionConnection.sendCommandValueInt("autoPrinterSwitchActiveEnable", displaySettings->automaticSwitchActiveOnlyEnabled ? 1 : 0);
    if (isConfigChange) {
        if ((displaySettings->automaticInactiveOff > 0) && (this->lastActivePrinters == 0)) {
            this->nextionConnection.sendCommandValueInt("thup", 1);
            this->nextionConnection.sendCommandValueInt("thsp", displaySettings->automaticInactiveOff);
        } else {
            this->nextionConnection.sendCommandValueInt("sleep", 0);
            this->nextionConnection.sendCommandValueInt("thup", 1);
            this->nextionConnection.sendCommandValueInt("thsp", 0);
            this->nextionConnection.sendCommandValueInt("sleep", 0);
        }
    }
    if (this->globalDataController->getWeatherSettings()->show && this->globalDataController->getWeatherSettings()->cityId != 0) {
        this->nextionConnection.sendCommandValueInt("hasWeather", 1);
    } else {
        this->nextionConnection.sendCommandValueInt("hasWeather", 0);
    }
    if (this->globalDataController->getSensorSettings()->showOnDisplay && this->globalDataController->getSensorSettings()->activated) {
        this->nextionConnection.sendCommandValueInt("hasSensor", 1);
    } else {
        this->nextionConnection.sendCommandValueInt("hasSensor", 0);
    }
    if (isConfigChange) {
        this->nextionConnection.resetDevice();
        this->nextionConnection.switchToPage(4);
    }
}

/**
 * @brief Handles page switch from loading to main when main loop ist one time completed
 */
void NextionDisplay::firstLoopCompleted() {
    this->nextionConnection.switchToPage(4);
    DisplayDataStruct *displaySettings = this->globalDataController->getDisplaySettings();
    if (displaySettings->automaticInactiveOff > 0) {
        this->lastActivePrinters = 0;
        this->nextionConnection.sendCommandValueInt("thup", 1);
        this->nextionConnection.sendCommandValueInt("thsp", displaySettings->automaticInactiveOff);
    }
    this->handleUpdate();
}

/**
 * @brief Handle update screen
 */
void NextionDisplay::handleUpdate() {
    TimeClient *timeClient = this->globalDataController->getTimeClient();

    // Resync basic data every 10s
    if(timeClient->getSecondsFromLast(this->lastSyncEpochBasic) > 10) {
        this->syncSettingsData();
        this->syncStateData();
        this->syncWeatherData();
        this->lastSyncEpochBasic = timeClient->getLastEpoch();
    }

    // Resync extended data every 2s
    if(timeClient->getSecondsFromLast(this->lastSyncEpochExtended) > 2) {
        this->syncPrintersData();
        this->lastSyncEpochExtended = timeClient->getLastEpoch();
    }
}

/**
 * @brief Set Display rotation (unsupported on nextion)
 */
void NextionDisplay::flipDisplayUpdate() {
}

/**
 * @brief Show bootscreen with version
 */
void NextionDisplay::showBootScreen() {
    this->nextionConnection.switchToPage(0);
    this->nextionConnection.sendCommandValueTxt("vars.PrintBuddyVer.txt", "V" + this->globalDataController->getSystemSettings()->version);
}

/**
 * @brief We need to configure WiFi
 * 
 * @param apSsid        AccessPoint ssid
 * @param apIp          IP of this device
 */
void NextionDisplay::showApAccessScreen(String apSsid, String apIp) {
    this->nextionConnection.sendCommandValueTxt("vars.WifiApHostName.txt", apSsid);
    String webAddress = "http://" + apIp + "/";
    this->nextionConnection.sendCommandValueTxt("vars.WifiServer.txt", webAddress);
    this->nextionConnection.switchToPage(1);
}

/**
 * @brief We are connected to WiFi, show data for config webinterface
 * @param isEnabled 
 */
void NextionDisplay::showWebserverSplashScreen(bool isEnabled) {
    if(isEnabled) {
        String webAddress = "http://" + WiFi.localIP().toString();
        if (this->globalDataController->getSystemSettings()->webserverPort!= 80) {
            webAddress += ":" + String(this->globalDataController->getSystemSettings()->webserverPort);
        }
        webAddress += "/";
         
        this->debugController->printLn("Use this URL : " + webAddress);
        this->nextionConnection.sendCommandValueTxt("vars.WifiAp.txt", WiFi.SSID());
        this->nextionConnection.sendCommandValueTxt("vars.WifiServer.txt", webAddress);
        this->nextionConnection.switchToPage(2);
    } else {
        this->debugController->printLn("Web Interface is Disabled");
        this->nextionConnection.sendCommandValueTxt("vars.WifiAp.txt", WiFi.SSID());
        this->nextionConnection.switchToPage(3);
    }
    this->handleUpdate();
}

/**
 * @brief Syncronize basic state data
 */
void NextionDisplay::syncStateData() {
    TimeClient * timeClient = this->globalDataController->getTimeClient();
    this->nextionConnection.sendCommandValueInt("internalHour", timeClient->getHoursNumber());
    this->nextionConnection.sendCommandValueInt("internalMinute", timeClient->getMinutesNumber());
    this->nextionConnection.sendCommandValueInt("internalSecond", timeClient->getSecondsNumber());
    this->nextionConnection.sendCommandValueInt("vars.WifiStrength.val", EspController::getWifiQuality());
}

/**
 * @brief Syncronize weather data
 */
void NextionDisplay::syncWeatherData() {
    if (this->globalDataController->getWeatherSettings()->show && this->globalDataController->getWeatherSettings()->cityId != 0) {
        OpenWeatherMapClient *weatherClient = this->globalDataController->getWeatherClient();
        this->nextionConnection.sendCommandValueTxt("vars.EWeatherState.txt", weatherClient->getCondition(0));
        this->nextionConnection.sendCommandValueTxt("vars.EWeatherTemp.txt", weatherClient->getTempRounded(0) + " " + weatherClient->getTempSymbol());
        this->nextionConnection.sendCommandValueTxt("vars.EWeatherHumi.txt", weatherClient->getHumidityRounded(0) + " %");
        this->nextionConnection.sendCommandValueTxt("vars.EWeatherWind.txt", weatherClient->getWindRounded(0) + " " + weatherClient->getSpeedSymbol());
        this->nextionConnection.sendCommandValueTxt("vars.EWeatherCity.txt", weatherClient->getCity(0));
        this->nextionConnection.sendCommandValueTxt("vars.EWeatherLoc.txt", "Lat: " + weatherClient->getLat(0) + ", Lon: " + weatherClient->getLon(0));
        this->nextionConnection.sendCommandValueTxt("vars.EWeatherIcon.txt", this->getWeatherIconShortId());
    }
    if (this->globalDataController->getSensorSettings()->showOnDisplay && this->globalDataController->getSensorSettings()->activated) {
        SensorDataStruct *sensorSettings = this->globalDataController->getSensorSettings();
        BaseSensorClient *sensorClient = this->globalDataController->getSensorClient(sensorSettings);
        this->nextionConnection.sendCommandValueInt("hasSensor", 1);
        this->nextionConnection.sendCommandValueTxt("vars.SensType.txt", sensorClient->getType());
        this->nextionConnection.sendCommandValueTxt("vars.SensTemp.txt", sensorClient->getTemperatureRounded(sensorSettings)  + "°C");
        if (sensorClient->hasAirQuality()) {
            this->nextionConnection.sendCommandValueInt("vars.SensAqEnab.val", 1);
            this->nextionConnection.sendCommandValueInt("vars.SensAq.val", sensorClient->airQualityAsInt(sensorSettings));
        } else {
            this->nextionConnection.sendCommandValueInt("vars.SensAqEnab.val", 0);
        }
        if (sensorClient->hasHumidity()) {
            this->nextionConnection.sendCommandValueInt("vars.SensHumiEnab.val", 1);
            this->nextionConnection.sendCommandValueTxt("vars.SensHumi.txt", sensorClient->getHumidityRounded(sensorSettings) + "% Humidiy");
        } else {
            this->nextionConnection.sendCommandValueInt("vars.SensHumiEnab.val", 0);
        }
        if (sensorClient->hasPressure()) {
            this->nextionConnection.sendCommandValueInt("vars.SensPressEnab.val", 1);
            this->nextionConnection.sendCommandValueTxt("vars.SensPressure.txt", String(globalDataController->getSensorSettings()->pressure, 1) + " hPa");
        } else {
            this->nextionConnection.sendCommandValueInt("vars.SensPressEnab.val", 0);
        }
        if (sensorClient->hasAltitude()) {
            this->nextionConnection.sendCommandValueInt("vars.SensAltEnab.val", 1);
            this->nextionConnection.sendCommandValueTxt("vars.SensAlt.txt", String(globalDataController->getSensorSettings()->altitude, 1) +  "m");
        } else {
            this->nextionConnection.sendCommandValueInt("vars.SensAltEnab.val", 0);
        }
    } 
}

/**
 * @brief Syncronize settings data
 */
void NextionDisplay::syncSettingsData() {
    uint32_t heapFree = 0;
    uint16_t heapMax = 0;
    uint8_t heapFrag = 0;
    EspController::getHeap(&heapFree, &heapMax, &heapFrag);

    this->nextionConnection.sendCommandValueTxt("vars.ESPChip.txt", "ChipID: " + String(ESP.getChipId()));
    this->nextionConnection.sendCommandValueTxt("vars.ESPCore.txt", "CoreVersion: " + String(ESP.getCoreVersion()));
    this->nextionConnection.sendCommandValueTxt("vars.ESPFrag.txt", "Heap (frag/free/max): " + String(heapFrag) + "% |" +  String(heapFree) + " b|" + String(heapMax) + " b");
}

/**
 * @brief Syncronize printer data
 */
void NextionDisplay::syncPrintersData() {
    this->nextionConnection.sendCommandValueInt("activePrinters", this->globalDataController->numPrintersPrinting());
    this->nextionConnection.sendCommandValueInt("totalPrinters", this->globalDataController->getNumPrinters());

    PrinterDataStruct *printerConfigs = globalDataController->getPrinterSettings();
    for(int i=0; i<this->globalDataController->getNumPrinters(); i++) {
        if (printerConfigs[i].state == PRINTER_STATE_ERROR) {
            this->nextionConnection.sendCommandValueInt("vars.pr" + String(i+1) + "State.val", 3);
        }
        else if (printerConfigs[i].state == PRINTER_STATE_OFFLINE) {
            this->nextionConnection.sendCommandValueInt("vars.pr" + String(i+1) + "State.val", 0);
        }
        else if (printerConfigs[i].state == PRINTER_STATE_STANDBY) {
            this->nextionConnection.sendCommandValueInt("vars.pr" + String(i+1) + "State.val", 1);
        }
        else {
            this->nextionConnection.sendCommandValueInt("vars.pr" + String(i+1) + "State.val", 2);
        }
        this->nextionConnection.sendCommandValueTxt("vars.pr" + String(i+1) + "Name.txt", String(printerConfigs[i].customName));
        this->nextionConnection.sendCommandValueTxt("vars.pr" + String(i+1) + "Type.txt", globalDataController->getPrinterClientType(&printerConfigs[i]) + " | " + String(printerConfigs[i].remoteAddress) + ":" + String(printerConfigs[i].remotePort));
        this->nextionConnection.sendCommandValueTxt("vars.pr" + String(i+1) + "heIs.txt", String(printerConfigs[i].toolTemp, 1) + "°C");
        this->nextionConnection.sendCommandValueTxt("vars.pr" + String(i+1) + "heTarget.txt", String(printerConfigs[i].toolTargetTemp, 0) + "°C");
        if (printerConfigs[i].bedTemp > 0 ) {
            this->nextionConnection.sendCommandValueTxt("vars.pr" + String(i+1) + "hbIs.txt", String(printerConfigs[i].bedTemp) + "°C");
            this->nextionConnection.sendCommandValueTxt("vars.pr" + String(i+1) + "hbTarget.txt", String(printerConfigs[i].bedTargetTemp) + "°C");
        } else {
            this->nextionConnection.sendCommandValueTxt("vars.pr" + String(i+1) + "hbIs.txt", "N/A");
            this->nextionConnection.sendCommandValueTxt("vars.pr" + String(i+1) + "hbTarget.txt", "N/A");
        }

        if (String(printerConfigs[i].fileName).length() > 0) {
            this->nextionConnection.sendCommandValueTxt("vars.pr" + String(i+1) + "Job.txt", String(printerConfigs[i].fileName));
        } else {
            this->nextionConnection.sendCommandValueTxt("vars.pr" + String(i+1) + "Job.txt", "");
        }

        int val = printerConfigs[i].progressPrintTime;
        int hours = globalDataController->numberOfHours(val);
        int minutes = globalDataController->numberOfMinutes(val);
        int seconds = globalDataController->numberOfSeconds(val);
        this->nextionConnection.sendCommandValueTxt("vars.pr" + String(i+1) + "PrintSince.txt", globalDataController->zeroPad(hours) + ":" + globalDataController->zeroPad(minutes) + ":" + globalDataController->zeroPad(seconds));

        val = printerConfigs[i].progressPrintTimeLeft;
        hours = globalDataController->numberOfHours(val);
        minutes = globalDataController->numberOfMinutes(val);
        seconds = globalDataController->numberOfSeconds(val);
        this->nextionConnection.sendCommandValueTxt("vars.pr" + String(i+1) + "PrintRemain.txt", globalDataController->zeroPad(hours) + ":" + globalDataController->zeroPad(minutes) + ":" + globalDataController->zeroPad(seconds));

        this->nextionConnection.sendCommandValueTxt("vars.pr" + String(i+1) + "PrintEst.txt", "");
        this->nextionConnection.sendCommandValueInt("vars.pr" + String(i+1) + "JobPercent.val", printerConfigs[i].progressCompletion);
        this->nextionConnection.sendCommandValueTxt("vars.pr" + String(i+1) + "PrintError.txt", String(printerConfigs[i].error));
    }

    // Automatic switching pages
    DisplayDataStruct *displaySettings = this->globalDataController->getDisplaySettings();
    
    if((this->lastActivePrinters == 0) && (this->globalDataController->numPrintersPrinting() > 0)) {
        this->lastActivePrinters = this->globalDataController->numPrintersPrinting();
        if (displaySettings->automaticSwitchEnabled) {
            this->nextionConnection.switchToPage(7);
        }
        this->nextionConnection.sendCommandValueInt("sleep", 0);
        this->nextionConnection.sendCommandValueInt("thup", 1);
        this->nextionConnection.sendCommandValueInt("thsp", 0);
        this->nextionConnection.sendCommandValueInt("sleep", 0);
    } else if((this->lastActivePrinters > 0) && (this->globalDataController->numPrintersPrinting() == 0)) {
        this->lastActivePrinters = this->globalDataController->numPrintersPrinting();
        if (displaySettings->automaticSwitchEnabled) {
            this->nextionConnection.switchToPage(4);
        }
        if (displaySettings->automaticInactiveOff > 0) {
            this->nextionConnection.sendCommandValueInt("thup", 1);
            this->nextionConnection.sendCommandValueInt("thsp", displaySettings->automaticInactiveOff);
        }
    }
}

/**
 * @brief Retrun ID for weather icon for nextion device
 * @return String 
 */
String NextionDisplay::getWeatherIconShortId()
{
    String weather = this->globalDataController->getWeatherClient()->getWeatherId(0);
    int id = weather.toInt();
    String returnVal = "211";
    bool d = weather.indexOf('d') > 0;
    switch(id)
    {
        case 800:
            returnVal = "800";
            if (d) {
                returnVal += "d";
            } else {
                returnVal += "n";
            }
            break;
        case 801:
            returnVal = "801";
            if (d) {
                returnVal += "d";
            } else {
                returnVal += "n";
            }
            break;
        case 802:
            returnVal = "802";
            if (d) {
                returnVal += "d";
            } else {
                returnVal += "n";
            }
            break;
        case 803: returnVal = "803"; break;
        case 804: returnVal = "804"; break;
        
        case 200: returnVal = "211"; break;
        case 201: returnVal = "211"; break;
        case 202: returnVal = "211"; break;
        case 210: returnVal = "211"; break;
        case 211: returnVal = "211"; break;
        case 212: returnVal = "212"; break;
        case 221: returnVal = "212"; break;
        case 230: returnVal = "230"; break;
        case 231: returnVal = "231"; break;
        case 232: returnVal = "231"; break;
        
        case 300: returnVal = "500d"; break;
        case 301: returnVal = "500d"; break;
        case 302: returnVal = "500d"; break;
        case 310: returnVal = "500d"; break;
        case 311: returnVal = "500d"; break;
        case 312: returnVal = "500d"; break;
        case 313: returnVal = "500d"; break;
        case 314: returnVal = "500d"; break;
        case 321: returnVal = "500d"; break;
        
        case 500:
            returnVal = "500";
            if (d) {
                returnVal += "d";
            } else {
                returnVal += "n";
            }
            break;
        case 501: returnVal = "501"; break;
        case 502: returnVal = "502"; break;
        case 503: returnVal = "502"; break;
        case 504: returnVal = "502"; break;
        case 511: returnVal = "502"; break;
        case 520: returnVal = "502"; break;
        case 521: returnVal = "502"; break;
        case 522: returnVal = "502"; break;
        case 531: returnVal = "502"; break;
        
        case 600:
            returnVal = "600";
            if (d) {
                returnVal += "d";
            } else {
                returnVal += "n";
            }
            break;
        case 601: returnVal = "601"; break;
        case 602: returnVal = "602"; break;
        case 611: returnVal = "611"; break;
        case 612: returnVal = "611"; break;
        case 615: returnVal = "611"; break;
        case 616: returnVal = "611"; break;
        case 620: returnVal = "611"; break;
        case 621: returnVal = "611"; break;
        case 622: returnVal = "611"; break;
        
        case 701: returnVal = "721d"; break;
        case 711: returnVal = "721d"; break;
        case 721:
            returnVal = "721";
            if (d) {
                returnVal += "d";
            } else {
                returnVal += "n";
            }
            break;
        case 731: returnVal = "721d"; break;
        case 741: returnVal = "741"; break;
        case 751: returnVal = "741"; break;
        case 761: returnVal = "741"; break;
        case 762: returnVal = "741"; break;
        case 771: returnVal = "741"; break;
        case 781: returnVal = "741"; break;
        default:break; 
    }
    return returnVal;
}
