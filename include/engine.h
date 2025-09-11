#ifndef SOIL_ENGINE_H
#define SOIL_ENGINE_H

#include "input/manager.h"
#include "sound/manager.h"
#include "stage/manager.h"
#include "video/manager.h"
#include "window.h"

#ifdef _WIN32
#define ENGINE_EXPORT __declspec(dllexport)
#else
#define ENGINE_EXPORT
#endif

namespace soil {
    class Engine {
    public:
        explicit Engine(const WindowParameter &params);

        ~Engine() = default;

        void Run() const;

        void Stop() const;

        [[nodiscard]] input::Manager *GetInputManager() const;

        [[nodiscard]] video::Manager *GetVideoManager() const;

        [[nodiscard]] stage::Manager *GetStageManager() const;

        [[nodiscard]] sound::Manager *GetSoundManager() const;

        [[nodiscard]] Window *GetWindow() const;

        Engine(Engine const &) = delete;

        void operator=(Engine const &) = delete;

    private:
        Window *window_;
        input::Manager *inputManager_;
        video::Manager *videoManager_;
        sound::Manager *soundManager_;
        stage::Manager *stageManager_;
    };
} // namespace soil

#endif
