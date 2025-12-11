#include "video/render/forward/forward_rendering.h"
namespace soil::video::render {

using dc = draw::DrawableContainer;
void ForwardRendering::Render(State &state,
                              draw::DrawableContainer &container) const {
  RenderOpaquePass(state, container);
  RenderTransparentPass(state, container);
}

void ForwardRendering::RenderOpaquePass(State &state,
                                        draw::DrawableContainer &container) {
  container.Sort(dc::Kind::Opaque, dc::SortDirection::NearToFar);
  DrawEach(state, container.GetDrawables(dc::Kind::Opaque));
}

void ForwardRendering::RenderTransparentPass(
    State &state, draw::DrawableContainer &container) {
  container.Sort(dc::Kind::Transparent, dc::SortDirection::FarToNear);
  DrawEach(state, container.GetDrawables(dc::Kind::Transparent));
}

void ForwardRendering::DrawEach(
    State &state, const std::vector<draw::Drawable *> &drawables) {
  for (auto *drawable : drawables) {
    drawable->Bind(state);
    drawable->Draw();
  }
}

}  // namespace soil::video::render
