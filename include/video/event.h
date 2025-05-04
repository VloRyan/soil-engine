#ifndef VIDEO_EVENT_H_
#define VIDEO_EVENT_H_


#include <cstdint>

#include "event/event.h"
#include "event/handler.hpp"


namespace video {
 class Event : public event::Event {
 public:
  enum class Cause : std::uint8_t {
   WindowResized
  };

  ~Event() override;

  [[nodiscard]] Cause GetCause() const;

 protected:
  explicit Event(Cause cause);

 private:
  Cause cause_;
 };

 using EventHandler = event::Handler<Event>;
} // input

#endif //VIDEO_EVENT_H_
