#ifndef SOIL_VIDEO_RENDER_DRAW_DRAWABLE_CONTAINER_H
#define SOIL_VIDEO_RENDER_DRAW_DRAWABLE_CONTAINER_H

#include <vector>

#include "drawable.hpp"
namespace soil::video::render::draw {
class DrawableContainer : event::DrawableEventHandler {
 public:
  enum class Kind : std::uint8_t {
    Opaque = 0,
    Transparent,
  };
  enum class SortDirection : std::uint8_t {
    FarToNear = 0,
    NearToFar,
  };
  DrawableContainer() = default;
  ~DrawableContainer() override = default;

  void Insert(draw::Drawable *drawable);
  bool Remove(draw::Drawable *drawable);
  const std::vector<draw::Drawable *> &GetDrawables(Kind kind);
  void Sort(Kind kind, SortDirection);
  void Handle(const event::DrawableEvent &event) override;

 private:
  struct SortableDrawable {
    float Distance;
    class draw::Drawable *Drawable;
  };
  static void Sort(std::vector<draw::Drawable *> &drawables,
                   SortDirection direction);
  static bool RemoveFrom(std::vector<draw::Drawable *> &drawables,
                         draw::Drawable *renderable);
  std::vector<draw::Drawable *> opaqueDrawables_;
  std::vector<draw::Drawable *> transparentDrawables_;
};
}  // namespace soil::video::render::draw
#endif
