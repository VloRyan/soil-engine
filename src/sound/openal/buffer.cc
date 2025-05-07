
#include "sound/openal/buffer.h"
#include <AL/al.h>

#include <utility>

namespace soil::sound::openal {
    Buffer::Buffer(std::string name) : sound::Buffer(), id_(0), name_(std::move(name)), dataSize_(0) {
        // create our openAL buffer and check for success
        alGenBuffers(1, &id_);
    }

    Buffer::~Buffer() {
        if (id_ > 0) {
            alDeleteBuffers(1, &id_);
        }
    }

    std::string Buffer::getName() const { return name_; }

    uint Buffer::getId() const { return id_; }

    uint Buffer::getDataSize() const { return dataSize_; }

    void Buffer::setData(const ALvoid *data, const ALsizei dataSize, const ALenum format, const ALsizei frequency) {
        dataSize_ = static_cast<uint>(dataSize);
        alBufferData(id_, format, data, dataSize, frequency);
    }
} // namespace soil::sound::openal
