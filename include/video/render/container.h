#ifndef SOIL_VIDEO_RENDER_FORWARD_CONTAINER_H
#define SOIL_VIDEO_RENDER_FORWARD_CONTAINER_H
#include <vector>

#include "renderable.h"
#include "renderable_object.h"

namespace soil::video::render {
struct RenderDef {
  bool Blending{false};

  friend bool operator==(const RenderDef& lhs, const RenderDef& rhs) {
    return lhs.Blending == rhs.Blending;
  }

  friend bool operator!=(const RenderDef& lhs, const RenderDef& rhs) {
    return !(lhs == rhs);
  }
};

class Container {
 public:
  Container() = default;

  ~Container() = default;

  void Add(RenderableObject* renderable, const RenderDef& state);

  bool Remove(RenderableObject* renderable, const RenderDef& state);

  void GetPerDef(std::vector<RenderableObject*>& v, const RenderDef& state);

  bool Empty() const;

  inline static auto OPAQUE = RenderDef{.Blending = false};
  inline static auto NON_OPAQUE = RenderDef{.Blending = true};

 private:
  struct RenderableWithState {
    RenderableObject* Ptr;
    RenderDef State;
  };

  std::vector<RenderableWithState> renderables_;
};
}  // namespace soil::video::render

#endif
