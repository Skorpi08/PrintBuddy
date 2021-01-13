#include "NextionUpdater.h"

/**
 * @brief Construct a new Nextion Updater:: Nextion Updater object
 * @param connectionHandle 
  * @param debugController 
 */
NextionUpdater::NextionUpdater(NextionConnection *connectionHandle, DebugController *debugController) {
    this->debugController = debugController;
    this->connectionHandle = connectionHandle;
}


// https://github.com/itead/ITEADLIB_Arduino_Nextion/blob/master/NexUpload.cpp