#include "video/render/renderable.h"
#include "video/render/renderable_group_elements.h"
namespace soil::video::render {
class RenderableMock : public Renderable {
 public:
  explicit RenderableMock(const StateIdentifier& stateId)
      : Renderable(stateId) {}
  ~RenderableMock() override = default;
  void Render(State& state, int count) override {}
  [[nodiscard]] const vertex::Vao* Vao() const override { return nullptr; }
  [[nodiscard]] enum DrawMode DrawMode() const override {
    return DrawMode::Quads;
  }

  [[nodiscard]] RenderableGroup* NewGroup() const override {
    return new RenderableGroupElements();
  }
  void ApplyData(const data::IWriter& writer,
                 soil::video::render::State& state) const override {}
  struct {
    int Render{0};
  } Calls;
};
}  // namespace soil::video::render
