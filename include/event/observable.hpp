#ifndef ENGINE_EVENT_OBSERVABLE_H
#define ENGINE_EVENT_OBSERVABLE_H
#include <event/handler.hpp>
#include <stdexcept>
#include <vector>

namespace soil::event {
template <class T, std::enable_if_t<std::is_base_of_v<Event, T>>*>
class Observable {
 public:
  Observable() = default;

  virtual ~Observable() {
    for (auto* listener : listeners_) {
      listener->onDelete_ = nullptr;
    }
  }

  virtual void AddListener(Handler<T>* listener) {
    listeners_.push_back(listener);
    listener->onDelete_ = [this](Handler<T>* l) { RemoveListener(l); };
  }

  virtual void RemoveListener(Handler<T>* listener) {
    for (auto itr = listeners_.begin(); itr < listeners_.end(); ++itr) {
      if (listener == *itr) {
        listeners_.erase(itr);
        listener->onDelete_ = nullptr;
        return;
      }
    }
    throw std::runtime_error("RemoveListener failed");
  }

 protected:
  virtual void fire(const T& event) {
    std::vector<Handler<T>*> listeners(listeners_);
    for (Handler<T>* listener : listeners) {
      listener->Handle(event);
    }
  }

 private:
  std::vector<Handler<T>*> listeners_;
};
}  // namespace soil::event
#endif /*  ENGINE_EVENT_OBSERVABLE_H */
