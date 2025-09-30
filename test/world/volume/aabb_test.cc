#include "world/volume/aabb.h"

#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>

#include "testing.h"

namespace soil::world::volume {
class AABBTest : public testing::Test {};

TEST_F(AABBTest, IsInside) {
  const auto aabb = AABB({5.F, 5.F, 5.F});  // -2.5, -2.5, -2.5 -> 2.5, 2.5, 2.5

  ASSERT_TRUE(
      aabb.IsInside({-5.F, -5.F, -5.F}, {5.F, 5.F, 5.F}));  // completely inside
  ASSERT_FALSE(aabb.IsInside({-10.F, -10.F, -10.F},
                             {-5.F, -5.F, -5.F}));  // completely outside
  ASSERT_TRUE(
      aabb.IsInside({0.F, 0.F, 0.F}, {2.4F, 2.4F, 2.4F}));  // partly inside
}

TEST_F(AABBTest, IsInsideXZ) {
  const auto aabb = AABB({1.F, 1.F, 1.F});  // -2.5, -2.5, -2.5 -> 2.5, 2.5, 2.5

  ASSERT_TRUE(aabb.IsInsideXZ({-5.F, 0.F, -5.F},
                              {5.F, 0.F, 5.F}));  // completely inside
  ASSERT_FALSE(aabb.IsInsideXZ({-10.F, 0.F, -10.F},
                               {-5.F, 0.F, -5.F}));  // completely outside
  ASSERT_TRUE(
      aabb.IsInsideXZ({0.F, 0.F, 0.F}, {2.4F, 0.F, 2.4F}));  // partly inside
}

TEST_F(AABBTest, GenerateMesh) {
  const Line expectedData[] = {
      {
          .Start = glm::vec3(-0.5F, 0.5F, -0.5F),
          .End = glm::vec3(0.5F, 0.5F, -0.5F),
      },
      {
          .Start = glm::vec3(0.5F, 0.5F, -0.5F),
          .End = glm::vec3(0.5F, 0.5F, 0.5F),
      },
      {
          .Start = glm::vec3(0.5F, 0.5F, 0.5F),
          .End = glm::vec3(-0.5F, 0.5F, 0.5F),
      },
      {
          .Start = glm::vec3(-0.5F, 0.5F, 0.5F),
          .End = glm::vec3(-0.5F, 0.5F, -0.5F),
      },

      {
          .Start = glm::vec3(-0.5F, -0.5F, -0.5F),
          .End = glm::vec3(0.5F, -0.5F, -0.5F),
      },
      {
          .Start = glm::vec3(0.5F, -0.5F, -0.5F),
          .End = glm::vec3(0.5F, -0.5F, 0.5F),
      },
      {
          .Start = glm::vec3(0.5F, -0.5F, 0.5F),
          .End = glm::vec3(-0.5F, -0.5F, 0.5F),
      },
      {
          .Start = glm::vec3(-0.5F, -0.5F, 0.5F),
          .End = glm::vec3(-0.5F, -0.5F, -0.5F),
      },

      {
          .Start = glm::vec3(-0.5F, -0.5F, -0.5F),
          .End = glm::vec3(-0.5F, 0.5F, -0.5F),
      },
      {
          .Start = glm::vec3(0.5F, -0.5F, -0.5F),
          .End = glm::vec3(0.5F, 0.5F, -0.5F),
      },
      {
          .Start = glm::vec3(0.5F, -0.5F, 0.5F),
          .End = glm::vec3(0.5F, 0.5F, 0.5F),
      },
      {
          .Start = glm::vec3(-0.5F, -0.5F, 0.5F),
          .End = glm::vec3(-0.5F, 0.5F, 0.5F),
      },
  };
  auto aabb = AABB({1.F, 1.F, 1.F});
  aabb.SetPosition(glm::vec3(0.F));

  const auto got = aabb.GenerateLines();
  ASSERT_EQ(got.size(), 12);

  for (auto i = 0; i < 12; i++) {
    EXPECT_EQ(got[i].Start, expectedData[i].Start) << "i: " + std::to_string(i);
    EXPECT_EQ(got[i].End, expectedData[i].End) << "i: " + std::to_string(i);
  }
}
}  // namespace soil::world::volume
