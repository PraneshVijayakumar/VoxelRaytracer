#ifndef FORGE_WINDOWS_LOGGING_H
#define FORGE_WINDOWS_LOGGING_H

#include "ForgeWindowsHeader.h"
#include <Forge/Source/Core/Logging.h>

#define FORGE_WINDOWS_ERROR_LOG(pErrorString) FORGE_ERROR_LOG("Windows error : ", pErrorString)

#define FORGE_WINDOWS_GET_AND_LOG_LAST_ERROR(pErrorString) FORGE_ERROR_LOG("Windows error : ", pErrorString, '\n', "Windows Error code : ", GetLastError(), "\n\n")

#endif