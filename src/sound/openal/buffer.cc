
#include "sound/openal/buffer.h"

#include <AL/al.h>

#include <utility>

namespace soil::sound::openal {
Buffer::Buffer() : sound::Buffer(), id_(0), dataSize_(0) {
  // create our openAL buffer and check for success
  alGenBuffers(1, &id_);
}

Buffer::~Buffer() {
  if (id_ > 0) {
    alDeleteBuffers(1, &id_);
  }
}

ALuint Buffer::Id() const { return id_; }

ALsizei Buffer::DataSize() const { return dataSize_; }

void Buffer::SetData(const ALvoid *data, const ALsizei dataSize,
                     const ALenum format, const ALsizei samplerate) {
  dataSize_ = dataSize;
  alBufferData(id_, format, data, dataSize, samplerate);
}
}  // namespace soil::sound::openal
