#include "video/buffer/ubo.h"

namespace soil::video::buffer {
    UniformBufferObject::UniformBufferObject(const UsageType usage, const AccessType access) :
        Object(Types::Uniform, usage, access), m_Layout(Layout::STD140) {}

    void UniformBufferObject::SetLayout(const Layout layout) { m_Layout = layout; }
} // namespace soil::video::buffer
