
#include "sound/manager.h"
#include "AL/al.h"
#include "AL/alc.h"
#include <plog/Log.h>
#include <stdexcept>

#include "exception.h"
#include "log.h"
#include "sound/file.h"
#include "sound/wave.h"

namespace sound {
Manager::Manager() : device_(nullptr), context_(nullptr), listener_(nullptr) {}

Manager::~Manager() {
  alcMakeContextCurrent(nullptr);
  alcDestroyContext(context_);
  alcCloseDevice(device_);
}

void Manager::Init() {
  // Initialization
  device_ = alcOpenDevice(nullptr);
  if (device_ == nullptr) {
    throw std::runtime_error("Failed to open OpenAL device.");
  }
  context_ = alcCreateContext(device_, nullptr);
  alcMakeContextCurrent(context_);

  // Check for EAX 2.0 support
  auto g_bEAX = alIsExtensionPresent("EAX2.0");
  logErrors();

  listener_ = new Listener();
  ALCint numMono = 0;
  ALCint numStereo = 0;
  alcGetIntegerv(device_, ALC_MONO_SOURCES, 1, &numMono);
  alcGetIntegerv(device_, ALC_STEREO_SOURCES, 1, &numStereo);
  PLOG_DEBUG << "Device and Context initialized. Sources: " << numMono
             << " mono, " << numStereo << " stereo";
}

Source *Manager::GetSource(const std::string &fileName) {
  ALenum error = 0;
  auto *const buffer = GetBuffer(fileName);

  /*Create source*/
  auto *const source = new Source(buffer);

  if ((error = alGetError()) != AL_NO_ERROR) {
    throw engine::Exception("Failed to get AudioSource from " + fileName +
                            ": " + std::to_string(error));
  }
  return source;
}

Buffer *Manager::GetBuffer(const std::string &fileName) {
  std::string cacheKey = fileName;

  /*Is source already loaded*/
  if (const auto itr = bufferCache_.find(cacheKey); itr != bufferCache_.end()) {
    PLOG_DEBUG << "Cached buffer(" << itr->second->getId()
               << ") is used for file " << fileName;
    return itr->second;
  }

  Buffer *newBuffer = loadAudioFile(fileName);
  PLOG_INFO << "New buffer(" << newBuffer->getId() << ") created for file "
            << newBuffer->getName();
  PLOG_DEBUG << "Cache model with key " << cacheKey;
  bufferCache_.insert(std::pair(cacheKey, newBuffer));
  return newBuffer;
}

Buffer *Manager::loadAudioFile(const std::string &filename) {
  const File *file = Wave::LoadFile(filename);
  if (file == nullptr) {
    throw engine::Exception("Error loading audio file " + filename);
  }
  auto *newBuffer = new Buffer(filename);

  newBuffer->setData(file->Data, file->DataSize, file->Format, file->Frequency);

  delete file;
  ALenum error;
  if ((error = alGetError()) != AL_NO_ERROR) {
    throw engine::Exception("Failed to get AudioSource from " + filename +
                            ": " + std::to_string(error));
  }
  return newBuffer;
}

void Manager::logErrors() {
  auto error = alGetError();
  while (error != AL_NO_ERROR) {
    switch (error) {
    case AL_INVALID_ENUM:
      engine::Log::Error("AL_INVALID_ENUM");
      break;
    case AL_INVALID_VALUE:
      engine::Log::Error("AL_INVALID_VALUE");
      break;

    case AL_INVALID_OPERATION:
      engine::Log::Error("AL_INVALID_OPERATION");
      break;
    case AL_OUT_OF_MEMORY:
      engine::Log::Error("AL_OUT_OF_MEMORY");
      break;

    default:
      engine::Log::Error("Unknown error");
      ;
    }
    error = alGetError();
  }
}

Listener *Manager::GetListener() const { return listener_; }
} // namespace sound
