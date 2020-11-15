#include <set>
#include <stdio.h>
#include "include/ILogger.h"

using namespace std;

namespace {
    class LoggerImpl : ILogger {

    protected:
        static LoggerImpl * _instance;
        FILE * _log_file{nullptr};
        set<void *> _clients{nullptr};
    public:
        static ILogger * getLogger(void * client);

        void log(const char * message, ReturnCode returnCode)    override;
        void releaseLogger(void * client)                        override;
        ReturnCode setLogFile(const char * logFileName)          override;

        ~LoggerImpl()                                           override;
    };
    LoggerImpl * LoggerImpl::_instance = nullptr;
}

static char const* RC_messages[(size_t)ReturnCode::RC_UNKNOWN + 1] = {
        "Success! (no errors)",
        "Not enough memory for allocation",
        "Can't work with nullptr",
        "Zero dimension object",
        "Working with objects of different dimensions",
        "Working with objects without value",
        "Run out the bounds",
        "Unable open file",
        "Can't find an element",
        "Invalid parameters passed",
        "Initialization of parameters is required",
        "Unknown error"
};

ILogger * LoggerImpl::getLogger(void *client) {
    if (!client) {
        return nullptr;
    }

    if (!LoggerImpl::_instance) {
        _instance = new(std::nothrow) LoggerImpl();
        if (!_instance) {
            return nullptr;
        }
        _instance->_log_file = stdout;
    }
    _instance->_clients.insert(client);
    return _instance;
}

void LoggerImpl::log(const char * message, ReturnCode returnCode) {
    if (!_instance) {
        return;
    }
    if (!message) {
        fprintf(_log_file, "fun=%s code=%d message=NULL \n", __FUNCTION__, returnCode);
    }
    fprintf(_log_file, "fun=%s code=%d message=%s \n", __FUNCTION__, returnCode ,RC_messages[(size_t)returnCode]);
    //fflush(_log_file);
}

void LoggerImpl::releaseLogger(void * client) {
    if (!_instance) {
        return;
    }
    if (!client) {
        return;
    }
    auto client_iter = _clients.find(client);
    if (client_iter != _clients.end())
        _clients.erase(client_iter);
    if (_clients.empty()) {
        delete _instance;
        _instance = nullptr;
    }
}

ReturnCode LoggerImpl::setLogFile(const char * logFileName) {
    if (!_instance) {
        return ReturnCode::RC_NULL_PTR;
    }
    if (_instance->_log_file != stdout) {
        if (!freopen(logFileName, "a", _instance->_log_file)) {
            return ReturnCode::RC_OPEN_FILE;
        }
    } else {
        _instance->_log_file = fopen(logFileName, "w");
        if (_instance->_log_file == nullptr) {
            _instance->_log_file = stdout;
        }
    }
    return ReturnCode::RC_SUCCESS;
}

LoggerImpl::~LoggerImpl() {
    if (!_instance) {
        return;
    }
    fflush(_log_file);
    if (_log_file != stdout)
        fclose(_log_file);
}