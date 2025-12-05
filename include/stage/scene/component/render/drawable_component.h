#ifndef SOIL_STAGE_SCENE_COMPONENT_RENDER_DRAWABLE_COMPONENT_HPP
#define SOIL_STAGE_SCENE_COMPONENT_RENDER_DRAWABLE_COMPONENT_HPP
#include "stage/scene/component/component.h"
#include "video/render/draw/drawable.hpp"

namespace soil::stage::scene::component {
class DrawableComponent : public Component {
 public:
  ~DrawableComponent() override = default;
  [[nodiscard]] virtual bool IsVisible() const;
  virtual void SetVisible(bool visible);
  [[nodiscard]] bool IsOpaque() const;
  virtual void SetOpaque(bool opaque);
  [[nodiscard]] virtual bool IsCulled() const;
  virtual void SetCulled(bool culled);
  [[nodiscard]] virtual video::render::draw::Drawable* Drawable() = 0;
  [[nodiscard]] virtual bool IsDrawablePile();

 protected:
  explicit DrawableComponent(bool opaque = true, bool visible = true);
  bool visible_;
  bool opaque_;
  bool culled_;
};
}  // namespace soil::stage::scene::component
#endif
