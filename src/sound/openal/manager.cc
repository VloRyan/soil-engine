
#include "sound/openal/manager.h"

#include <plog/Log.h>

#include <filesystem>
#include <stdexcept>

#include "AL/al.h"
#include "AL/alc.h"
#include "sound/file.h"
#include "sound/ogg.h"
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

sound::Source* Manager::GetSource(const std::string& fileName,
                                  const bool loop) {
  auto* file = GetAudioFile(fileName);
  auto* source = new Source(file, loop);
  source->AddListener(this);
  return source;
}

File* Manager::GetAudioFile(const std::string& fileName) {
  std::string cacheKey = fileName;
  if (const auto itr = fileCache_.find(cacheKey); itr != fileCache_.end()) {
    return itr->second;
  }
  File* file = nullptr;
  const auto ext = std::filesystem::path(fileName).extension();
  if (ext == ".ogg") {
    file = OggFile::Load(fileName);
  } else if (ext == ".wav") {
    file = WaveFile::Load(fileName);
  } else {
    throw std::runtime_error("FileType unsupported");
  }
  PLOG_DEBUG << "Cache sound file with key " << cacheKey;
  fileCache_.insert(std::pair(cacheKey, file));
  return file;
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
    source->Update();
  }
}

}  // namespace soil::sound::openal
