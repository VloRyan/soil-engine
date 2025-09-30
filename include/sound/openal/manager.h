#ifndef SOIL_SOUND_OPENAL_MANAGER_H
#define SOIL_SOUND_OPENAL_MANAGER_H

#include <vector>

#include "base.h"
#include "sound/manager.h"
#include "source.h"

class ALCdevice;
class ALCcontext;

namespace soil::sound::openal {
class Manager final : public sound::Manager, public event::EventHandler {
 public:
  Manager();

  ~Manager() override;

  void Init() override;

  sound::Source* GetSource(const std::string& fileName) override;

  Buffer* GetBuffer(const std::string& fileName) override;

  [[nodiscard]] Listener* GetListener() const override;

  void Handle(const event::Event& event) override;

  void Update() override;

 private:
  static Buffer* loadAudioFile(const std::string& filename);

  static void logErrors();

  ALCdevice* device_;
  ALCcontext* context_;
  Listener* listener_;
  HashMap<std::string, Buffer*> bufferCache_;
  std::vector<openal::Source*> sources_;
  std::vector<openal::Source*> playingSources_;
};
}  // namespace soil::sound::openal

#endif
