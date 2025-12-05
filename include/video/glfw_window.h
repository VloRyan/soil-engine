#ifndef SOIL_VIDEO_GLFW_WINDOW_H
#define SOIL_VIDEO_GLFW_WINDOW_H

#include <GLFW/glfw3.h>

#include "window.hpp"
namespace soil::video {

class GLFWWindow final : public Window {
 public:
  GLFWWindow(GLFWwindow* window);
  ~GLFWWindow() override;

  void Close() override;
  [[nodiscard]] glm::vec2 CenterMouseCursor() const override;
  [[nodiscard]] glm::vec2 GetMouseCursorPos() const override;
  void SetTitle(const std::string& title) override;
  glm::ivec2 GetCursorPos() override;

 protected:
  void registerCallbacks();

 private:
  GLFWwindow* window_;
};
}  // namespace soil::video
#endif
