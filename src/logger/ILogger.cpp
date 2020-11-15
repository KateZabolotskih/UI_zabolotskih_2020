#include "include/ILogger.h"
#include "ILoggerImpl.cpp"

ILogger* ILogger::createLogger(void* client) {
    return LoggerImpl::getLogger(client);
}

ILogger::~ILogger() {}