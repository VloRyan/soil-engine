
#include "sound/openal/manager.h"

#include <plog/Log.h>

#include <stdexcept>

#include "AL/al.h"
#include "AL/alc.h"
#include "sound/file.h"
#include "sound/openal/buffer.h"
#include "sound/openal/listener.h"
#include "sound/openal/source.h"
#include "sound/wave.h"
#include "util/strings.h"

namespace soil::sound::openal {
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
  const auto eax2Enabled = alIsExtensionPresent("EAX2.0") == AL_TRUE;
  logErrors();

  listener_ = new Listener();
  ALCint numMono = 0;
  ALCint numStereo = 0;
  alcGetIntegerv(device_, ALC_MONO_SOURCES, 1, &numMono);
  alcGetIntegerv(device_, ALC_STEREO_SOURCES, 1, &numStereo);
  PLOG_DEBUG << "Device and Context initialized. Sources: " << numMono
             << " mono, " << numStereo << " stereo.\n"
             << "Capabilities: \n"
             << "EAX 2.0: " << util::Strings::to_string(eax2Enabled);
}

sound::Source* Manager::GetSource(const std::string& fileName) {
  auto* const buffer = GetBuffer(fileName);

  // Create source
  const auto source = new openal::Source(buffer);

  if (ALenum error = 0; (error = alGetError()) != AL_NO_ERROR) {
    throw std::runtime_error("Failed to get AudioSource from " + fileName +
                             ": " + std::to_string(error));
  }
  source->AddListener(this);
  return source;
}

sound::Buffer* Manager::GetBuffer(const std::string& fileName) {
  std::string cacheKey = fileName;

  // Is source already loaded
  if (const auto itr = bufferCache_.find(cacheKey); itr != bufferCache_.end()) {
    PLOG_DEBUG << "Cached buffer(" << itr->second->getId()
               << ") is used for file " << fileName;
    return itr->second;
  }

  auto newBuffer = loadAudioFile(fileName);
  PLOG_INFO << "New buffer(" << newBuffer->getId() << ") created for file "
            << newBuffer->getName();
  PLOG_DEBUG << "Cache model with key " << cacheKey;
  bufferCache_.insert(std::pair(cacheKey, newBuffer));
  return newBuffer;
}

sound::Buffer* Manager::loadAudioFile(const std::string& filename) {
  const File* file = Wave::LoadFile(filename);
  if (file == nullptr) {
    throw std::runtime_error("Error loading audio file " + filename);
  }
  auto* newBuffer = new Buffer(filename);

  newBuffer->setData(file->Data, file->DataSize, file->Format, file->Frequency);

  delete file;
  if (ALenum error; (error = alGetError()) != AL_NO_ERROR) {
    throw std::runtime_error("Failed to get AudioSource from " + filename +
                             ": " + std::to_string(error));
  }
  return newBuffer;
}

void Manager::logErrors() {
  auto error = alGetError();
  while (error != AL_NO_ERROR) {
    switch (error) {
      case AL_INVALID_ENUM:
        PLOG_ERROR << "AL_INVALID_ENUM";
        break;
      case AL_INVALID_VALUE:
        PLOG_ERROR << "AL_INVALID_VALUE";
        break;
      case AL_INVALID_OPERATION:
        PLOG_ERROR << "AL_INVALID_OPERATION";
        break;
      case AL_OUT_OF_MEMORY:
        PLOG_ERROR << "AL_OUT_OF_MEMORY";
        break;
      default:
        PLOG_ERROR << "Unknown error";
    }
    error = alGetError();
  }
}

sound::Listener* Manager::GetListener() const { return listener_; }

void Manager::Handle(const event::Event& event) {
  switch (event.Cause()) {
    case event::Cause::Source: {
      auto& sourceEvent = dynamic_cast<const event::SourceEvent&>(event);
      auto* source = dynamic_cast<openal::Source*>(sourceEvent.Source());
      switch (sourceEvent.Trigger()) {
        case event::SourceEvent::TriggerType::Added:
          sources_.push_back(source);
          if (sourceEvent.Source()->IsPlaying()) {
            playingSources_.push_back(source);
          }
          break;
        case event::SourceEvent::TriggerType::PlayStateChanged: {
          int playingIndex = -1;
          for (auto i = 0; i < playingSources_.size(); i++) {
            if (playingSources_[i] == sourceEvent.Source()) {
              playingIndex = i;
            }
          }
          if (sourceEvent.Source()->IsPlaying()) {
            if (playingIndex == -1) {
              playingSources_.push_back(source);
            }
          } else {
            playingSources_.erase(playingSources_.begin() + playingIndex);
          }
          break;
        }
        case event::SourceEvent::TriggerType::Removed:
          sourceEvent.Source()->RemoveListener(this);
          if (sourceEvent.Source()->IsPlaying()) {
            for (auto itr = playingSources_.begin();
                 itr != playingSources_.end(); ++itr) {
              if (*itr == sourceEvent.Source()) {
                playingSources_.erase(itr);
                break;
              }
            }
          }
          for (auto itr = sources_.begin(); itr != sources_.end(); ++itr) {
            if (*itr == sourceEvent.Source()) {
              sources_.erase(itr);
              break;
            }
          }
          break;
      }
    }
  }
}

void Manager::Update() {
  std::vector playingSources(playingSources_);
  for (auto* source : playingSources) {
    source->UpdatePlayState();
  }
}
}  // namespace soil::sound::openal
