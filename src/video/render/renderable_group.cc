#include "video/render/renderable_group.h"

#include "video/shader/shader.h"
namespace soil::video::render {

/*
void RenderableGroup::Insert(Renderable *renderable) {
#ifdef DEBUG
  if (!renderables_.empty() && ID() != renderable->StateId()) {
    throw std::runtime_error("stateId does not match with group id");
  }
#endif
  renderables_.push_back(renderable);
}

bool RenderableGroup::Remove(Renderable *renderable) {
  for (auto itr = renderables_.begin(); itr != renderables_.end(); ++itr) {
    if (*itr == renderable) {
      renderables_.erase(itr);
      return true;
    }
  }
  return false;
}

const std::vector<Renderable *> &RenderableGroup::Content() const {
  return renderables_;
}

void RenderableGroup::Render(video::render::State &state) const {
#ifdef DEBUG
  if (Empty()) {
    throw std::runtime_error("empty group can not be rendered");
  }
  // state.Apply(ID());
#endif
  auto noopWriter = NoopWriter();
  switch (renderables_.front()->DrawCommand()) {
    case DrawCommand::Elements:
      for (auto *renderable : renderables_) {
        renderable->ApplyData(noopWriter);
        auto vao = renderable->Vao();
        const auto *ebo = vao->GetEbo();
        vao->Bind();
        shader::Shader::DrawElements(static_cast<uint>(renderable->DrawMode()),
                                     ebo->GetIndexCount(), ebo->GetIndexType());
      }
    case DrawCommand::ElementsInstanced:
      const auto amount = static_cast<int>(renderables_.size());
      if (amount == 0) {
        return;
      }
      auto *front = renderables_.front();
      auto vao = front->Vao();
      const auto *ebo = vao->GetEbo();
      vao->Bind();
      shader::Shader::DrawElementsInstanced(
          static_cast<uint>(front->DrawMode()), ebo->GetIndexCount(),
          ebo->GetIndexType(), amount, 0);
  }
}

bool RenderableGroup::Empty() const { return renderables_.empty(); }
*/
}  // namespace soil::video::render
