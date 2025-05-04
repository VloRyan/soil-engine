#ifndef ENGINE_ENGINE_INPUT_MANAGER_H_
#define ENGINE_ENGINE_INPUT_MANAGER_H_

#include "event.h"
#include "event/observable.hpp"
#include "window.h"
#include <mutex>

namespace input {
using EventQueue = std::vector<Event>;

class Manager : public event::Observable<Event> {
public:
  Manager();

  ~Manager() override;

  void Update();

  void Init(engine::Window *window);

protected:
  void processEvents(EventQueue *queue);

  static MouseButton getMouseButton(int button);

  static Keys getKey(int key);

private:
  static std::mutex mutex_;
  engine::Window *window_;
  EventQueue *eventQueue_;
  EventQueue eventQueueBack1_;
  EventQueue eventQueueBack2_;
};
} // namespace input

#endif // ENGINE_ENGINE_INPUT_MANAGER_H_
