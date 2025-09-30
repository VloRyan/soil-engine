#include "world/volume/quad_tree.h"

#include <cstdarg>

#include "glm/glm.hpp"
#include "gmock/gmock-matchers.h"
#include "gtest/gtest.h"
#include "stage/scene/node.h"
#include "stage/scene/scene.h"
#include "world/volume/aabb.h"

namespace soil::world::volume {
class QuadTreeTest : public testing::Test {
 protected:
  [[nodiscard]] static Volume* NewBoundingVolumeAt(const glm::vec2 pos,
                                                   const glm::vec2 dim) {
    auto* volume = new AABB({dim.x, 0.F, dim.y});
    volume->SetPosition({pos.x, 0.F, pos.y});
    return volume;
  }

  static std::vector<const Volume*> VolumesAt(const QuadTree& quadTree,
                                              const std::int16_t index) {
    std::vector<const Volume*> vol;
    quadTree.GetVolumes(index, vol);
    return vol;
  }

  static std::vector<const Volume*> VolumesAt(const QuadTree& quadTree,
                                              const glm::vec3 point) {
    std::vector<const Volume*> vol;
    quadTree.QueryVolumesAt(point, vol);
    return vol;
  }

  static std::vector<const Volume*> VolumesInRangeAt(const QuadTree& quadTree,
                                                     const glm::vec3 point,
                                                     const float radius) {
    std::vector<const Volume*> vol;
    quadTree.QueryVolumesInRange(point, radius, vol);
    return vol;
  }
};

TEST_F(QuadTreeTest, InsertOneItem) {
  auto quadTree = QuadTree(20, 10, 5.F);
  auto* volume = NewBoundingVolumeAt({-5.F, -5.F}, glm::vec3(5.F));

  quadTree.Insert(volume);

  EXPECT_EQ(quadTree.GetNodeCount(), 1);
  std::vector<const Volume*> vol;
  quadTree.GetVolumes(0, vol);
  EXPECT_THAT(vol, testing::ElementsAre(volume));
}

TEST_F(QuadTreeTest, InsertFourItems) {
  auto quadTree = QuadTree(20, 10, 5.F);
  const auto* topLeft = NewBoundingVolumeAt({-5.F, -5.F}, glm::vec3(5.F));
  const auto* topRight = NewBoundingVolumeAt({5.F, -5.F}, glm::vec3(5.F));
  const auto* bottomRight = NewBoundingVolumeAt({5.F, 5.F}, glm::vec3(5.F));
  const auto* bottomLeft = NewBoundingVolumeAt({-5.F, 5.F}, glm::vec3(5.F));

  quadTree.Insert(topLeft);
  quadTree.Insert(topRight);
  quadTree.Insert(bottomRight);
  quadTree.Insert(bottomLeft);

  EXPECT_EQ(quadTree.GetNodeCount(), 1);
  EXPECT_THAT(VolumesAt(quadTree, 0),
              testing::ElementsAre(topLeft, topRight, bottomRight, bottomLeft));
}

TEST_F(QuadTreeTest, InsertFiveItems) {
  auto quadTree = QuadTree(20, 10, 5.F);
  const auto* topLeft = NewBoundingVolumeAt({-5.F, -5.F}, glm::vec3(5.F));
  const auto* topLeft2 = NewBoundingVolumeAt({-5.F, -5.F}, glm::vec3(5.F));
  const auto* topRight = NewBoundingVolumeAt({5.F, -5.F}, glm::vec3(5.F));
  const auto* bottomRight = NewBoundingVolumeAt({5.F, 5.F}, glm::vec3(5.F));
  const auto* bottomLeft = NewBoundingVolumeAt({-5.F, 5.F}, glm::vec3(5.F));

  quadTree.Insert(topLeft);
  quadTree.Insert(topLeft2);
  quadTree.Insert(topRight);
  quadTree.Insert(bottomRight);
  quadTree.Insert(bottomLeft);

  EXPECT_EQ(quadTree.GetNodeCount(), 5);
  EXPECT_TRUE(VolumesAt(quadTree, 0).empty());
  EXPECT_THAT(VolumesAt(quadTree, 1), testing::ElementsAre(topLeft, topLeft2));
  EXPECT_THAT(VolumesAt(quadTree, 2), testing::ElementsAre(topRight));
  EXPECT_THAT(VolumesAt(quadTree, 3), testing::ElementsAre(bottomLeft));
  EXPECT_THAT(VolumesAt(quadTree, 4), testing::ElementsAre(bottomRight));
}

TEST_F(QuadTreeTest, InsertOnEdges) {
  auto quadTree = QuadTree(64, 2, 4.F);
  const auto* a = NewBoundingVolumeAt({-8.F, -8.F}, glm::vec3(1.F));
  const auto* b = NewBoundingVolumeAt({0.F, 8.F}, glm::vec3(1.F));
  const auto* c = NewBoundingVolumeAt({-8.F, 0.F}, glm::vec3(1.F));
  const auto* d = NewBoundingVolumeAt({8.F, 0.F}, glm::vec3(1.F));
  const auto* e = NewBoundingVolumeAt({0, -8.F}, glm::vec3(1.F));

  quadTree.Insert(a);
  quadTree.Insert(b);
  quadTree.Insert(c);
  quadTree.Insert(d);
  quadTree.Insert(e);

  EXPECT_EQ(quadTree.GetNodeCount(), 5);
  EXPECT_TRUE(VolumesAt(quadTree, 0).empty());
  EXPECT_THAT(VolumesAt(quadTree, 1), testing::ElementsAre(a, c, e));
  EXPECT_THAT(VolumesAt(quadTree, 2), testing::ElementsAre(d, e));
  EXPECT_THAT(VolumesAt(quadTree, 3), testing::ElementsAre(b, c));
  EXPECT_THAT(VolumesAt(quadTree, 4), testing::ElementsAre(b, d));
}

TEST_F(QuadTreeTest, QueryVolumesAt) {
  auto quadTree = QuadTree(64, 2, 4.F);
  const auto* a = NewBoundingVolumeAt({-8.F, -8.F}, glm::vec3(1.F));
  const auto* b = NewBoundingVolumeAt({0.F, 8.F}, glm::vec3(1.F));
  const auto* c = NewBoundingVolumeAt({-8.F, 0.F}, glm::vec3(1.F));
  const auto* d = NewBoundingVolumeAt({8.F, 0.F}, glm::vec3(1.F));
  const auto* e = NewBoundingVolumeAt({8.4F, 0.4F}, glm::vec3(1.F));

  quadTree.Insert(a);
  quadTree.Insert(b);
  quadTree.Insert(c);
  quadTree.Insert(d);
  quadTree.Insert(e);

  ASSERT_THAT(quadTree.GetNodeCount(), 5);
  EXPECT_THAT(VolumesAt(quadTree, glm::vec3(0.F, 0.F, 8.F)),
              testing::ElementsAre(b));
  EXPECT_THAT(VolumesAt(quadTree, glm::vec3(8.2F, 0.F, 0.2F)),
              testing::ElementsAre(d, e));
}

TEST_F(QuadTreeTest, DetermineLevel) {
  const auto quadTree = QuadTree(1);

  EXPECT_THAT(quadTree.DetermineLevel(0), 0);
  EXPECT_THAT(quadTree.DetermineLevel(2), 1);
  EXPECT_THAT(quadTree.DetermineLevel(5), 2);
  EXPECT_THAT(quadTree.DetermineLevel(69), 3);
  EXPECT_THAT(quadTree.DetermineLevel(330), 4);
}

TEST_F(QuadTreeTest, Remove) {
  auto quadTree = QuadTree(512, 4, 1.F);
  const auto* topLeft = NewBoundingVolumeAt({-5.F, -5.F}, glm::vec3(5.F));
  const auto* topLeft2 = NewBoundingVolumeAt({-5.F, -5.F}, glm::vec3(5.F));
  const auto* topRight = NewBoundingVolumeAt({5.F, -5.F}, glm::vec3(5.F));
  const auto* bottomRight = NewBoundingVolumeAt({5.F, 5.F}, glm::vec3(5.F));
  const auto* bottomLeft = NewBoundingVolumeAt({-5.F, 5.F}, glm::vec3(5.F));

  quadTree.Insert(topLeft);
  quadTree.Insert(topLeft2);
  quadTree.Insert(topRight);
  quadTree.Insert(bottomRight);
  quadTree.Insert(bottomLeft);

  ASSERT_THAT(VolumesAt(quadTree, 3), testing::ElementsAre(bottomLeft));

  auto ret = quadTree.Remove(bottomLeft);
  EXPECT_TRUE(ret);
  EXPECT_TRUE(VolumesAt(quadTree, 3).empty());

  ASSERT_THAT(VolumesAt(quadTree, 1), testing::ElementsAre(topLeft, topLeft2));

  ret = quadTree.Remove(topLeft);
  EXPECT_TRUE(ret);
  EXPECT_THAT(VolumesAt(quadTree, 1), testing::ElementsAre(topLeft2));
}

// TODO testing::UnorderedElementsAre is producing an undefined reference.
static bool UnorderedElementsAre(const std::vector<const Volume*>& actual,
                                 const std::vector<const Volume*>& expected) {
  if (expected.size() != actual.size()) {
    return false;
  }
  auto exCopy = std::vector(actual);
  std::vector<const Volume*> missingElements;
  for (auto vol : expected) {
    bool found = false;
    for (auto itr = exCopy.begin(); itr != exCopy.end(); ++itr) {
      if (*itr == vol) {
        exCopy.erase(itr);
        found = true;
        break;
      }
    }
    if (!found) {
      missingElements.push_back(vol);
    }
  }
  return exCopy.empty() && missingElements.empty();
}

TEST_F(QuadTreeTest, QueryVolumesInRange) {
  auto quadTree = QuadTree(64, 2, 4.F);
  const auto* a = NewBoundingVolumeAt({-8.F, -8.F}, glm::vec3(1.F));
  const auto* b = NewBoundingVolumeAt({0.F, 8.F}, glm::vec3(1.F));
  const auto* c = NewBoundingVolumeAt({-8.F, 0.F}, glm::vec3(1.F));
  const auto* d = NewBoundingVolumeAt({8.F, 0.F}, glm::vec3(1.F));
  const auto* e = NewBoundingVolumeAt({0, -8.F}, glm::vec3(1.F));

  quadTree.Insert(a);
  quadTree.Insert(b);
  quadTree.Insert(c);
  quadTree.Insert(d);
  quadTree.Insert(e);

  ASSERT_THAT(quadTree.GetNodeCount(), 5);

  /*EXPECT_THAT(VolumesInRangeAt(quadTree, glm::vec3(-7.F, 0.F, -7.F), 1.F),
              testing::UnorderedElementsAre(a));
  EXPECT_THAT(VolumesInRangeAt(quadTree, glm::vec3(-7.F, 0.F, -7.F), 7.F),
              testing::UnorderedElementsAre(a, c, e));
  EXPECT_THAT(VolumesInRangeAt(quadTree, glm::vec3(0.F, 0.F, 0.F), 10.F),
              testing:: UnorderedElementsAre(a, b, c, d, e));*/
  EXPECT_TRUE(UnorderedElementsAre(
      VolumesInRangeAt(quadTree, glm::vec3(-7.F, 0.F, -7.F), 1.F), {a}));
  EXPECT_TRUE(UnorderedElementsAre(
      VolumesInRangeAt(quadTree, glm::vec3(-7.F, 0.F, -7.F), 7.F), {a, c, e}));
  EXPECT_TRUE(UnorderedElementsAre(
      VolumesInRangeAt(quadTree, glm::vec3(0.F, 0.F, 0.F), 10.F),
      {a, b, c, d, e}));

  EXPECT_THAT(VolumesInRangeAt(quadTree, glm::vec3(-33.F, 0.F, -33.F), 0.9F),
              testing::ElementsAre());
}
}  // namespace soil::world::volume
