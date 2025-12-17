#include "video/render/state_identifier.hpp"

#include <gtest/gtest.h>

#include "video/shader/program.h"
namespace soil::video::render {

class StateIdentifierTest : public testing::Test {};

TEST_F(StateIdentifierTest, StateIdentifierTest) {
  auto shader = shader::Program("test", "test");
  auto id1 = StateIdentifier{.Shader = &shader,
                             .VAOId = 2,
                             .DrawMode = DrawMode::Points,
                             .State =
                                 {
                                     .Blend = false,
                                     .DepthTest = false,
                                     .StencilTest = false,
                                     .ScissorTest = false,
                                     .DepthFunc = DepthFunc::Less,
                                 }

  };
  auto id2 = id1;

  EXPECT_EQ(id1.ChangeCount(id2), 0);
  EXPECT_EQ(id2.ChangeCount(id1), 0);

  id1.Shader = nullptr;
  EXPECT_EQ(id1.ChangeCount(id2), 1);
  EXPECT_EQ(id2.ChangeCount(id1), 1);

  id2.VAOId = 815;
  EXPECT_EQ(id1.ChangeCount(id2), 2);
  EXPECT_EQ(id2.ChangeCount(id1), 2);

  id1.DrawMode = DrawMode::Triangles;
  EXPECT_EQ(id1.ChangeCount(id2), 3);
  EXPECT_EQ(id2.ChangeCount(id1), 3);

  id2.State.Blend = true;
  EXPECT_EQ(id1.ChangeCount(id2), 4);
  EXPECT_EQ(id2.ChangeCount(id1), 4);

  id1.State.ScissorTest = true;
  EXPECT_EQ(id1.ChangeCount(id2), 5);
  EXPECT_EQ(id2.ChangeCount(id1), 5);

  id2.State.StencilTest = true;
  EXPECT_EQ(id1.ChangeCount(id2), 6);
  EXPECT_EQ(id2.ChangeCount(id1), 6);

  id1.State.DepthTest = true;
  EXPECT_EQ(id1.ChangeCount(id2), 7);
  EXPECT_EQ(id2.ChangeCount(id1), 7);

  id1.State.DepthFunc = DepthFunc::Equal;
  EXPECT_EQ(id1.ChangeCount(id2), 8);
  EXPECT_EQ(id2.ChangeCount(id1), 8);

  id2.State.DepthFunc = DepthFunc::Equal;
  EXPECT_EQ(id1.ChangeCount(id2), 7);
  EXPECT_EQ(id2.ChangeCount(id1), 7);

  id2.State.DepthTest = true;
  EXPECT_EQ(id1.ChangeCount(id2), 6);
  EXPECT_EQ(id2.ChangeCount(id1), 6);
}
}  // namespace soil::video::render
