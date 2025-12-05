#ifndef SOIL_VIDEO_RENDER_ALGORITHM_HPP
#define SOIL_VIDEO_RENDER_ALGORITHM_HPP

#include "video/render/draw/drawable.hpp"
#include "video/render/draw/drawable_container.h"
namespace soil::video::render {
class Algorythm {
 public:
  virtual ~Algorythm() = default;
  virtual void Render(State& state, draw::DrawableContainer& container) = 0;

 protected:
  Algorythm() = default;
};
}  // namespace soil::video::render
#endif
