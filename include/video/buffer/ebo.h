#ifndef SOIL_VIDEO_BUFFER_EBO_H_
#define SOIL_VIDEO_BUFFER_EBO_H_
#include "video/buffer/object.h"

namespace soil::video::buffer {
    /**Element Array Buffer */
    class Ebo final : public Object {
    public:
        explicit Ebo(UsageType usage = UsageType::Static, AccessType access = AccessType::Draw);

        Ebo(const Ebo &orig) = delete;

        ~Ebo() override;
    };
} // namespace soil::video::buffer
#endif // SOIL_VIDEO_BUFFER_EBO_H_
