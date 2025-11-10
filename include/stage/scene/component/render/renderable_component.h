#ifndef SOIL_STAGE_SCENE_COMPONENT_RENDER_RENDERABLE_COMPONENT_HPP
#define SOIL_STAGE_SCENE_COMPONENT_RENDER_RENDERABLE_COMPONENT_HPP
#include "stage/scene/component/component.h"
#include "video/render/renderable.h"

namespace soil::stage::scene::component {
class RenderableComponent : public Component {
 public:
  ~RenderableComponent() override = default;

  [[nodiscard]] virtual bool IsVisible() const;

  virtual void SetVisible(bool visible);

  [[nodiscard]] bool IsOpaque() const;

  virtual void SetOpaque(bool opaque);

  [[nodiscard]] virtual bool IsCulled() const;

  virtual void SetCulled(bool culled);

  [[nodiscard]] virtual video::render::Renderable* GetRenderable() = 0;

 protected:
  explicit RenderableComponent(bool opaque = true, bool visible = true);

 private:
  bool visible_;
  bool opaque_;
  bool culled_;
};
}  // namespace soil::stage::scene::component
#endif
