#include "BaseSensorClientImpl.h"

/**
 * @brief Construct a new Base Sensor Client Impl:: Base Sensor Client Impl object
 * 
 * @param clientType            Client type name
 * @param globalDataController  Handle to global data controller
 * @param debugController       Handle to debug controller
 * @param jsonRequestClient     Handle to json request instance
 */
BaseSensorClientImpl::BaseSensorClientImpl(
    String clientType,
    GlobalDataController *globalDataController,
    DebugController *debugController,
    JsonRequestClient *jsonRequestClient
) {
    this->globalDataController = globalDataController;
    this->debugController = debugController;
    this->clientType = clientType;
    this->jsonRequestClient = jsonRequestClient;
}

/**
 * @brief Get the type of client for this instance
 * @return String 
 */
String BaseSensorClientImpl::getType() {
    return this->clientType;
}

/**
 * @brief Calculate pressure to altitude
 * @param pressure      Pressure from sensor (hPa)
 * @param temp          temperature from sensor (Deg)
 * @return float        Altitude in meter
 */
float BaseSensorClientImpl::pressureToAltitude(float pressure, float temp) {
    return (1.0 - pow(pressure /SEALEVELPRESSURE_HPA,0.1903)) * (temp + 273.15F) / 0.0065F;
}