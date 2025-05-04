#include "video/buffer/ebo.h"

namespace video::buffer {
Ebo::Ebo(const UsageType usage, const AccessType access)
    : Object(Types::ElementArray, usage, access) {}

Ebo::~Ebo() = default;
} // namespace video::buffer
