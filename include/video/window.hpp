#ifndef SOIL_VIDEO_WINDOW_HPP
#define SOIL_VIDEO_WINDOW_HPP
#include <bitset>
#include <glm/glm.hpp>
#include <string>

#include "event/observable.hpp"
#include "video/event/window_event.h"

namespace soil::video {
class Window
    : public ::soil::event::Observable<soil::video::event::WindowEvent> {
 public:
  enum class State : std::uint8_t {
    Closed = 0,
    Maximized,
    Minimized,
    Fullscreen,
    Focused
  };

  virtual ~Window() = default;
  [[nodiscard]] virtual inline bool IsWindowed() const;
  [[nodiscard]] virtual bool IsClosed() const;
  [[nodiscard]] virtual bool HasState(State stateFlag) const;
  virtual void Close();
  [[nodiscard]] virtual const glm::ivec2& GetSize() const;
  virtual void SetTitle(const std::string& title);

  virtual void SetKeyCallback(
      const std::function<void(int key, int scancode, int action, int mods)>&
          keyCallback);
  virtual void SetCharCallback(
      const std::function<void(uint codepoint)>& charCallback);
  virtual void SetMouseButtonCallback(
      const std::function<void(int button, int action, int mods)>&
          mouseButtonCallback);
  virtual void SetScrollCallback(
      const std::function<void(double xOffset, double yOffset)>&
          scrollCallback);

  [[nodiscard]] virtual glm::vec2 CenterMouseCursor() const = 0;
  [[nodiscard]] virtual glm::vec2 GetMouseCursorPos() const = 0;
  virtual glm::ivec2 GetCursorPos() = 0;

 protected:
  explicit Window() = default;

  std::string title_;
  std::bitset<5> stateFlags_;
  glm::ivec2 size_;

  std::function<void(int key, int scancode, int action, int mods)> keyCallback_;
  std::function<void(uint codepoint)> charCallback_;
  std::function<void(int button, int action, int mods)> mouseButtonCallback_;
  std::function<void(double xOffset, double yOffset)> scrollCallback_;
};

}  // namespace soil::video
#endif
