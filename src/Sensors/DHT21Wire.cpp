#include "DHT21Wire.h"

/**
 * @brief Construct a new DHT21Wire::DHT21Wire object
 * 
 * @param globalDataController  Handle to global data controller
 * @param debugController       Handle to debug controller
 * @param jsonRequestClient     Handle to json request instance
 */
DHT21Wire::DHT21Wire(GlobalDataController *globalDataController, DebugController *debugController, JsonRequestClient *jsonRequestClient)
: DHTxxSensorBase("DHT21 - 1Wire", globalDataController, debugController, jsonRequestClient) {
}

/**
 * @brief Initialize the sensor
 * 
 * @param i2cInterface          Handle to I2C Interface
 * @param spiInterface          Handle to SPI Interface
 * @param spiCsPin              SPI chip select pin to use
 * @param oneWirePin            Pin to use for one wire communication
 */
void DHT21Wire::initialize(TwoWire *i2cInterface, SPIClass *spiInterface, uint8_t spiCsPin, uint8_t oneWirePin) {
    this->dht = new DHT_Unified(oneWirePin, DHT21);
}
