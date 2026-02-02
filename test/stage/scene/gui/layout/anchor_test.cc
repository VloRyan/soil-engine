#include "stage/scene/gui/layout/anchor.h"

#include <gmock/gmock-matchers.h>

#include "gtest/gtest.h"
#include "stage/scene/gui/rectangle.h"
#include "testing.h"
#include "util/strings.h"

namespace soil::stage::scene::gui::layout {
class AnchorTest : public testing::Test {};

TEST_F(AnchorTest, Contruct) {
  struct Alignment alignment = {Alignment::Horizontal::Center, Alignment::Vertical::Center};
  const auto anchor = Anchor(alignment);
  EXPECT_EQ(anchor.GetAlignment(), alignment);
}

TEST_F(AnchorTest, Align) {
  EXPECT_VEC_EQ(Anchor::Align({Alignment::Horizontal::Center,
                               Alignment::Vertical::Center}, glm::vec3(0.F), glm::ivec2(10), glm::ivec2(100),
                              glm::ivec4(0)),
                glm::vec3(0.F));

  EXPECT_VEC_EQ(Anchor::Align({Alignment::Horizontal::Left, Alignment::Vertical::Top},
                              glm::vec3(0.F),
                              glm::ivec2(10),
                              glm::ivec2(100),
                              glm::ivec4(0)),
                glm::vec3(-45.F, 45.F, 0.F));

  EXPECT_VEC_EQ(Anchor::Align({Alignment::Horizontal::Right, Alignment::Vertical::Bottom},
                              glm::vec3(0.F),
                              glm::ivec2(10),
                              glm::ivec2(100),
                              glm::ivec4(0)),
                glm::vec3(45.F, -45.F, 0.F));
}

TEST_F(AnchorTest, AlignWithPadding) {

  EXPECT_VEC_EQ(Anchor::Align({Alignment::Horizontal::Center, Alignment::Vertical::Center},
                              glm::vec3(0.F),
                              glm::ivec2(10),
                              glm::ivec2(100),
                              glm::ivec4(5, 10, 15, 20)),
                glm::vec3(0.F, 0.F, 0.F));

  EXPECT_VEC_EQ(Anchor::Align({Alignment::Horizontal::Left, Alignment::Vertical::Top},
                              glm::vec3(0.F),
                              glm::ivec2(10),
                              glm::ivec2(100),
                              glm::ivec4(5, 10, 15, 20)),
                glm::vec3(-40.F, 35.F, 0.F));

  EXPECT_VEC_EQ(Anchor::Align({Alignment::Horizontal::Right, Alignment::Vertical::Bottom},
                              glm::vec3(0.F),
                              glm::ivec2(10),
                              glm::ivec2(100),
                              glm::ivec4(5, 10, 15, 20)),
                glm::vec3(30.F, -25.F, 0.F));
}

}  // namespace soil::stage::scene::gui::layout
