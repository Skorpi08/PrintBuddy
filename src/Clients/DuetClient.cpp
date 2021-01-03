// DUET API: https://reprap.org/wiki/RepRap_Firmware_Status_responses

#include "DuetClient.h"

/**
 * @brief Construct a new Duet Client:: Duet Client object
 * 
 * @param globalDataController  Handle to global data controller
 * @param debugController       Handle to debug controller
 * @param jsonRequestClient     Handle to json request instance
 */
DuetClient::DuetClient(GlobalDataController *globalDataController, DebugController *debugController, JsonRequestClient *jsonRequestClient)
    : BasePrinterClientImpl("Duet", globalDataController, debugController, jsonRequestClient) {
}

/**
 * @brief Refresh job and state data for printer
 * @param printerData       Handle to printer struct
 */
void DuetClient::getPrinterJobResults(PrinterDataStruct *printerData) {
    JsonDocument *jsonDoc;
    if (!this->isValidConfig(printerData)) {
        return;
    }

    // Req 1 -> We need to trigger the api to response data!
    this->debugController->printLn("Get Duet Data: " + String(printerData->remoteAddress) + ":" + String(printerData->remotePort));
    jsonDoc = this->jsonRequestClient->requestJson(
        PRINTER_REQUEST_GET,
        String(printerData->remoteAddress),
        printerData->remotePort,
        String(printerData->encAuth),
        "/rr_connect?password=reprap",
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

    // Req 2
    this->debugController->printLn("Get Duet Data: " + String(printerData->remoteAddress) + ":" + String(printerData->remotePort));
    jsonDoc = this->jsonRequestClient->requestJson(
        PRINTER_REQUEST_GET,
        String(printerData->remoteAddress),
        printerData->remotePort,
        String(printerData->encAuth),
        "/rr_status?type=1",
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
    printerData->state = DuetClient::translateState(((*jsonDoc)["status"]).as<String>());

    if (this->isOperational(printerData)) {
        this->debugController->printLn("Status: " + this->globalDataController->getPrinterStateAsText(printerData));
    } else {
        this->debugController->printLn("Printer Not Operational");
    }

    if (printerData->state == PRINTER_STATE_PRINTING) {
        printerData->isPrinting = true;
    } else {
        // We dont printing, so abort function here
        printerData->isPrinting = false;
        //return;
    }

    // Req 3
    jsonDoc = this->jsonRequestClient->requestJson(
        PRINTER_REQUEST_GET,
        String(printerData->remoteAddress),
        printerData->remotePort,
        String(printerData->encAuth),
        "/rr_status?type=3",
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
    printerData->filamentLength = ((*jsonDoc)["result"]["status"]["job"]["print_stats"]["filament_used"]).as<float>();
    printerData->progressPrintTime = ((*jsonDoc)["printDuration"]).as<float>();
    printerData->progressCompletion = ((*jsonDoc)["fractionPrinted"]).as<int>();
    printerData->toolTemp = ((*jsonDoc)["temps"]["current"][1]).as<int>();
    printerData->toolTargetTemp = ((*jsonDoc)["temps"]["tools"]["active"][0][0]).as<int>();
    printerData->bedTemp = ((*jsonDoc)["temps"]["bed"]["current"]).as<int>();
    printerData->bedTargetTemp = ((*jsonDoc)["temps"]["bed"]["active"]).as<int>();
    printerData->progressFilepos = ((*jsonDoc)["filePosition"]).as<int>();
    printerData->estimatedPrintTime = ((*jsonDoc)["file"]).as<float>();
    printerData->progressPrintTimeLeft = ((*jsonDoc)["timesLeft"]["file"]).as<float>();

    if (this->isOperational(printerData)) {
        this->debugController->printLn("Status: "
            + this->globalDataController->getPrinterStateAsText(printerData) + " "
            + String(printerData->fileName) + "("
            + String(printerData->progressCompletion) + "%)"
        );
    }
}

/**
 * @brief Refresh psu state data for printer
 * @param printerData       Handle to printer struct
 */
void DuetClient::getPrinterPsuState(PrinterDataStruct *printerData)  {
}

/**
 * We translate the avail states 
 *  - C (configuration file is being processed)
 *  - I (idle, no movement or code is being performed)
 *  - B (busy, live movement is in progress or a macro file is being run)
 *  - P (printing a file)
 *  - D (decelerating, pausing a running print)
 *  - S (stopped, live print has been paused)
 *  - R (resuming a paused print)
 *  - H (halted, after emergency stop)
 *  - F (flashing new firmware)
 *  - T (changing tool, new in 1.17b)
 *  - M (simulating print)
 */
int DuetClient::translateState(String stateText) {
    stateText.toLowerCase();
    stateText.trim();
    if (stateText == "i" || stateText == "t" || stateText == "b" || stateText == "c" || stateText == "f" || stateText == "t" || stateText == "o" || stateText == "m") {
        return PRINTER_STATE_STANDBY;
    }
    if (stateText == "p" || stateText == "r") {
        return PRINTER_STATE_PRINTING;
    }
    if (stateText == "d" || stateText == "s") {
        return PRINTER_STATE_PAUSED;
    }
    if (stateText == "h") {
        return PRINTER_STATE_ERROR;
    }
    return PRINTER_STATE_OFFLINE;
}
