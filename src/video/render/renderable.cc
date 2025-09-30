#include "video/render/renderable.h"

namespace soil::video::render {
Renderable::Renderable() : containerStateIndex_(-1) {}

int Renderable::GetContainerStateIndex() const { return containerStateIndex_; }

void Renderable::SetContainerStateIndex(const int index) {
  containerStateIndex_ = index;
}

}  // namespace soil::video::render
