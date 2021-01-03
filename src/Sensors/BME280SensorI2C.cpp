#include "BME280SensorI2C.h"

/**
 * @brief Construct a new BME280SensorI2C::BME280SensorI2C object
 * 
 * @param globalDataController  Handle to global data controller
 * @param debugController       Handle to debug controller
 * @param jsonRequestClient     Handle to json request instance
 */
BME280SensorI2C::BME280SensorI2C(GlobalDataController *globalDataController, DebugController *debugController, JsonRequestClient *jsonRequestClient)
: BME280SensorBase("BME280 - Mode I2C", globalDataController, debugController, jsonRequestClient) {
}

/**
 * @brief Initialize the sensor
 * 
 * @param i2cInterface          Handle to I2C Interface
 * @param spiInterface          Handle to SPI Interface
 * @param spiCsPin              SPI chip select pin to use
 * @param oneWirePin            Pin to use for one wire communication
 */
void BME280SensorI2C::initialize(TwoWire *i2cInterface, SPIClass *spiInterface, uint8_t spiCsPin, uint8_t oneWirePin) {
    this->bme = new Adafruit_BME280(); 
    this->i2cInterface = i2cInterface;
}

/**
 * @brief start the sensor
 * @param sensorData            Handle to sensor data struct
 * @return True if started
 */
boolean BME280SensorI2C::startSensor(SensorDataStruct *sensorData) {
    if (!this->bme->begin(0x77, this->i2cInterface)) {
        MemoryHelper::stringToChar("Could not find a valid BME280 sensor, check wiring!", sensorData->error, 120);
        sensorData->sensorIsRuning = false;
        return false;
    }
    sensorData->sensorIsRuning = true;

    this->bme->setSampling(Adafruit_BME280::MODE_FORCED,
        Adafruit_BME280::SAMPLING_X4, // temperature
        Adafruit_BME280::SAMPLING_X4, // pressure
        Adafruit_BME280::SAMPLING_X4, // humidity
        Adafruit_BME280::FILTER_OFF
    );
    return true;
}
