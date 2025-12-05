#ifndef SOIL_ENGINE_H
#define SOIL_ENGINE_H

#include "event/engine_event.h"
#include "input/manager.h"
#include "sound/manager.h"
#include "stage/manager.h"
#include "video/glfw_window.h"
#include "video/manager.h"

#ifdef _WIN32
#define ENGINE_EXPORT __declspec(dllexport)
#else
#define ENGINE_EXPORT
#endif

namespace soil {
class Engine : public event::Observable<event::EngineEvent> {
 public:
  struct Statistics {
    int FPS;
    int VertexCount;
    int DrawCount;
    int StateChanges;

    long updateInputTime = 0L;
    long updateStageTime = 0L;
    long updateVideoTime = 0L;
    long updateSoundTime = 0L;
    long startRenderTime = 0L;
    long renderTime = 0L;
    long endRenderTime = 0L;
  };

  struct Config_t {
    int TicksPerSecond{25};
    int MaxFrameSkip{5};
  };

  struct WindowParameter {
    glm::ivec2 Size;
  };
  struct ContextCreateParameter {
    WindowParameter Window;
    glm::ivec2 OpenGLVersion;
  };

  struct Args_t {
    Config_t Config{};
    video::Context::CreateParameter Context{};
  };

  explicit Engine(const Args_t& args);

  ~Engine();

  void Run();

  void Stop() const;

  [[nodiscard]] input::Manager* GetInputManager() const;
  [[nodiscard]] video::Manager* GetVideoManager() const;
  [[nodiscard]] stage::Manager* GetStageManager() const;
  [[nodiscard]] sound::Manager* GetSoundManager() const;
  Engine(Engine const&) = delete;
  void operator=(Engine const&) = delete;
  static void Quit();
  static const Config_t& Config();

  const Statistics& GetStatistics() const;

 private:
  void UpdateStatistics(const Statistics& newStats);
  input::Manager* inputManager_;
  video::Manager* videoManager_;
  sound::Manager* soundManager_;
  stage::Manager* stageManager_;
  Config_t config_;
  Statistics stats_;
  static Engine* INSTANCE_;
  stage::Resources* resources_;
};
}  // namespace soil

#endif
