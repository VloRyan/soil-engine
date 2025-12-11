#ifndef SOIL_VIDEO_RENDER_FORWARD_FORWARD_RENDERING_H
#define SOIL_VIDEO_RENDER_FORWARD_FORWARD_RENDERING_H
#include "video/render/algorithm.hpp"
#include "video/render/draw/drawable.hpp"
namespace soil::video::render {
class ForwardRendering : public Algorythm {
 public:
  ForwardRendering() = default;
  ~ForwardRendering() override = default;
  void Render(State &state, draw::DrawableContainer &container) const override;

 private:
  static void RenderOpaquePass(State &state,
                               draw::DrawableContainer &container);
  static void RenderTransparentPass(State &state,
                                    draw::DrawableContainer &container);
  static void DrawEach(State &state,
                       const std::vector<draw::Drawable *> &drawables);
};
}  // namespace soil::video::render
#endif
