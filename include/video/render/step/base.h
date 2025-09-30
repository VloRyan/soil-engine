#ifndef SOIL_VIDEO_RENDER_STEP_BASE_H
#define SOIL_VIDEO_RENDER_STEP_BASE_H
#include <string>

#include "context.hpp"

namespace soil::video::render::step {
enum class Type : std::int8_t {
  Render = 0,
  Prepare,
};

class Base {
  friend class Pipeline;

 public:
  explicit Base(std::string id, Type type, Base* requiredStep = nullptr);

  virtual ~Base() = default;

  virtual void Process(Context& context) = 0;

  [[nodiscard]] Base* GetRequiredStep() const;

  std::string GetId();

  [[nodiscard]] virtual Type GetType() const;

 protected:
  Base* requiredStep_;

 private:
  std::string id_;
  Type type_;
};
}  // namespace soil::video::render::step
#endif
