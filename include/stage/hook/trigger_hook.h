#ifndef SOIL_STAGE_HOOK_TRIGGER_HOOK_H
#define SOIL_STAGE_HOOK_TRIGGER_HOOK_H
#include <bitset>
#include <vector>
namespace soil::stage::hook {
class TriggerHook {
 public:
  enum class TriggerType {
    BeforeUpdateScene = 0,
    AfterUpdateScene,
    Render,
    COUNT,
  };

  explicit TriggerHook(const std::vector<TriggerType>& triggers);
  virtual ~TriggerHook() = default;
  [[nodiscard]] bool IsTrigger(TriggerType trigger);
  virtual void OnTrigger(TriggerType trigger) = 0;

 protected:
  void SetTriggers(const std::vector<TriggerType>& triggers);

 private:
  std::bitset<static_cast<size_t>(TriggerType::COUNT)> triggers_;
};
}  // namespace soil::stage::hook

#endif
