#ifndef SOIL_STAGE_SCENE_HOOK_HOOK_H
#define SOIL_STAGE_SCENE_HOOK_HOOK_H
#include "bitset"
#include "stage/event/component.h"
namespace soil::stage::scene {
class Scene;
}
namespace soil::stage::scene::hook {

class Hook : public event::ComponentEventHandler {
 public:
  enum class Trigger_t {
    BeforeUpdateScene = 0,
    AfterUpdateScene,
    Render,
    COUNT,
  };

  enum class HandlerType : std::uint8_t {
    None = 0,
    Component,
  };

  ~Hook() override = default;

  [[nodiscard]] bool IsTrigger(Trigger_t trigger);
  [[nodiscard]] virtual HandlerType GetHandlerType() const;

  void Handle(const event::Component& event) override;

  virtual void Perform(Trigger_t trigger) = 0;

 protected:
  explicit Hook(const std::vector<Trigger_t>& triggers,
                HandlerType handlerType = HandlerType::None);

 private:
  std::bitset<static_cast<size_t>(Trigger_t::COUNT)> triggers_;
  HandlerType handlerType_;
};
}  // namespace soil::stage::scene::hook

#endif
