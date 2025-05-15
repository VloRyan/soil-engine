
#ifndef SOIL_SOUND_OPENAL_MANAGER_H
#define SOIL_SOUND_OPENAL_MANAGER_H

#include "base.h"
#include "sound/manager.h"

class ALCdevice;
class ALCcontext;

namespace soil::sound::openal {
    class Manager final : public sound::Manager {
    public:
        Manager();

        ~Manager() override;

        void Init() override;

        Source *GetSource(const std::string &fileName) override;

        Buffer *GetBuffer(const std::string &fileName) override;
        [[nodiscard]] Listener *GetListener() const override;

    private:
        static Buffer *loadAudioFile(const std::string &filename);

        static void logErrors();

        ALCdevice *device_;
        ALCcontext *context_;
        Listener *listener_;
        HashMap<std::string, Buffer *> bufferCache_;
    };
} // namespace soil::sound::openal

#endif // SOIL_SOUND_OPENAL_MANAGER_H
