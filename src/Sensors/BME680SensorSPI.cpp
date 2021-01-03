#include "BME680SensorSPI.h"

/**
 * @brief Construct a new BME680SensorSPI::BME680SensorSPI object
 * 
 * @param globalDataController  Handle to global data controller
 * @param debugController       Handle to debug controller
 * @param jsonRequestClient     Handle to json request instance
 */
BME680SensorSPI::BME680SensorSPI(GlobalDataController *globalDataController, DebugController *debugController, JsonRequestClient *jsonRequestClient)
: BME680SensorBase("BME680 - Mode SPI", globalDataController, debugController, jsonRequestClient) {
}

/**
 * @brief Initialize the sensor
 * 
 * @param i2cInterface          Handle to I2C Interface
 * @param spiInterface          Handle to SPI Interface
 * @param spiCsPin              SPI chip select pin to use
 * @param oneWirePin            Pin to use for one wire communication
 */
void BME680SensorSPI::initialize(TwoWire *i2cInterface, SPIClass *spiInterface, uint8_t spiCsPin, uint8_t oneWirePin) {
    this->bme = new Adafruit_BME680(spiCsPin, spiInterface);
}
