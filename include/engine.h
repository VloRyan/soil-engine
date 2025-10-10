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
  struct Config_t {
    int TicksPerSecond{25};
    int MaxFrameSkip{5};
  };

  struct Args_t {
    Config_t Config{};
    WindowParameter Window{};
  };

  explicit Engine(const Args_t& args);

  ~Engine() = default;

  void Run() const;

  void Stop() const;

  [[nodiscard]] input::Manager* GetInputManager() const;

  [[nodiscard]] video::Manager* GetVideoManager() const;

  [[nodiscard]] stage::Manager* GetStageManager() const;

  [[nodiscard]] sound::Manager* GetSoundManager() const;

  [[nodiscard]] Window* GetWindow() const;

  Engine(Engine const&) = delete;

  void operator=(Engine const&) = delete;

  static void Quit();
  static const Config_t& Config();

 private:
  Window* window_;
  stage::Resources* resources_;
  input::Manager* inputManager_;
  video::Manager* videoManager_;
  sound::Manager* soundManager_;
  stage::Manager* stageManager_;
  Config_t config_;
  static Engine* INSTANCE_;
};
}  // namespace soil

#endif
