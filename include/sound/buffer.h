#ifndef SOIL_SOUND_BUFFER_H
#define SOIL_SOUND_BUFFER_H
#include <string>

namespace soil::sound {
    class Buffer {
    public:
        explicit Buffer() = default;
        virtual ~Buffer() = default;
        [[nodiscard]] virtual std::string getName() const = 0;
        [[nodiscard]] virtual uint getId() const = 0;
        [[nodiscard]] virtual uint getDataSize() const = 0;
        virtual void setData(const void *data, int dataSize, int format, int frequency) = 0;
    };
} // namespace soil::sound
#endif
