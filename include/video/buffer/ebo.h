#ifndef ENGINE_ENGINE_VIDEO_BUFFER_EBO_H_
#define ENGINE_ENGINE_VIDEO_BUFFER_EBO_H_
#include "video/buffer/object.h"

namespace video::buffer {
    /**Element Array Buffer */
    class Ebo : public Object {
    public:
        explicit Ebo(UsageType usage = UsageType::Static, AccessType access = AccessType::Draw);

        Ebo(const Ebo &orig) = delete;

        ~Ebo() override;
    };
}
#endif //ENGINE_ENGINE_VIDEO_BUFFER_EBO_H_
