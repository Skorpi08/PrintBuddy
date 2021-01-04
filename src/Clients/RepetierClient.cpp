// 
// ArduinoJSON Assistant: https://arduinojson.org/v6/assistant/

#include "RepetierClient.h"

/**
 * @brief Construct a new Repetier Client:: Repetier Client object
 * 
 * @param globalDataController  Handle to global data controller
 * @param debugController       Handle to debug controller
 * @param jsonRequestClient     Handle to json request instance
 */
RepetierClient::RepetierClient(GlobalDataController *globalDataController, DebugController *debugController, JsonRequestClient *jsonRequestClient)
    : BasePrinterClientImpl("Repetier", globalDataController, debugController, jsonRequestClient) {
}

/**
 * @brief Refresh job and state data for printer
 * @param printerData       Handle to printer struct
 */
void RepetierClient::getPrinterJobResults(PrinterDataStruct *printerData) {
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
#endif
}

/**
 * @brief Refresh psu state data for printer
 * @param printerData       Handle to printer struct
 */
void RepetierClient::getPrinterPsuState(PrinterDataStruct *printerData)  {
}

/**
 * We translate the avail states 
 */
int RepetierClient::translateState(String stateText) {
    return PRINTER_STATE_OFFLINE;
}
