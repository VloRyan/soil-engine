#ifndef ENGINE_ENGINE_EXCEPTION_H_
#define ENGINE_ENGINE_EXCEPTION_H_
#include <string>

namespace engine {
    class Exception : public std::exception {
    public:
        explicit Exception(std::string message);

        Exception(std::string message, Exception *cause);

        ~Exception() override;

        [[nodiscard]] Exception *getCause() const;

        [[nodiscard]] virtual std::string getMessage() const noexcept;

        [[nodiscard]] const char *what() const noexcept override;

    private:
        Exception *cause_;
        std::string message_;
    };
}
#endif //ENGINE_ENGINE_EXCEPTION_H_
