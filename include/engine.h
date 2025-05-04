#ifndef ENGINE_ENGINE_H
#define ENGINE_ENGINE_H

// #include "frame_limiter.h"
#include <memory>
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

namespace engine
{
    class Engine
    {
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

        [[nodiscard]] int GetDesiredFps() const;

        void SetDesiredFps(int desired_fps);

        [[nodiscard]] int GetCurrentFPS() const;

    private:
        std::unique_ptr<Window> window_;
        input::Manager *inputManager_;
        video::Manager *videoManager_;
        sound::Manager *soundManager_;
        stage::Manager *stageManager_;
        // FrameLimiter limiter;
        //  int fps_;
    };
} // namespace engine

#endif // ENGINE_ENGINE_H
