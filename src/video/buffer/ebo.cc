#include "video/buffer/ebo.h"

namespace soil::video::buffer {
    Ebo::Ebo(const UsageType usage, const AccessType access) : Object(Types::ElementArray, usage, access) {}

    Ebo::~Ebo() = default;
} // namespace soil::video::buffer
