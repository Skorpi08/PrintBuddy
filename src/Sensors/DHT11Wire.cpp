#include "DHT11Wire.h"

/**
 * @brief Construct a new DHT11Wire::DHT11Wire object
 * 
 * @param globalDataController  Handle to global data controller
 * @param debugController       Handle to debug controller
 * @param jsonRequestClient     Handle to json request instance
 */
DHT11Wire::DHT11Wire(GlobalDataController *globalDataController, DebugController *debugController, JsonRequestClient *jsonRequestClient)
: DHTxxSensorBase("DHT11 - 1Wire", globalDataController, debugController, jsonRequestClient) {
}

/**
 * @brief Initialize the sensor
 * 
 * @param i2cInterface          Handle to I2C Interface
 * @param spiInterface          Handle to SPI Interface
 * @param spiCsPin              SPI chip select pin to use
 * @param oneWirePin            Pin to use for one wire communication
 */
void DHT11Wire::initialize(TwoWire *i2cInterface, SPIClass *spiInterface, uint8_t spiCsPin, uint8_t oneWirePin) {
    this->dht = new DHT_Unified(oneWirePin, DHT11);
}
