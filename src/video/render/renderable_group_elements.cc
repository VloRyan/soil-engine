#include "video/render/renderable_group_elements.h"

#include "video/shader/shader.h"
namespace soil::video::render {

void RenderableGroupElements::Insert(Renderable *renderable) {
#ifdef DEBUG
  if (!renderables_.empty() &&
      renderables_.front()->StateId() != renderable->StateId()) {
    throw std::runtime_error("stateId does not match with group id");
  }
#endif
  renderables_.push_back(renderable);
}

bool RenderableGroupElements::Remove(Renderable *renderable) {
  for (auto itr = renderables_.begin(); itr != renderables_.end(); ++itr) {
    if (*itr == renderable) {
      renderables_.erase(itr);
      return true;
    }
  }
  return false;
}

const std::vector<Renderable *> &RenderableGroupElements::Content() const {
  return renderables_;
}

void RenderableGroupElements::Render(video::render::State &state) {
#ifdef DEBUG
  if (Empty()) {
    throw std::runtime_error("empty group can not be rendered");
  }
#endif
  auto &stateId = renderables_.front()->StateId();
  state.Apply(stateId.State);
  stateId.Shader->Prepare(state);
  auto noopWriter = NoopWriter();
  for (auto *renderable : renderables_) {
    renderable->ApplyData(noopWriter, state);
    renderable->Render(state, 1);
  }
}

bool RenderableGroupElements::Empty() const { return renderables_.empty(); }

}  // namespace soil::video::render
