#include "exception.h"

#include <utility>

namespace soil
{
    Exception::Exception(std::string message) : Exception(std::move(message), nullptr) {}

    Exception::Exception(std::string message, Exception *cause) : cause_(cause), message_(std::move(message)) {}

    Exception::~Exception() = default;

    Exception *Exception::getCause() const { return cause_; }

    std::string Exception::getMessage() const noexcept { return message_; }

    const char *Exception::what() const noexcept { return message_.c_str(); }
} // namespace engine
