#ifndef SOIL_STAGE_SCENE_COMPONENT_RENDER_MESH_INSTANCE_COMPONENT_H
#define SOIL_STAGE_SCENE_COMPONENT_RENDER_MESH_INSTANCE_COMPONENT_H
#include "drawable_component.h"
#include "video/render/draw/vao_elements_instanced.h"

namespace soil::stage::scene::component::render {
class MeshInstanceComponent
    : public DrawableComponent,
      public video::render::draw::VaoElementsInstanced::Data {
 public:
  explicit MeshInstanceComponent(
      const video::render::draw::VaoElementsInstanced::PileDescriptor&
          pileDescriptor);
  explicit MeshInstanceComponent(const std::string& name);
  ~MeshInstanceComponent() override;
  [[nodiscard]] video::render::draw::Drawable* Drawable() override;
  void SetOpaque(bool opaque) override;
  bool IsDrawablePile() override;
  void SetVisible(bool visible) override;
  void SetCulled(bool culled) override;

 protected:
  void SignalChanged() override;

 protected:
  video::render::draw::VaoElementsInstanced* pile_;
};
}  // namespace soil::stage::scene::component::render

#endif
