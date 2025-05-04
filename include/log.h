#ifndef ENGINE_ENGINE_LOG_LOG_H_
#define ENGINE_ENGINE_LOG_LOG_H_

#include <string>
#include <plog/Severity.h>

namespace engine {
    class Log {
    public:
        static void Init();

        static void Debug(const std::string &text);

        static void Debugf(const std::string &format, ...);

        static void Info(const std::string &text);

        static void Infof(const std::string &format, ...);

        static void Error(const std::string &text);

        static void Errorf(const std::string &format, ...);
    };
} // logger

#endif //ENGINE_ENGINE_LOG_LOG_H_
