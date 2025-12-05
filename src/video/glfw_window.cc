#include "video/glfw_window.h"

#include <ios>
#include <sstream>

namespace soil::video {
GLFWWindow::GLFWWindow(GLFWwindow *window) : window_(window) {
  registerCallbacks();
}

GLFWWindow::~GLFWWindow() {
  glfwSetKeyCallback(window_, nullptr);
  glfwSetCharCallback(window_, nullptr);
  glfwSetMouseButtonCallback(window_, nullptr);
  glfwSetScrollCallback(window_, nullptr);
}

/*
void Window::Open() {
  glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
  glfwWindowHint(GLFW_SAMPLES, GLFW_DONT_CARE);
  glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_EGL_CONTEXT_API);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, parameter_.OpenGLVersion[0]);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, parameter_.OpenGLVersion[1]);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);
  glfwWindowHint(GLFW_CONTEXT_ROBUSTNESS, GLFW_NO_ROBUSTNESS);
  glfwWindowHint(GLFW_CONTEXT_RELEASE_BEHAVIOR, GLFW_ANY_RELEASE_BEHAVIOR);
  glfwWindowHint(GLFW_CONTEXT_NO_ERROR, GLFW_FALSE);

  if (HasState(WindowState::Maximized)) {
    glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
  }

#ifdef DEBUG
  glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#else
  glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_FALSE);
#endif
  GLFWmonitor *monitor = nullptr;
  if (parameter_.Type == WindowType::Fullscreen) {
    monitor = glfwGetPrimaryMonitor();
  }
  window_ = glfwCreateWindow(parameter_.Size.x, parameter_.Size.y,
                                 title_.c_str(), monitor, nullptr);
  if (window_ == nullptr) {
    const int code = glfwGetError(nullptr);
    std::stringstream stream;
    stream << std::hex << code;
    throw std::runtime_error("Create window failed(0x" + stream.str() + ")");
  }
  glfwMakeContextCurrent(window_);
  glfwSwapInterval(0);

  stateFlags_ = static_cast<short>(WindowState::Open) +
                static_cast<short>(WindowState::Focused);
  glfwSetWindowTitle(window_, title_.c_str());
  registerCallbacks();
}
*/
void GLFWWindow::Close() {
  Window::Close();
  glfwSetWindowShouldClose(window_, GLFW_TRUE);
}

void GLFWWindow::registerCallbacks() {
  static auto *instance = this;
  glfwSetFramebufferSizeCallback(
      window_, [](GLFWwindow *, const int width, const int height) {
        instance->size_ = glm::ivec2(width, height);
        // const auto aspect = static_cast<float>(width) /
        // static_cast<float>(height);
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
