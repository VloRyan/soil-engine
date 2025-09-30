#include "gtest/gtest.h"
#include "world/volume/container.h"

namespace soil::world {
class IntersectionTest : public testing::Test {};

TEST_F(IntersectionTest, TestCircle) {
  constexpr auto box = Intersection::Box2d{.Min = glm::vec2(0.5F, 1.F),
                                           .Max = glm::vec2(1.5F, 2.F)};

  /* TODO: Returns false?!
   EXPECT_TRUE(Intersection::Test(box, Intersection::Circle2d{.Center =
   glm::vec2(2.F, 1.F), .Radius = 0.5F}));*/
  EXPECT_FALSE(Intersection::Test(
      box,
      Intersection::Circle2d{.Center = glm::vec2(2.1F, 1.F), .Radius = 0.5F}));
  EXPECT_FALSE(Intersection::Test(
      box,
      Intersection::Circle2d{.Center = glm::vec2(2.0F, 1.1F), .Radius = 0.5F}));
  EXPECT_FALSE(Intersection::Test(
      box,
      Intersection::Circle2d{.Center = glm::vec2(2.0F, 1.F), .Radius = 0.4F}));

  EXPECT_TRUE(Intersection::Test(
      box,
      Intersection::Circle2d{.Center = glm::vec2(2.F, 1.F), .Radius = 3.F}));
}
}  // namespace soil::world
