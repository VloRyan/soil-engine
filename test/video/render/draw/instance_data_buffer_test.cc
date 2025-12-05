#include "video/render/draw/instance_data_buffer.h"

#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>

#include "mocks.hpp"
#include "video/buffer/mocks.hpp"

namespace soil::video::render::draw {
class InstanceDataBufferTest : public testing::Test {};

TEST_F(InstanceDataBufferTest, Constructor) {
  constexpr auto instanceSize = 1;
  auto bo = video::buffer::BufferObjectMock();
  const auto buffer = InstanceDataBuffer(&bo, instanceSize);

  EXPECT_EQ(buffer.InstanceSize(), instanceSize);
  EXPECT_EQ(buffer.GetPerInstanceBuffer(), &bo);
  EXPECT_EQ(buffer.InstancesCount(), 0);
  EXPECT_EQ(buffer.DirtyInstancesCount(), 0);
}

TEST_F(InstanceDataBufferTest, Insert) {
  auto bo = video::buffer::BufferObjectMock();
  auto buffer = InstanceDataBuffer(&bo, 0);
  auto instance = InstanceDataMock();

  buffer.Insert(&instance);
  EXPECT_EQ(buffer.InstancesCount(), 1);
  EXPECT_EQ(buffer.DirtyInstancesCount(), 1);
  EXPECT_THAT(buffer.Data(), testing::ElementsAre(&instance));
}

TEST_F(InstanceDataBufferTest, UpdateSetDirty) {
  auto bo = video::buffer::BufferObjectMock();
  auto buffer = InstanceDataBuffer(&bo, 0);
  auto instance = InstanceDataMock();

  buffer.Insert(&instance);
  EXPECT_EQ(buffer.InstancesCount(), 1);
  EXPECT_EQ(buffer.DirtyInstancesCount(), 1);
  EXPECT_EQ(instance.Calls.Write, 0);

  buffer.Update();
  EXPECT_EQ(buffer.InstancesCount(), 1);
  EXPECT_EQ(buffer.DirtyInstancesCount(), 0);
  EXPECT_EQ(instance.Calls.Write, 1);

  buffer.Update();
  EXPECT_EQ(buffer.InstancesCount(), 1);
  EXPECT_EQ(buffer.DirtyInstancesCount(), 0);
  EXPECT_EQ(instance.Calls.Write, 1);

  buffer.SetDirty(&instance);
  EXPECT_EQ(buffer.InstancesCount(), 1);
  EXPECT_EQ(buffer.DirtyInstancesCount(), 1);
  EXPECT_EQ(instance.Calls.Write, 1);

  buffer.Update();
  EXPECT_EQ(buffer.InstancesCount(), 1);
  EXPECT_EQ(buffer.DirtyInstancesCount(), 0);
  EXPECT_EQ(instance.Calls.Write, 2);
}

TEST_F(InstanceDataBufferTest, Remove) {
  auto bo = video::buffer::BufferObjectMock();
  auto buffer = InstanceDataBuffer(&bo, 0);
  auto instance = InstanceDataMock();

  auto result = buffer.Remove(&instance);
  EXPECT_EQ(result, false);
  EXPECT_EQ(buffer.InstancesCount(), 0);
  EXPECT_EQ(buffer.DirtyInstancesCount(), 0);

  buffer.Insert(&instance);
  EXPECT_EQ(buffer.InstancesCount(), 1);
  EXPECT_EQ(buffer.DirtyInstancesCount(), 1);
  EXPECT_THAT(buffer.Data(), testing::ElementsAre(&instance));

  result = buffer.Remove(&instance);
  EXPECT_EQ(result, true);
  EXPECT_EQ(buffer.InstancesCount(), 0);
  EXPECT_EQ(buffer.DirtyInstancesCount(), 0);
  EXPECT_TRUE(buffer.Data().empty());
}

}  // namespace soil::video::render::draw
