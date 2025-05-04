#include "engine.h"
#include <plog/Formatters/TxtFormatter.h>
#include <plog/Initializers/ConsoleInitializer.h>
#include <plog/Log.h>
#include "exception.h"
#include "input/manager.h"
#include "video/manager.h"

namespace engine
{
    Engine::Engine(const WindowParameter &params) :
        window_(std::make_unique<Window>(Window(params))), inputManager_(new input::Manager),
        videoManager_(new video::Manager), soundManager_(new sound::Manager),
        stageManager_(new stage::Manager(videoManager_, soundManager_, inputManager_))
    {
        plog::init<plog::TxtFormatter>(plog::debug, plog::OutputStream::streamStdOut);
        PLOG_DEBUG << "init engine";
        if (glfwInit() == 0)
        {
            const int code = glfwGetError(nullptr);
            std::stringstream stream;
            stream << std::hex << code;
            throw Exception("failed to init GLFW(0x" + stream.str() + ")");
        }

        window_ = std::make_unique<Window>(Window(params));

        videoManager_->Init(window_.get());
        inputManager_->Init(window_.get());
        soundManager_->Init();
        stageManager_->Init(window_.get());
    }

    void Engine::Run() const
    {
        PLOG_DEBUG << "Run";
        constexpr int ticksPerSecond = 25.0F;
        constexpr float skipTicks = 1.0F / ticksPerSecond;

        auto nextGameTick = 0.0;
        // float interpolation;
        auto loops = 0;
        auto frameTime = 0.0;
        auto frameCounter = 0;
        while (!window_->IsClosed())
        {
            constexpr int maxFrameSkip = 5;
            // limiter.WaitForFrame();

            loops = 0;
            auto currentTime = glfwGetTime();
            while (currentTime > nextGameTick && loops < maxFrameSkip)
            {
                // constexpr auto timeElapsed = skipTicks;
                // //static_cast<float>(currentTime - startTimeLastFrame);
                glfwPollEvents();
                inputManager_->Update();
                stageManager_->Update();
                videoManager_->Update();
                // soundManager_->Update();
                if (nextGameTick == 0.0)
                {
                    nextGameTick = currentTime;
                }
                nextGameTick += skipTicks;
                loops++;
                currentTime = glfwGetTime();
            }
            videoManager_->BeginRender();
            stageManager_->Render(videoManager_->GetState());
            videoManager_->EndRender();
#ifdef DEBUG
            const auto vertexCount = static_cast<int>(video::shader::Shader::GetVerticeCount());
            const auto drawCount = static_cast<int>(video::shader::Shader::GetDrawCount());
            video::shader::Shader::ResetCounter();
#else
            constexpr auto vertexCount = 0;
            constexpr auto drawCount = 0;
#endif
            frameTime += glfwGetTime() - currentTime;
            frameCounter++;
            if (frameTime >= 1.0)
            {
                // If last was more than 1sec ago
                window_->UpdateStatistics({.fps = frameCounter, .vertexCount = vertexCount, .drawCount = drawCount});
                frameTime = 0;
                frameCounter = 0;
            }
            // limiter.EndFrame();
        }
        PLOG_DEBUG << "Stop engine";
        delete stageManager_;
        delete soundManager_;
        delete videoManager_;
        delete inputManager_;
        glfwTerminate();
    }

    void Engine::Stop() const { window_->Close(); }

    input::Manager *Engine::GetInputManager() const { return inputManager_; }

    video::Manager *Engine::GetVideoManager() const { return videoManager_; }

    stage::Manager *Engine::GetStageManager() const { return stageManager_; }

    sound::Manager *Engine::GetSoundManager() const { return soundManager_; }

    Window *Engine::GetWindow() const { return window_.get(); }

    int Engine::GetDesiredFps() const { return 0; } // limiter.GetFPSLimit(); }

    void Engine::SetDesiredFps(const int desired_fps)
    {
        // limiter.SetFPSLimit(desired_fps);
    }

    int Engine::GetCurrentFPS() const
    {
        // return limiter.GetCurrentFPS();
        // return fps_;
        return 0;
    }
} // namespace engine
