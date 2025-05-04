#ifndef ENGINE_ENGINE_VIDEO_WINDOW_H_
#define ENGINE_ENGINE_VIDEO_WINDOW_H_

// #define GLFW_INCLUDE_NONE
#include <bitset>
#include <string>
#include "GLFW/glfw3.h"
#include "event/observable.hpp"
#include "glm/vec2.hpp"
#include "window_event.h"

#define OPEN_GL_MAJOR_VERSION 3
#define OPEN_GL_MINOR_VERSION 3

namespace engine
{
    struct Statistics
    {
        int fps;
        int vertexCount;
        int drawCount;
    };

    enum class WindowType : std::uint8_t
    {
        Fullscreen = 0,
        Windowed
    };

    enum class WindowState : std::uint8_t
    {
        Closed = 0,
        Open,
        Maximized,
        Minimized,
        Focused
    };

    struct WindowParameter
    {
        glm::ivec2 Size{800, 600};
        glm::ivec2 RenderSize{640, 480};
        glm::ivec2 RenderSizeAspect{640, 480};
        WindowType Type{WindowType::Windowed};
        int RefreshRate{GLFW_DONT_CARE};
        glm::ivec2 OpenGLVersion{OPEN_GL_MAJOR_VERSION, OPEN_GL_MINOR_VERSION};
    };

    class Window final : public ::event::Observable<WindowEvent>
    {
    public:
        explicit Window(const WindowParameter &args);

        void Open();

        [[nodiscard]] inline bool IsWindowed() const;

        void SwapBuffers() const;

        [[nodiscard]] bool IsClosed() const;

        [[nodiscard]] bool HasState(WindowState stateFlag) const;

        void Close();

        [[nodiscard]] GLFWwindow *GetGLFWWindow() const;

        [[nodiscard]] glm::vec2 CenterMouseCursor() const;

        [[nodiscard]] glm::vec2 GetMouseCursorPos() const;

        [[nodiscard]] const glm::ivec2 &GetSize() const;

        [[nodiscard]] const glm::ivec2 &GetRenderSize() const;

        void SetParameter(const WindowParameter &params);

        [[nodiscard]] const WindowParameter &GetParameter() const;

        void SetTitle(const std::string &title) const;

        void UpdateStatistics(const Statistics &newStats);

        [[nodiscard]] const Statistics &GetStatistics() const;

    protected:
        void registerCallbacks();

    private:
        std::string title_;
        WindowParameter parameter_;
        std::bitset<4> stateFlags_;
        GLFWwindow *glfwWindow_;
        Statistics statistics_;
    };
} // namespace engine
#endif // ENGINE_ENGINE_VIDEO_WINDOW_H_
