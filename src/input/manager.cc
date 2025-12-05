#include "input/manager.h"

#include <GLFW/glfw3.h>
#include <plog/Log.h>

#include <cmath>

namespace soil::input {
std::mutex Manager::mutex_;

Manager::Manager(video::Window &window)
    : window_(window), eventQueue_(&eventQueueBack1_), cursorPosition_(0) {
  RegisterCallbacks();
}

Manager::~Manager() {
  window_.SetKeyCallback(nullptr);
  window_.SetKeyCallback(nullptr);
  window_.SetMouseButtonCallback(nullptr);
  window_.SetScrollCallback(nullptr);
}

void Manager::RegisterCallbacks() {
  static auto *instance = this;
  // Inputs
  window_.SetKeyCallback(
      [](const int key, const int, const int action, const int) {
        const auto state = static_cast<Event::StateType>(action);
        mutex_.lock();
        instance->eventQueue_->push_back(
            Event::MakeKeyChangedEvent(getKey(key), state));
        mutex_.unlock();
      });
  window_.SetCharCallback([](const unsigned int codepoint) {
    mutex_.lock();
    instance->eventQueue_->push_back(
        Event::MakeCharacterEnteredEvent(static_cast<char>(codepoint)));
    mutex_.unlock();
  });
  window_.SetMouseButtonCallback(

      [this](const int button, const int action, const int) {
        const auto mouseButton = GetMouseButton(button);
        const auto cursorPosition = window_.GetCursorPos();
        const auto state = static_cast<Event::StateType>(action);
        mutex_.lock();
        instance->eventQueue_->push_back(
            Event::MakeMouseButtonEvent(cursorPosition, mouseButton, state));
        mutex_.unlock();
      });
  window_.SetScrollCallback([this](const double xOffset, const double yOffset) {
    const auto cursorPosition = window_.GetCursorPos();
    mutex_.lock();
    instance->eventQueue_->push_back(Event::MakeMouseWheelEvent(
        cursorPosition, glm::vec2(xOffset, yOffset)));
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
  if (const auto newCursorPos = window_.GetCursorPos();
      cursorPosition_ != newCursorPos) {
    eventQueueBackBuffer->push_back(
        Event::MakeMousePositionEvent(newCursorPos));
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
    switch (event.Origin) {
      case Event::OriginType::Keyboard: {
        if (event.State != Event::StateType::Release) {
          pressedKeys.push_back(event.Key);
        } else {
          releasedKeys.push_back(event.Key);
        }
        break;
      }
      case Event::OriginType::MouseButton: {
        if (event.State != Event::StateType::Release) {
          pressedButtons.push_back(event.MouseButton);
        } else {
          releasedButtons.push_back(event.MouseButton);
        }
        break;
      }
      default: {
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
      eventQueue_->push_back(
          Event::MakeKeyChangedEvent(pressedKey, Event::StateType::Press));
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
      auto cursorPos = window_.GetCursorPos();
      eventQueue_->push_back(Event::MakeMouseButtonEvent(
          cursorPos, pressedButton, Event::StateType::Press));
    }
  }
}

Keys Manager::getKey(int key) {
  const bool isNumber = key >= static_cast<int>(Keys::Key_0) &&
                        key <= static_cast<int>(Keys::Key_9);
  const bool isLetter = !isNumber && key >= static_cast<int>(Keys::A) &&
                        key <= static_cast<int>(Keys::Z);
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
    case GLFW_KEY_TAB:
      return Keys::Tab;
    default:
      return Keys::Unknown;
  }
}

MouseButton Manager::GetMouseButton(const int button) {
  return static_cast<MouseButton>(static_cast<int>(MouseButton::Left) +
                                  (button - GLFW_MOUSE_BUTTON_1));
}
}  // namespace soil::input
