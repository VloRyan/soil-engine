#include "video/glfw_window.h"

#include <ios>
#include <sstream>

namespace soil::video {
GLFWWindow::GLFWWindow(GLFWwindow *window) : window_(window) {
  registerCallbacks();
  glfwGetWindowSize(window, &size_.x, &size_.y);
}

GLFWWindow::~GLFWWindow() {
  glfwSetKeyCallback(window_, nullptr);
  glfwSetCharCallback(window_, nullptr);
  glfwSetMouseButtonCallback(window_, nullptr);
  glfwSetScrollCallback(window_, nullptr);
}

void GLFWWindow::Close() {
  Window::Close();
  glfwSetWindowShouldClose(window_, GLFW_TRUE);
}

void GLFWWindow::registerCallbacks() {
  static auto *instance = this;
  glfwSetFramebufferSizeCallback(
      window_, [](GLFWwindow *, const int width, const int height) {
        instance->size_ = glm::ivec2(width, height);
        const auto event = event::WindowEvent(
            instance, event::WindowEvent::CauseType::SizeChanged);
        instance->fire(event);
      });

  glfwSetWindowIconifyCallback(window_, [](GLFWwindow *, const int iconified) {
    instance->stateFlags_[static_cast<short>(Window::State::Minimized)] =
        iconified != 0;
  });

  glfwSetWindowFocusCallback(window_, [](GLFWwindow *, const int focused) {
    instance->stateFlags_[static_cast<short>(Window::State::Focused)] =
        focused != 0;
  });

  glfwSetWindowCloseCallback(window_, [](GLFWwindow *) {
    instance->stateFlags_ = static_cast<short>(Window::State::Closed);
  });

  glfwSetKeyCallback(
      window_, [](GLFWwindow *, int key, int scancode, int action, int mods) {
        if (instance->keyCallback_ != nullptr) {
          instance->keyCallback_(key, scancode, action, mods);
        }
      });

  glfwSetCharCallback(window_, [](GLFWwindow *, unsigned int codepoint) {
    if (instance->charCallback_ != nullptr) {
      instance->charCallback_(codepoint);
    }
  });

  glfwSetMouseButtonCallback(
      window_, [](GLFWwindow *, int button, int action, int mods) {
        if (instance->mouseButtonCallback_ != nullptr) {
          instance->mouseButtonCallback_(button, action, mods);
        }
      });

  glfwSetScrollCallback(window_, [](GLFWwindow *, double x, double y) {
    if (instance->scrollCallback_ != nullptr) {
      instance->scrollCallback_(x, y);
    }
  });
}

glm::vec2 GLFWWindow::CenterMouseCursor() const {
  glm::uvec2 centerPosition;
  centerPosition.x = size_.x / 2;
  centerPosition.y = size_.y / 2;
  glfwSetCursorPos(window_, centerPosition.x, centerPosition.y);
  return centerPosition;
}

glm::vec2 GLFWWindow::GetMouseCursorPos() const {
  glm::dvec2 pos;
  glfwGetCursorPos(window_, &pos[0], &pos[1]);
  return pos;
}

void GLFWWindow::SetTitle(const std::string &title) {
  Window::SetTitle(title);
  glfwSetWindowTitle(window_, title.c_str());
}

glm::ivec2 GLFWWindow::GetCursorPos() {
  double x = NAN;
  double y = NAN;
  glfwGetCursorPos(window_, &x, &y);
  return {static_cast<int>(std::floor(x)), static_cast<int>(std::floor(y))};
}

}  // namespace soil::video
