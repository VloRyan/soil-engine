#include "log.h"
#include <plog/Log.h>
#include <plog/Formatters/TxtFormatter.h>
#include "plog/Initializers/ConsoleInitializer.h"

namespace soil {
    void Log::Init() {
        plog::init<plog::TxtFormatter>(plog::debug, plog::OutputStream::streamStdOut);
    }

    void Log::Debug(const std::string &text) {
        PLOG_DEBUG << text;
    }

    void Log::Debugf(const std::string &format, ...) {
        va_list args;
        va_start(args, format);
        PLOG_DEBUG.printf(format.c_str(), args);
        va_end(args);
    }

    void Log::Info(const std::string &text) {
        PLOG_INFO << text;
    }

    void Log::Infof(const std::string &format, ...) {
        va_list args;
        va_start(args, format);
        PLOG_INFO.printf(format.c_str(), args);
        va_end(args);
    }

    void Log::Error(const std::string &text) {
        PLOG_ERROR << text;
    }

    void Log::Errorf(const std::string &format, ...) {
        va_list args;
        va_start(args, format);
        PLOG_ERROR.printf(format.c_str(), args);
        va_end(args);
    }
} // logger
