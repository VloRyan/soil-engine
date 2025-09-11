#ifndef TEST_INSTANCE_H
#define TEST_INSTANCE_H
#include "video/render/instance/instance.h"

namespace soil::video::render::instance {
    class TestInstance final : public Instance {
    public:
        TestInstance() = default;
        void WriteData(buffer::Cursor* cursor) const override;
    };
} // namespace soil::video::render::instance


#endif
