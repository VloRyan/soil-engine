#include "video/window.hpp"
namespace soil::video {
bool Window::IsWindowed() const { return !HasState(State::Fullscreen); }

bool Window::IsClosed() const { return HasState(State::Closed); }

bool Window::HasState(State stateFlag) const {
  return stateFlags_[static_cast<short>(stateFlag)] == true;
}
void Window::Close() { stateFlags_[static_cast<int>(State::Closed)] = true; }
const glm::ivec2& Window::GetSize() const { return size_; };
void Window::SetTitle(const std::string& title) { title_ = title; }

void Window::SetKeyCallback(
    const std::function<void(int key, int scancode, int action, int mods)>&
        keyCallback) {
  keyCallback_ = keyCallback;
}
void Window::SetCharCallback(
    const std::function<void(uint codepoint)>& charCallback) {
  charCallback_ = charCallback;
}
void Window::SetMouseButtonCallback(
    const std::function<void(int button, int action, int mods)>&
        mouseButtonCallback) {
  mouseButtonCallback_ = mouseButtonCallback;
}
void Window::SetScrollCallback(
    const std::function<void(double xOffset, double yOffset)>& scrollCallback) {
  scrollCallback_ = scrollCallback;
}

}  // namespace soil::video
