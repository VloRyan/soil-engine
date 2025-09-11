#include "engine.h"
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
            throw std::runtime_error("failed to init GLFW(0x" + stream.str() + ")");
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
        auto startLoopTime = 0.0;

        std::chrono::steady_clock::time_point begin;

        auto updateInputTime = 0L;
        auto updateStageTime = 0L;
        auto updateVideoTime = 0L;
        auto startRenderTime = 0L;
        auto renderTime = 0L;
        auto endRenderTime = 0L;

        while (!window_->IsClosed()) {
            constexpr int maxFrameSkip = 5;
            loops = 0;
            startLoopTime = glfwGetTime();
            while (startLoopTime > nextGameTick && loops < maxFrameSkip) {
                glfwPollEvents();
                inputManager_->Update();
                updateInputTime +=
                    std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - begin)
                        .count();
                begin = std::chrono::steady_clock::now();
                stageManager_->Update();
                updateStageTime +=
                    std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - begin)
                        .count();
                begin = std::chrono::steady_clock::now();
                videoManager_->Update();
                updateVideoTime +=
                    std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - begin)
                        .count();

                if (nextGameTick == 0.0) {
                    nextGameTick = startLoopTime;
                }
                nextGameTick += skipTicks;
                loops++;
                startLoopTime = glfwGetTime();
            }
            begin = std::chrono::steady_clock::now();
            videoManager_->BeginRender();
            startRenderTime +=
                std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - begin).count();

            begin = std::chrono::steady_clock::now();
            stageManager_->Render(videoManager_->GetState());
            renderTime +=
                std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - begin).count();

            begin = std::chrono::steady_clock::now();
            videoManager_->EndRender();
            endRenderTime +=
                std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - begin).count();
            frameTime += glfwGetTime() - startLoopTime;
            if (frameTime < 1.0) {
                frameCounter++;
            } else {
                // If last was more than 1sec ago
#ifdef DEBUG
                const auto vertexCount = static_cast<int>(video::shader::Shader::GetVerticeCount());
                const auto drawCount = static_cast<int>(video::shader::Shader::GetDrawCount());
                video::shader::Shader::ResetCounter();
#else
                constexpr auto VertexCount = 0;
                constexpr auto DrawCount = 0;
#endif
                window_->UpdateStatistics({.FPS = frameCounter,
                                           .VertexCount = vertexCount,
                                           .DrawCount = drawCount,
                                           .StateChanges = videoManager_->GetState().GetChanges(),
                                           .updateInputTime = updateInputTime,
                                           .updateStageTime = updateStageTime,
                                           .updateVideoTime = updateVideoTime,
                                           .startRenderTime = startRenderTime,
                                           .renderTime = renderTime,
                                           .endRenderTime = endRenderTime});
                frameTime = 0;
                frameCounter = 0;

                updateInputTime = 0;
                updateStageTime = 0;
                updateVideoTime = 0;
                startRenderTime = 0;
                renderTime = 0;
                endRenderTime = 0;

                videoManager_->GetState().ResetChangeCounter();
            }
        }
        PLOG_DEBUG << "Stop engine";
        delete stageManager_;
        delete soundManager_;
        delete videoManager_;
        delete inputManager_;
        glfwTerminate();
    }

    void Engine::Stop() const {
        window_->Close();
    }

    input::Manager *Engine::GetInputManager() const {
        return inputManager_;
    }

    video::Manager *Engine::GetVideoManager() const {
        return videoManager_;
    }

    stage::Manager *Engine::GetStageManager() const {
        return stageManager_;
    }

    sound::Manager *Engine::GetSoundManager() const {
        return soundManager_;
    }

    Window *Engine::GetWindow() const {
        return window_;
    }

} // namespace soil
