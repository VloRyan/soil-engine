#include "video/render/instance/instance.h"

namespace video::render::instance {
Instance::Instance() : index_(-1) {}

int Instance::GetIndex() const { return index_; }

void Instance::SetIndex(const int index) { index_ = index; }
} // namespace video::render::instance
