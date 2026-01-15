#include "stage/scene/gui/layout/anchor.h"

#include <gmock/gmock-matchers.h>

#include "gtest/gtest.h"
#include "stage/scene/gui/rectangle.h"
#include "testing.h"
#include "util/strings.h"

namespace soil::stage::scene::gui::layout {
class AnchorTest : public testing::Test {};

TEST_F(AnchorTest, Contruct) {
  struct Anchor::Alignment alignment = {
      .Horizontal = Anchor::HorizontalAlignments::Center,
      .Vertical = Anchor::VerticalAlignments::Center};
  const auto anchor = Anchor(alignment);
  EXPECT_EQ(anchor.GetAlignment(), alignment);
}

TEST_F(AnchorTest, Apply) {
  auto anchor = Anchor({.Horizontal = Anchor::HorizontalAlignments::Center,
                        .Vertical = Anchor::VerticalAlignments::Center});
  auto actual = anchor.Align(glm::vec3(0.F), glm::ivec2(10), glm::ivec2(100),
                             glm::ivec4(0));
  EXPECT_VEC_EQ(actual, glm::vec3(0.F));

  anchor.SetAlignment({.Horizontal = Anchor::HorizontalAlignments::Left,
                       .Vertical = Anchor::VerticalAlignments::Top});
  actual = anchor.Align(glm::vec3(0.F), glm::ivec2(10), glm::ivec2(100),
                        glm::ivec4(0));
  EXPECT_VEC_EQ(actual, glm::vec3(-45.F, 45.F, 0.F));

  anchor.SetAlignment({.Horizontal = Anchor::HorizontalAlignments::Right,
                       .Vertical = Anchor::VerticalAlignments::Bottom});
  actual = anchor.Align(glm::vec3(0.F), glm::ivec2(10), glm::ivec2(100),
                        glm::ivec4(0));
  EXPECT_VEC_EQ(actual, glm::vec3(45.F, -45.F, 0.F));
}

TEST_F(AnchorTest, ApplyrWithPadding) {
  auto anchor = Anchor({.Horizontal = Anchor::HorizontalAlignments::Center,
                        .Vertical = Anchor::VerticalAlignments::Center});
  auto actual = anchor.Align(glm::vec3(0.F), glm::ivec2(10), glm::ivec2(100),
                             glm::ivec4(5, 10, 15, 20));
  EXPECT_VEC_EQ(actual, glm::vec3(0.F, 0.F, 0.F));

  anchor.SetAlignment({.Horizontal = Anchor::HorizontalAlignments::Left,
                       .Vertical = Anchor::VerticalAlignments::Top});
  actual = anchor.Align(glm::vec3(0.F), glm::ivec2(10), glm::ivec2(100),
                        glm::ivec4(5, 10, 15, 20));
  EXPECT_VEC_EQ(actual, glm::vec3(-40.F, 35.F, 0.F));

  anchor.SetAlignment({.Horizontal = Anchor::HorizontalAlignments::Right,
                       .Vertical = Anchor::VerticalAlignments::Bottom});
  actual = anchor.Align(glm::vec3(0.F), glm::ivec2(10), glm::ivec2(100),
                        glm::ivec4(5, 10, 15, 20));
  EXPECT_VEC_EQ(actual, glm::vec3(30.F, -25.F, 0.F));
}

}  // namespace soil::stage::scene::gui::layout
