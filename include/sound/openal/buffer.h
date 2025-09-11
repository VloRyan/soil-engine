#ifndef SOIL_SOUND_OPENAL_BUFFER_H
#define SOIL_SOUND_OPENAL_BUFFER_H
#include <string>
#include "sound/buffer.h"

namespace soil::sound::openal {
    class Buffer final : public sound::Buffer {
    public:
        explicit Buffer(std::string name);

        ~Buffer() override;

        [[nodiscard]] std::string getName() const override;

        [[nodiscard]] uint getId() const override;

        [[nodiscard]] uint getDataSize() const override;

        void setData(const void *data, int dataSize, int format, int frequency) override;

    private:
        uint id_;
        std::string name_;

        uint dataSize_;
    };
} // namespace soil::sound::openal
#endif
