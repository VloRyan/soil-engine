#include "window.h"

#include <ios>
#include <sstream>

#include "exception.h"

namespace soil {
    Window::Window(const WindowParameter &args) :
        parameter_(args), stateFlags_(0), glfwWindow_(nullptr), statistics_() {}

    void Window::Open() {
        glfwWindowHint(GLFW_SAMPLES, 1);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, parameter_.OpenGLVersion[0]);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, parameter_.OpenGLVersion[1]);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);

        if (HasState(WindowState::Maximized)) {
            glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
        }

#ifdef DEBUG
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif
        GLFWmonitor *monitor = nullptr;
        if (parameter_.Type == WindowType::Fullscreen) {
            monitor = glfwGetPrimaryMonitor();
        }
        glfwWindow_ = glfwCreateWindow(parameter_.Size.x, parameter_.Size.y, title_.c_str(), monitor, nullptr);
        if (glfwWindow_ == nullptr) {
            const int code = glfwGetError(nullptr);
            std::stringstream stream;
            stream << std::hex << code;
            throw Exception("Create window failed(0x" + stream.str() + ")");
        }
        glfwMakeContextCurrent(glfwWindow_);
        stateFlags_ = static_cast<short>(WindowState::Open) + static_cast<short>(WindowState::Focused);
        glfwSetWindowTitle(glfwWindow_, title_.c_str());

        registerCallbacks();

        glfwSwapInterval(0);
    }

    void Window::Close() {
        glfwSetWindowShouldClose(glfwWindow_, GLFW_TRUE);
        stateFlags_ = static_cast<short>(WindowState::Closed);
    }

    bool Window::IsClosed() const { return stateFlags_ == static_cast<short>(WindowState::Closed); }

    void Window::SwapBuffers() const { glfwSwapBuffers(glfwWindow_); }

    bool Window::IsWindowed() const { return parameter_.Type == WindowType::Windowed; }

    bool Window::HasState(WindowState stateFlag) const { return stateFlags_[static_cast<short>(stateFlag)]; }

    void Window::registerCallbacks() {
        static auto *instance = this;
        glfwSetFramebufferSizeCallback(glfwWindow_, [](GLFWwindow *, const int width, const int height) {
            instance->parameter_.Size = glm::ivec2(width, height);
            const auto aspect = static_cast<float>(width) / static_cast<float>(height);
            instance->parameter_.RenderSizeAspect = glm::ivec2(
                static_cast<float>(instance->parameter_.RenderSize.y) * aspect, instance->parameter_.RenderSize.y);
            const auto event = WindowEvent(instance, WindowEvent::Cause::SizeChanged);
            instance->fire(event);
        });
        glfwSetWindowIconifyCallback(glfwWindow_, [](GLFWwindow *, const int iconified) {
            instance->stateFlags_[static_cast<short>(WindowState::Minimized)] = iconified != 0;
        });
        glfwSetWindowFocusCallback(glfwWindow_, [](GLFWwindow *, const int focused) {
            instance->stateFlags_[static_cast<short>(WindowState::Focused)] = focused != 0;
        });
        glfwSetWindowCloseCallback(
            glfwWindow_, [](GLFWwindow *) { instance->stateFlags_ = static_cast<short>(WindowState::Closed); });
    }

    GLFWwindow *Window::GetGLFWWindow() const { return glfwWindow_; }

    const glm::ivec2 &Window::GetSize() const { return parameter_.Size; }

    const glm::ivec2 &Window::GetRenderSize() const { return parameter_.RenderSizeAspect; }

    glm::vec2 Window::CenterMouseCursor() const {
        glm::uvec2 centerPosition;
        centerPosition.x = parameter_.Size.x / 2;
        centerPosition.y = parameter_.Size.y / 2;
        glfwSetCursorPos(glfwWindow_, centerPosition.x, centerPosition.y);
        return centerPosition;
    }

    glm::vec2 Window::GetMouseCursorPos() const {
        glm::dvec2 pos;
        glfwGetCursorPos(glfwWindow_, &pos[0], &pos[1]);
        return pos;
    }

    void Window::SetParameter(const WindowParameter &params) {
        GLFWmonitor *monitor = nullptr;
        if (params.Type == WindowType::Fullscreen) {
            monitor = glfwGetPrimaryMonitor();
        }
        glfwSetWindowMonitor(glfwWindow_, monitor, 0, 0, params.Size.x, params.Size.y, params.RefreshRate);
        parameter_ = params;
        const auto event = WindowEvent(this, WindowEvent::Cause::SizeChanged);
        fire(event);
    }

    void Window::SetTitle(const std::string &title) const { glfwSetWindowTitle(glfwWindow_, title.c_str()); }

    const WindowParameter &Window::GetParameter() const { return parameter_; }

    void Window::UpdateStatistics(const Statistics &newStats) {
        this->statistics_ = newStats;
        const auto event = WindowEvent(this, WindowEvent::Cause::StatisticsChanged);
        fire(event);
    }

    const Statistics &Window::GetStatistics() const { return statistics_; }
} // namespace soil
