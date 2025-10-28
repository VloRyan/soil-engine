#ifndef SOIL_INPUT_MANAGER_H_
#define SOIL_INPUT_MANAGER_H_

#include <mutex>

#include "event.h"
#include "event/observable.hpp"
#include "window.h"

namespace soil::input {
using EventQueue = std::vector<Event>;

class Manager final : public event::Observable<Event> {
 public:
  Manager();

  ~Manager() override;

  void Update();

  void Init(Window* window);

 protected:
  void processEvents(EventQueue* queue);

  static MouseButton GetMouseButton(int button);

  static Keys getKey(int key);

  static glm::ivec2 GetCursorPosition(GLFWwindow* window);

 private:
  static std::mutex mutex_;
  Window* window_;
  EventQueue* eventQueue_;
  EventQueue eventQueueBack1_;
  EventQueue eventQueueBack2_;
  glm::ivec2 cursorPosition_;
};
}  // namespace soil::input

#endif
