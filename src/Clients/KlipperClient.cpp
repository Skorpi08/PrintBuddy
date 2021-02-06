// Moonraker API: https://github.com/Arksine/moonraker/blob/master/docs/web_api.md

#include "KlipperClient.h"

/**
 * @brief Construct a new Klipper Client:: Klipper Client object
 * 
 * @param globalDataController  Handle to global data controller
 * @param debugController       Handle to debug controller
 * @param jsonRequestClient     Handle to json request instance
 */
KlipperClient::KlipperClient(GlobalDataController *globalDataController, DebugController *debugController, JsonRequestClient *jsonRequestClient)
: BasePrinterClientImpl("Klipper", globalDataController, debugController, jsonRequestClient) {
}

/**
 * @brief Refresh job and state data for printer
 * @param printerData       Handle to printer struct
 */
void KlipperClient::getPrinterJobResults(PrinterDataStruct *printerData) {
    if (!this->isValidConfig(printerData)) {
        return;
    }
    
#ifdef SIMULATE_CLIENTS_PRINTING
    // Simulate printing
    printerData->state = PRINTER_STATE_PRINTING;
    printerData->filamentLength = 20;
    printerData->progressPrintTime = 1039;
    MemoryHelper::stringToChar(
        "test.gcode",
        printerData->fileName,
        60
    );
    printerData->isPrinting = true;
    printerData->toolTemp = 225;
    printerData->toolTargetTemp = 230;
    printerData->bedTemp = 79;
    printerData->bedTargetTemp = 80;
    printerData->progressFilepos = 20;
    printerData->estimatedPrintTime = 5005;
    printerData->progressPrintTimeLeft = 4000;
    printerData->progressCompletion = 20;
    return;
#else
    JsonDocument *jsonDoc;

    // Req 1
    this->debugController->printLn("Get Klipper Data: " + String(printerData->remoteAddress) + ":" + String(printerData->remotePort));
    jsonDoc = this->jsonRequestClient->requestJson(
        PRINTER_REQUEST_GET,
        String(printerData->remoteAddress),
        printerData->remotePort,
        String(printerData->encAuth),
        "/printer/objects/query?print_stats",
        "",
        true
    );
    if (this->jsonRequestClient->getLastError() != "") {
        this->debugController->printLn(this->jsonRequestClient->getLastError());
        if (this->jsonRequestClient->getLastError().indexOf("PARSER") == 0) {
            printerData->errorReadCnt++;
            if (printerData->errorReadCnt >= MAX_PRINTER_REQ_FAILED) {
                BasePrinterClient::resetPrinterData(printerData);
                MemoryHelper::stringToChar(this->jsonRequestClient->getLastError(), printerData->error, 120);
                printerData->state = PRINTER_STATE_ERROR;
                printerData->errorReadCnt = MAX_PRINTER_REQ_FAILED;
            }
        }
        return;
    }

    printerData->errorReadCnt = 0;
    printerData->state = KlipperClient::translateState(((*jsonDoc)["result"]["status"]["print_stats"]["state"]).as<String>());
    printerData->filamentLength = ((*jsonDoc)["result"]["status"]["job"]["print_stats"]["filament_used"]).as<float>();
    printerData->progressPrintTime = ((*jsonDoc)["result"]["status"]["print_stats"]["print_duration"]).as<float>();
    MemoryHelper::stringToChar(
        ((*jsonDoc)["result"]["status"]["print_stats"]["filename"]).as<const char*>(),
        printerData->fileName,
        60
    );

    if (this->isOperational(printerData)) {
        this->debugController->printLn("Status: " + this->globalDataController->getPrinterStateAsText(printerData));
    } else {
        this->debugController->printLn("Printer Not Operational");
    }
    printerData->isPrinting = (printerData->state == PRINTER_STATE_PRINTING);
    
    // Req 2
    jsonDoc = this->jsonRequestClient->requestJson(
        PRINTER_REQUEST_GET,
        String(printerData->remoteAddress),
        printerData->remotePort,
        String(printerData->encAuth),
        "/printer/objects/query?heater_bed&extruder&display_status&toolhead&virtual_sdcard&print_stats",
        "",
        true
    );
    if (this->jsonRequestClient->getLastError() != "") {
        this->debugController->printLn(this->jsonRequestClient->getLastError());
        if (this->jsonRequestClient->getLastError().indexOf("PARSER") == 0) {
            printerData->errorReadCnt++;
            if (printerData->errorReadCnt >= MAX_PRINTER_REQ_FAILED) {
                BasePrinterClient::resetPrinterData(printerData);
                MemoryHelper::stringToChar(this->jsonRequestClient->getLastError(), printerData->error, 120);
                printerData->state = PRINTER_STATE_ERROR;
                printerData->errorReadCnt = MAX_PRINTER_REQ_FAILED;
            }
        }
        return;
    }

    printerData->errorReadCnt = 0;
    float progressCompletion = ((*jsonDoc)["result"]["status"]["display_status"]["progress"]).as<float>();
    printerData->toolTemp = ((*jsonDoc)["result"]["status"]["extruder"]["temperature"]).as<int>();
    printerData->toolTargetTemp = ((*jsonDoc)["result"]["status"]["extruder"]["target"]).as<int>();
    printerData->bedTemp = ((*jsonDoc)["result"]["status"]["heater_bed"]["temperature"]).as<int>();
    printerData->bedTargetTemp = ((*jsonDoc)["result"]["status"]["heater_bed"]["target"]).as<int>();
    float fileProgress = ((*jsonDoc)["result"]["status"]["virtual_sdcard"]["progress"]).as<float>();
    printerData->progressFilepos = ((*jsonDoc)["result"]["status"]["virtual_sdcard"]["file_position"]).as<int>();
    printerData->estimatedPrintTime = ((*jsonDoc)["result"]["status"]["toolhead"]["estimated_print_time"]).as<float>();
    printerData->progressPrintTime = ((*jsonDoc)["result"]["status"]["print_stats"]["print_duration"]).as<float>();

    float totalPrintTime = printerData->progressPrintTime / fileProgress;
    printerData->progressPrintTimeLeft = totalPrintTime - printerData->progressPrintTime;
    printerData->progressCompletion = progressCompletion * 100;

    if (this->isOperational(printerData)) {
        this->debugController->printLn("Status: "
            + this->globalDataController->getPrinterStateAsText(printerData) + " "
            + String(printerData->fileName) + "("
            + String(printerData->progressCompletion) + "%)"
        );
    }
#endif
}

/**
 * @brief Refresh psu state data for printer
 * @param printerData       Handle to printer struct
 */
void KlipperClient::getPrinterPsuState(PrinterDataStruct *printerData)  {
}

/**
 * We translate the avail states 
 *  - "standby": No print in progress
 *  - "printing": The printer is currently printing
 *  - "paused": A print in progress has been paused
 *  - "error": The print exited with an error. print_stats.message contains a related error message
 *  - "complete": The last print has completed
 */
int KlipperClient::translateState(String stateText) {
    stateText.toLowerCase();
    stateText.trim();
    if (stateText == "standby") {
        return PRINTER_STATE_STANDBY;
    }
    if (stateText == "printing") {
        return PRINTER_STATE_PRINTING;
    }
    if (stateText == "paused") {
        return PRINTER_STATE_PAUSED;
    }
    if (stateText == "complete") {
        return PRINTER_STATE_COMPLETED;
    }
    if (stateText == "error") {
        return PRINTER_STATE_ERROR;
    }
    return PRINTER_STATE_OFFLINE;
}
