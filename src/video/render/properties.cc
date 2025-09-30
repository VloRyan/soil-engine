#include "video/render/properties.h"

namespace soil::video::render {
Properties::Properties()
    : clippingPlanes_(), shadowMapSize_(0), wireFrame_(false) {}

bool Properties::isClippingPlaneEnabled(const int index) const {
  return clippingPlanes_.at(index) != glm::vec4(0);
}

bool Properties::isWireFrame() const { return wireFrame_; }

glm::vec4 Properties::GetClippingPlane(const int index) const {
  return clippingPlanes_.at(index);
}

void Properties::SetClippingPlane(const int index, const glm::vec4 plane) {
  if (index >= MAX_CLIPPING_PLANES || index < 0) {
    return;
  }
  clippingPlanes_[index] = plane;
}
}  // namespace soil::video::render
