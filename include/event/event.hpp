#ifndef SOIL_EVENT_EVENT_H
#define SOIL_EVENT_EVENT_H
#include <cstdint>

namespace soil::event {
struct Event {
  enum class Type_t : std::uint8_t {
    System = 0,
    Video,
    Input,
    Sound,
    Stage,
    Window,
    Game,
  };
  explicit Event(Type_t type) : Type(type) {}
  virtual ~Event() = default;
  
  Type_t Type;

  friend bool operator==(const Event& lhs, const Event& rhs) {
    return lhs.Type == rhs.Type;
  }

  friend bool operator!=(const Event& lhs, const Event& rhs) {
    return !(lhs == rhs);
  }
};
}  // namespace soil::event
#endif
