#include "video/buffer/uniform_buffer_object.h"

namespace video::buffer {
    UniformBufferObject::UniformBufferObject(const UsageType usage, const AccessType access)
        : Object(Object::Types::Uniform, usage, access),
          m_Layout(Layout::STD140) {
    }

    void UniformBufferObject::SetLayout(const Layout layout) {
        m_Layout = layout;
    }
}
