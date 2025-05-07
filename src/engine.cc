#include "engine.h"
#include "exception.h"
#include "input/manager.h"
#include "plog/Formatters/TxtFormatter.h"
#include "plog/Initializers/ConsoleInitializer.h"
#include "plog/Log.h"
#include "sound/openal/manager.h"
#include "video/manager.h"

namespace soil {
    Engine::Engine(const WindowParameter &params) :
        window_(new Window(params)), inputManager_(new input::Manager), videoManager_(new video::Manager),
        soundManager_(new sound::openal::Manager), stageManager_(new stage::Manager) {
        plog::init<plog::TxtFormatter>(plog::debug, plog::OutputStream::streamStdOut);
        PLOG_DEBUG << "init engine";
        if (glfwInit() == 0) {
            const int code = glfwGetError(nullptr);
            std::stringstream stream;
            stream << std::hex << code;
            throw Exception("failed to init GLFW(0x" + stream.str() + ")");
        }

        videoManager_->Init(window_);
        inputManager_->Init(window_);
        soundManager_->Init();
        const auto resources = new stage::Resources(window_, videoManager_, soundManager_, inputManager_);
        stageManager_->Init(window_, inputManager_, resources);
    }

    void Engine::Run() const {
        PLOG_DEBUG << "Run";
        constexpr int ticksPerSecond = 25.0F;
        constexpr float skipTicks = 1.0F / ticksPerSecond;

        auto nextGameTick = 0.0;
        auto loops = 0;
        auto frameTime = 0.0;
        auto frameCounter = 0;
        while (!window_->IsClosed()) {
            constexpr int maxFrameSkip = 5;

            loops = 0;
            auto currentTime = glfwGetTime();
            while (currentTime > nextGameTick && loops < maxFrameSkip) {
                glfwPollEvents();
                inputManager_->Update();
                stageManager_->Update();
                videoManager_->Update();
                if (nextGameTick == 0.0) {
                    nextGameTick = currentTime;
                }
                nextGameTick += skipTicks;
                loops++;
                currentTime = glfwGetTime();
            }
            videoManager_->BeginRender();
            stageManager_->Render(videoManager_->GetState());
            videoManager_->EndRender();
            frameTime += glfwGetTime() - currentTime;
            frameCounter++;
            if (frameTime >= 1.0) {
#ifdef DEBUG
                const auto vertexCount = static_cast<int>(video::shader::Shader::GetVerticeCount());
                const auto drawCount = static_cast<int>(video::shader::Shader::GetDrawCount());
                video::shader::Shader::ResetCounter();
#else
                constexpr auto vertexCount = 0;
                constexpr auto drawCount = 0;
#endif
                // If last was more than 1sec ago
                window_->UpdateStatistics({.fps = frameCounter, .vertexCount = vertexCount, .drawCount = drawCount});
                frameTime = 0;
                frameCounter = 0;
            }
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

    Window *Engine::GetWindow() const { return window_; }

} // namespace soil
