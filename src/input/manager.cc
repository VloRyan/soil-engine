#include "input/manager.h"
#include <cmath>
#include <plog/Log.h>

namespace soil::input {
    std::mutex Manager::mutex_;

    Manager::Manager() : window_(nullptr), eventQueue_(&eventQueueBack1_), cursorPosition_({0.F}) {}

    Manager::~Manager() {
        glfwSetKeyCallback(window_->GetGLFWWindow(), nullptr);
        glfwSetCharCallback(window_->GetGLFWWindow(), nullptr);
        glfwSetMouseButtonCallback(window_->GetGLFWWindow(), nullptr);
        glfwSetScrollCallback(window_->GetGLFWWindow(), nullptr);
    }

    void Manager::Init(Window *window) {
        window_ = window;
        static auto *instance = this;
        // Inputs
        glfwSetKeyCallback(window_->GetGLFWWindow(),
                           [](GLFWwindow *, const int key, const int, const int action, const int) {
                               const auto state = static_cast<Event::State>(action);
                               mutex_.lock();
                               instance->eventQueue_->push_back(Event::KeyChangedEvent(getKey(key), state));
                               mutex_.unlock();
                           });
        glfwSetCharCallback(window_->GetGLFWWindow(), [](GLFWwindow *, const unsigned int codepoint) {
            mutex_.lock();
            instance->eventQueue_->push_back(Event::CharacterEnteredEvent(static_cast<char>(codepoint)));
            mutex_.unlock();
        });
        glfwSetMouseButtonCallback(
            window_->GetGLFWWindow(), [](GLFWwindow *, const int button, const int action, const int) {
                const auto mouseButton = getMouseButton(button);
                const auto state = static_cast<Event::State>(action);
                double x = NAN;
                double y = NAN;
                glfwGetCursorPos(instance->window_->GetGLFWWindow(), &x, &y);
                mutex_.lock();
                instance->eventQueue_->push_back(Event::MouseButtonEvent(mouseButton, state, glm::vec2(x, y)));
                mutex_.unlock();
            });
        glfwSetScrollCallback(window_->GetGLFWWindow(), [](GLFWwindow *, const double xOffset, const double yOffset) {
            mutex_.lock();
            instance->eventQueue_->push_back(Event::MouseWheelEvent(glm::vec2(xOffset, yOffset)));
            mutex_.unlock();
        });
    }

    void Manager::Update() {
        mutex_.lock();
        EventQueue *eventQueueBackBuffer = eventQueue_;
        if (eventQueue_ == &eventQueueBack1_) {
            eventQueue_ = &eventQueueBack2_;
        } else {
            eventQueue_ = &eventQueueBack1_;
        }
        mutex_.unlock();
        if (const auto newCursorPos = window_->GetMouseCursorPos(); cursorPosition_ != newCursorPos) {
            eventQueueBackBuffer->push_back(Event::MousePositionEvent(newCursorPos));
            cursorPosition_ = newCursorPos;
        }
        processEvents(eventQueueBackBuffer);
    }

    void Manager::processEvents(EventQueue *queue) {
        std::vector<Keys> pressedKeys;
        std::vector<Keys> releasedKeys;
        std::vector<MouseButton> pressedButtons;
        std::vector<MouseButton> releasedButtons;

        for (auto itr = queue->begin(); itr != queue->end();) {
            Event event = *itr;
            switch (event.GetOrigin()) {
            case Event::Origin::Keyboard:
                {
                    if (event.GetState() != Event::State::Release) {
                        pressedKeys.push_back(event.GetKey());
                    } else {
                        releasedKeys.push_back(event.GetKey());
                    }
                    break;
                }
            case Event::Origin::MouseButton:
                {
                    if (event.GetState() != Event::State::Release) {
                        pressedButtons.push_back(event.GetMouseButton());
                    } else {
                        releasedButtons.push_back(event.GetMouseButton());
                    }
                    break;
                }
            default:
                {
                    // do nothing
                }
            }
            fire(event);
            ++itr;
        }
        queue->clear();

        for (const Keys pressedKey : pressedKeys) {
            bool isReleasedInThisFrame = false;
            for (const Keys releasedKey : releasedKeys) {
                if (releasedKey == pressedKey) {
                    isReleasedInThisFrame = true;
                    break;
                }
            }
            if (!isReleasedInThisFrame) {
                eventQueue_->push_back(Event::KeyChangedEvent(pressedKey, Event::State::Press));
            }
        }
        for (const MouseButton pressedButton : pressedButtons) {
            bool isReleasedInThisFrame = false;
            for (const MouseButton releasedButton : releasedButtons) {
                if (releasedButton == pressedButton) {
                    isReleasedInThisFrame = true;
                    break;
                }
            }
            if (!isReleasedInThisFrame) {
                double x;
                double y;
                glfwGetCursorPos(window_->GetGLFWWindow(), &x, &y);
                eventQueue_->push_back(Event::MouseButtonEvent(pressedButton, Event::State::Press, glm::vec2(x, y)));
            }
        }
    }

    Keys Manager::getKey(int key) {
        const bool isNumber = key >= static_cast<int>(Keys::Key_0) && key <= static_cast<int>(Keys::Key_9);
        const bool isLetter = !isNumber && key >= static_cast<int>(Keys::A) && key <= static_cast<int>(Keys::Z);
        if (isNumber || isLetter) {
            return static_cast<Keys>(key);
        }
        if (key >= GLFW_KEY_F1 && key <= GLFW_KEY_F12) {
            return static_cast<Keys>(static_cast<int>(Keys::F1) + (key - GLFW_KEY_F1));
        }
        switch (key) {
        case GLFW_KEY_LEFT:
            return Keys::Cursor_Left;
        case GLFW_KEY_RIGHT:
            return Keys::Cursor_Right;
        case GLFW_KEY_UP:
            return Keys::Cursor_Up;
        case GLFW_KEY_DOWN:
            return Keys::Cursor_Down;
        case GLFW_KEY_ESCAPE:
            return Keys::Escape;
        case GLFW_KEY_ENTER:
            return Keys::Enter;
        case GLFW_KEY_BACKSPACE:
            return Keys::Backspace;
        case GLFW_KEY_PAGE_UP:
            return Keys::Page_Up;
        case GLFW_KEY_PAGE_DOWN:
            return Keys::Page_Down;
        case GLFW_KEY_DELETE:
            return Keys::Delete;
        case GLFW_KEY_HOME:
            return Keys::Pos1;
        case GLFW_KEY_END:
            return Keys::End;
        case GLFW_KEY_INSERT:
            return Keys::Insert;
        case GLFW_KEY_KP_ADD:
            return Keys::Key_Plus;
        case GLFW_KEY_KP_SUBTRACT:
            return Keys::Key_Minus;
        case GLFW_KEY_SPACE:
            return Keys::Key_Space;
        case GLFW_KEY_LEFT_SHIFT:
            return Keys::Key_Left_Shift;
        case GLFW_KEY_GRAVE_ACCENT:
            return Keys::Circumflex;
        default:
            return Keys::Unknown;
        }
    }

    MouseButton Manager::getMouseButton(const int button) {
        return static_cast<MouseButton>(static_cast<int>(MouseButton::Left) + (button - GLFW_MOUSE_BUTTON_1));
    }
} // namespace soil::input
