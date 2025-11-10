#ifndef SOIL_STAGE_HOOK_TRIGGER_HOOK_H
#define SOIL_STAGE_HOOK_TRIGGER_HOOK_H
#include <bitset>
#include <vector>

namespace soil::stage::scene {
class Node;
}
namespace soil::stage::hook {
class TriggerHook {
 public:
  enum class TriggerType {
    BeforeUpdateScene = 0,
    AfterUpdateScene,
    BeforeRender,
    COUNT,
  };
  struct TriggerPoint {
    scene::Node* Root{nullptr};
    soil::stage::hook::TriggerHook::TriggerType TriggerType{
        hook::TriggerHook::TriggerType::BeforeUpdateScene};

    bool operator==(const TriggerHook::TriggerPoint& rhs) const {
      return Root == rhs.Root && TriggerType == rhs.TriggerType;
    }
  };
  struct TriggerPointEquality {
    bool operator()(const TriggerHook::TriggerPoint& lhs,
                    const TriggerPoint& rhs) const noexcept {
      return lhs == rhs;
    }
    size_t operator()(TriggerHook::TriggerPoint const& key) const {
      std::size_t seed = 0;
      hash_combine(seed, key.Root);
      hash_combine(seed, key.TriggerType);
      return seed;
    }
    template <class T>
    static inline void hash_combine(std::size_t& seed, const T& v) {
      std::hash<T> hasher;
      seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }
  };

  explicit TriggerHook() = default;
  virtual ~TriggerHook() = default;
  virtual void OnTrigger(const TriggerPoint& point) = 0;
};
}  // namespace soil::stage::hook

#endif
