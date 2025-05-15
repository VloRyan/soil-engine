#include "video/render/instance/buffer.h"


#include <gtest/gtest.h>

#include "test_buffer_object.h"
#include "test_instance.h"

namespace soil::video::render::instance {
    class BufferTest : public testing::Test {};

    TEST_F(BufferTest, Constructor) {
        constexpr auto instanceSize = 1;
        auto bo = TestBufferObject(12);
        const auto buffer = Buffer(&bo, instanceSize);

        EXPECT_EQ(buffer.GetInstanceSize(), instanceSize);
        EXPECT_EQ(buffer.GetPerInstanceBuffer(), &bo);
        EXPECT_EQ(buffer.GetInstancesCount(), 0);
        EXPECT_EQ(buffer.GetDirtyInstancesCount(), 0);
    }

    TEST_F(BufferTest, PrepareInstance_new_instance) {
        constexpr auto instanceSize = 1;
        auto bo = TestBufferObject(12);
        auto buffer = Buffer(&bo, instanceSize);
        auto instance = TestInstance();

        buffer.PrepareInstance(&instance);

        EXPECT_EQ(buffer.GetInstancesCount(), 0);
        EXPECT_EQ(buffer.GetDirtyInstancesCount(), 1);
    }

    TEST_F(BufferTest, PrepareInstance_already_dirty_instance) {
        constexpr auto instanceSize = 1;
        auto bo = TestBufferObject(12);
        auto buffer = Buffer(&bo, instanceSize);
        auto instance = TestInstance();

        buffer.PrepareInstance(&instance);
        buffer.PrepareInstance(&instance);

        EXPECT_EQ(buffer.GetInstancesCount(), 0);
        EXPECT_EQ(buffer.GetDirtyInstancesCount(), 1);
    }

    TEST_F(BufferTest, PrepareInstance_already_prepared_instance) {
        constexpr auto instanceSize = 1;
        auto bo = TestBufferObject(12);
        auto buffer = Buffer(&bo, instanceSize);
        auto instance = TestInstance();
        instance.SetIndex(4711); // prepared

        buffer.PrepareInstance(&instance);

        EXPECT_EQ(buffer.GetInstancesCount(), 0);
        EXPECT_EQ(buffer.GetDirtyInstancesCount(), 1);
    }

    TEST_F(BufferTest, Update_no_dirty) {
        constexpr auto instanceSize = 1;
        auto bo = TestBufferObject(12);
        auto buffer = Buffer(&bo, instanceSize);

        buffer.Update({0.F, 0.F, 0.F});

        EXPECT_EQ(buffer.GetInstancesCount(), 0);
        EXPECT_EQ(buffer.GetDirtyInstancesCount(), 0);
    }

    TEST_F(BufferTest, Update_with_dirty) {
        constexpr auto instanceSize = 1;
        auto bo = TestBufferObject(12);
        auto buffer = Buffer(&bo, instanceSize);
        auto instance = TestInstance();
        buffer.PrepareInstance(&instance);
        ASSERT_EQ(instance.GetIndex(), -1);

        buffer.Update({0.F, 0.F, 0.F});

        EXPECT_EQ(buffer.GetInstancesCount(), 1);
        EXPECT_EQ(buffer.GetDirtyInstancesCount(), 0);
        EXPECT_EQ(instance.GetIndex(), 0);
    }

} // namespace soil::video::render::instance
