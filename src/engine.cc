#include "engine.h"

#include <GLFW/glfw3.h>

#include "input/manager.h"
#include "plog/Formatters/TxtFormatter.h"
#include "plog/Initializers/ConsoleInitializer.h"
#include "plog/Log.h"
#include "sound/openal/manager.h"
#include "video/manager.h"

namespace soil {
Engine* Engine::INSTANCE_ = nullptr;

Engine::Engine(const Args_t& args)
    : inputManager_(nullptr),
      videoManager_(nullptr),
      soundManager_(nullptr),
      stageManager_(nullptr),
      resources_(nullptr),
      config_(args.Config) {
  if (INSTANCE_ != nullptr) {
    throw std::runtime_error("instance of engine already exits");
  }
  INSTANCE_ = this;
  plog::init<plog::TxtFormatter>(plog::debug, plog::OutputStream::streamStdOut);
  PLOG_DEBUG << "init engine";
  if (glfwInit() == 0) {
    const int code = glfwGetError(nullptr);
    std::stringstream stream;
    stream << std::hex << code;
    throw std::runtime_error("failed to init GLFW(0x" + stream.str() + ")");
  }
  videoManager_ = new video::Manager(args.Context);
  inputManager_ = new input::Manager(*videoManager_->GetWindow());
  soundManager_ = new sound::openal::Manager();
  resources_ =
      new stage::Resources(videoManager_, soundManager_, inputManager_);
  stageManager_ = new stage::Manager(*resources_);
}
Engine::~Engine() {
  delete resources_;
  delete stageManager_;
  delete soundManager_;
  delete videoManager_;
  delete inputManager_;
  glfwTerminate();
}
void Engine::UpdateStatistics(const Statistics& newStats) {
  this->stats_ = newStats;
  const auto event = event::EngineEvent(
      this, event::EngineEvent::CauseType::StatisticsChanged);
  fire(event);
}

const Engine::Statistics& Engine::GetStatistics() const { return stats_; }

void Engine::Run() {
  PLOG_DEBUG << "Run";
  stageManager_->HookTo(*this, *inputManager_, *videoManager_->GetWindow());
  const float skipTicks = 1.0F / static_cast<float>(config_.TicksPerSecond);

  auto nextGameTick = 0.0;
  auto loops = 0;
  auto frameTime = 0.0;
  auto frameCounter = 0;
  auto startLoopTime = 0.0;

  std::chrono::steady_clock::time_point begin;

  auto updateInputTime = 0L;
  auto updateStageTime = 0L;
  auto updateVideoTime = 0L;
  auto updateSoundTime = 0L;
  auto startRenderTime = 0L;
  auto renderTime = 0L;
  auto endRenderTime = 0L;

  while (videoManager_->WindowIsOpen()) {
    loops = 0;
    startLoopTime = glfwGetTime();
    while (startLoopTime > nextGameTick && loops < config_.MaxFrameSkip) {
      glfwPollEvents();
      inputManager_->Update();
      updateInputTime += std::chrono::duration_cast<std::chrono::microseconds>(
                             std::chrono::steady_clock::now() - begin)
                             .count();
      begin = std::chrono::steady_clock::now();
      soundManager_->Update();
      updateSoundTime += std::chrono::duration_cast<std::chrono::microseconds>(
                             std::chrono::steady_clock::now() - begin)
                             .count();
      begin = std::chrono::steady_clock::now();
      stageManager_->Update();
      updateStageTime += std::chrono::duration_cast<std::chrono::microseconds>(
                             std::chrono::steady_clock::now() - begin)
                             .count();
      begin = std::chrono::steady_clock::now();
      videoManager_->Update();
      updateVideoTime += std::chrono::duration_cast<std::chrono::microseconds>(
                             std::chrono::steady_clock::now() - begin)
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
    startRenderTime += std::chrono::duration_cast<std::chrono::microseconds>(
                           std::chrono::steady_clock::now() - begin)
                           .count();

    begin = std::chrono::steady_clock::now();
    stageManager_->Render(videoManager_->GetState());
    renderTime += std::chrono::duration_cast<std::chrono::microseconds>(
                      std::chrono::steady_clock::now() - begin)
                      .count();

    begin = std::chrono::steady_clock::now();
    videoManager_->EndRender();
    endRenderTime += std::chrono::duration_cast<std::chrono::microseconds>(
                         std::chrono::steady_clock::now() - begin)
                         .count();
    frameTime += glfwGetTime() - startLoopTime;
    if (frameTime < 1.0) {
      frameCounter++;
    } else {
      // If last was more than 1sec ago
#ifdef DEBUG
      const auto vertexCount =
          static_cast<int>(video::shader::Program::GetVerticeCount());
      const auto drawCount =
          static_cast<int>(video::shader::Program::GetDrawCount());
      video::shader::Program::ResetCounter();
#else
      constexpr auto VertexCount = 0;
      constexpr auto DrawCount = 0;
#endif
      UpdateStatistics({.FPS = frameCounter,
                        .VertexCount = vertexCount,
                        .DrawCount = drawCount,
                        .StateChanges = videoManager_->GetState().GetChanges(),
                        .updateInputTime = updateInputTime,
                        .updateStageTime = updateStageTime,
                        .updateVideoTime = updateVideoTime,
                        .updateSoundTime = updateSoundTime,
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
}

void Engine::Stop() const { videoManager_->GetWindow()->Close(); }

input::Manager* Engine::GetInputManager() const { return inputManager_; }

video::Manager* Engine::GetVideoManager() const { return videoManager_; }

stage::Manager* Engine::GetStageManager() const { return stageManager_; }

sound::Manager* Engine::GetSoundManager() const { return soundManager_; }

void Engine::Quit() { INSTANCE_->videoManager_->GetWindow()->Close(); }
const Engine::Config_t& Engine::Config() { return INSTANCE_->config_; }

}  // namespace soil
