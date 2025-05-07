#ifndef SOIL_EXCEPTION_H
#define SOIL_EXCEPTION_H
#include <string>

namespace soil {
    class Exception final : public std::exception {
    public:
        explicit Exception(std::string message);

        Exception(std::string message, Exception *cause);

        ~Exception() override;

        [[nodiscard]] Exception *getCause() const;

        [[nodiscard]] std::string getMessage() const noexcept;

        [[nodiscard]] const char *what() const noexcept override;

    private:
        Exception *cause_;
        std::string message_;
    };
} // namespace soil
#endif // SOIL_EXCEPTION_H
