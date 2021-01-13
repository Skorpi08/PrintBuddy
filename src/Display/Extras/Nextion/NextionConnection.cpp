#include "NextionConnection.h"

/**
 * @brief Construct a new Nextion Connection:: Nextion Connection object
 * @param serialPort 
  * @param debugController 
 */
NextionConnection::NextionConnection(SoftwareSerial *serialPort, DebugController *debugController) {
    this->debugController = debugController;
     this->serialPort = serialPort;
}

/**
 * @brief Set baudrate for connection
 * @param baudrate 
 */
void NextionConnection::setBaudrate(int baudrate) {
    this->serialPort->begin(DISPLAY_BAUDRATE);
}

/**
 * @brief Send reset command to display
 */
void NextionConnection::resetDevice() {
    this->sendCommandValueInt("thup", 1);
    this->sendCommandValueInt("thsp", 0);
    this->sendCommandValueInt("sleep", 0);
    this->sendCommand("rest");
    delay(500);
}

/**
 * @brief Swicth to another page
 * @param pageId 
 */
void NextionConnection::switchToPage(int pageId) {
    String command("page ");
    command += String(pageId);
    this->sendCommand(command.c_str());
    delay(50);
}

/**
 * @brief Set an text variable (var) with value
 * @param var 
 * @param value 
 */
void NextionConnection::sendCommandValueTxt(String var, String value) {
    String command(var + "=");
    command += "\"" + value + "\"";
    this->sendCommand(command.c_str());
}

/**
 * @brief Set an number variable (var) with value
 * @param var 
 * @param value 
 */
void NextionConnection::sendCommandValueInt(String var, int value) {
    String command(var + "=");
    command += String(value);
    this->sendCommand(command.c_str());
}

/**
 * @brief Send command to display
 * @param cmd 
 */
void NextionConnection::sendCommand(String cmd) {
    this->sendCommand(cmd.c_str());
}

/**
 * @brief Send command to display
 * @param cmd 
 */
void NextionConnection::sendCommand(const char* cmd) {
  while (this->serialPort->available())
  {
    this->serialPort->read();
  }
    
  this->serialPort->print(cmd);
  this->serialPort->write(0xFF);
  this->serialPort->write(0xFF);
  this->serialPort->write(0xFF);
}
