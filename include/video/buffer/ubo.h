#ifndef SOIL_VIDEO_BUFFER_UNIFORM_BUFFER_OBJECT_H
#define SOIL_VIDEO_BUFFER_UNIFORM_BUFFER_OBJECT_H
#include "object.h"

namespace soil::video::buffer {
    class UniformBufferObject final : public Object {
    public:
        enum class Layout : std::uint8_t { STD140 };

        explicit UniformBufferObject(UsageType usage = UsageType::Dynamic, AccessType access = AccessType::Draw);

        UniformBufferObject(const UniformBufferObject &orig) = delete;

        void SetLayout(Layout layout);

        ~UniformBufferObject() override = default;

    private:
        Layout m_Layout;
    };
} // namespace soil::video::buffer
#endif
