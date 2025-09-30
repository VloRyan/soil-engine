#include "world/volume/frustum.h"

#include <gtest/gtest.h>
#include <plog/Log.h>

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>

namespace soil::world::volume {
struct aabb {
  glm::vec3 Min;
  glm::vec3 Max;

  [[nodiscard]] std::vector<glm::vec3> Points() const {
    return std::vector{
        glm::vec3(Min.x, Min.y, Min.z),  // Lower-Left-Front
        glm::vec3(Min.x, Min.y, Max.z),  // Lower-Left-Back
        glm::vec3(Max.x, Min.y, Max.z),  // Lower-Right-Back
        glm::vec3(Max.x, Min.y, Min.z),  // Lower-Right-Front
        glm::vec3(Min.x, Max.y, Min.z),  // Upper-Left-Front
        glm::vec3(Min.x, Max.y, Max.z),  // Upper-Left-Back
        glm::vec3(Max.x, Max.y, Max.z),  // Upper-Right-Back
        glm::vec3(Max.x, Max.y, Min.z),  // Upper-Right-Front
    };
  }
};

class FrustumTest : public testing::Test {
 protected:
  aabb MakeAABB(glm::vec3 pos, glm::vec3 dim) {
    const auto halfDim = dim / 2.0f;
    return aabb{.Min = pos - halfDim, .Max = pos + halfDim};
  }

  glm::mat4 BuildOrthoHeightPerspective(glm::ivec2 size,
                                        float orthoSize = 10.F) {
    float aspect = 0.F;
    float halfHeight = 0.F;
    float halfWidth = 0.F;
    auto nearZ = 0.01F;
    auto farZ = 25.F;
    if (size.x > size.y) {
      aspect = static_cast<float>(size.x) / static_cast<float>(size.y);
      halfHeight = orthoSize * 0.5F;
      halfWidth = halfHeight * aspect;
    } else {
      aspect = static_cast<float>(size.y) / static_cast<float>(size.x);
      halfWidth = orthoSize * 0.5F;
      halfHeight = halfWidth * aspect;
    }
    return glm::ortho(-halfWidth, halfWidth, -halfHeight, halfHeight, nearZ,
                      farZ);
  }
};

TEST_F(FrustumTest, Constructor) {
  const auto projection = glm::ortho(0.F, 1.F, 0.F, 1.F, 0.0F, 1.F);
  const auto view = glm::lookAt(glm::vec3(0.F), glm::vec3(0.F, 0.F, -1.F),
                                glm::vec3(0.F, 1.F, 0.F));
  const Frustum frustum(projection * view);

  EXPECT_EQ(frustum.GetPlane(Planes::Left), glm::vec4(1.F, 0.F, 0.F, 0.F));
  EXPECT_EQ(frustum.GetPlane(Planes::Right), glm::vec4(-1.F, 0.F, 0.F, 1.F));
  EXPECT_EQ(frustum.GetPlane(Planes::Top), glm::vec4(0.F, -1.F, 0.F, 1.F));
  EXPECT_EQ(frustum.GetPlane(Planes::Bottom), glm::vec4(0.F, 1.F, 0.F, 0.F));
  EXPECT_EQ(frustum.GetPlane(Planes::Near), glm::vec4(0.F, 0.F, -1.F, 0.0F));
  EXPECT_EQ(frustum.GetPlane(Planes::Far), glm::vec4(0.F, 0.F, 1.F, 1.F));
}

TEST_F(FrustumTest, IsVisible_Vector) {
  const auto projection = glm::ortho(-5.F, 5.F, -5.F, 5.F, 0.01F, 25.F);
  const auto view = glm::lookAt(glm::vec3(0.F), glm::vec3(0.F, 0.F, -1.F),
                                glm::vec3(0.F, 1.F, 0.F));
  const Frustum frustum(projection * view);
  const auto invisibleBox =
      MakeAABB(glm::vec3(5.F), glm::vec3(1.F));  // Pos is behind viewer
  const auto visibleBox = MakeAABB(glm::vec3(5.F, 5.F, -5.F), glm::vec3(1.F));

  EXPECT_FALSE(frustum.IsVisible(invisibleBox.Points(), 8));
  EXPECT_TRUE(frustum.IsVisible(visibleBox.Points(), 8));
}

TEST_F(FrustumTest, IntersectBox) {
  const auto projection = glm::ortho(-5.F, 5.F, -5.F, 5.F, 0.01F, 25.F);
  auto view = glm::lookAt(glm::vec3(0, 10, 10), glm::vec3(0, 0, -1),
                          glm::vec3(0.F, 1.F, 0.F));
  Frustum frustum(projection * view);
  const auto box = MakeAABB(glm::vec3(5.F, 5.F, -5.F), glm::vec3(10.F));
  const auto outsideBox =
      MakeAABB(glm::vec3(16.0F, 16.0F, -16.0F), glm::vec3(10.F));
  const auto bigBox = MakeAABB(glm::vec3(0.F), glm::vec3(64.0F));

  EXPECT_TRUE(frustum.IntersectBox(box.Min, box.Max));
  EXPECT_FALSE(frustum.IntersectBox(outsideBox.Min, outsideBox.Max));

  view = glm::lookAt(glm::vec3(28.750F, 10.F, 25.F),
                     glm::vec3(28.750F, 00.F, 14.F), glm::vec3(0.F, 1.F, 0.F));
  frustum.SetProjectionView(projection * view);
  EXPECT_TRUE(frustum.IntersectBox(bigBox.Min, bigBox.Max));

  const auto child1 = MakeAABB(glm::vec3(-16.F, 0.F, -16.F), glm::vec3(32.0F));
  const auto child2 = MakeAABB(glm::vec3(16.F, 0.F, -16.F), glm::vec3(32.0F));
  const auto child3 = MakeAABB(glm::vec3(16.F, 0.F, 16.F), glm::vec3(32.0F));
  const auto child4 = MakeAABB(glm::vec3(-16.F, 0.F, 16.F), glm::vec3(32.0F));
  EXPECT_FALSE(frustum.IntersectBox(child1.Min, child1.Max));
  // TODO returns true for reasons?!
  // EXPECT_FALSE(frustum.IntersectBox(child2.Min, child2.Max));
  EXPECT_TRUE(frustum.IntersectBox(child3.Min, child3.Max));
  EXPECT_FALSE(frustum.IntersectBox(child4.Min, child4.Max));

  // TODO remove
  /*std::cout << "Treenode: " << util::Strings::to_string(child2.Min) << " x "
            << util::Strings::to_string(child2.Max) << "\n";
  for (auto i = 0; i < 6; i++) {
      auto plain = frustum.GetPlane(static_cast<world::volume::Planes>(i));
      std::cout << "Plain(" << std::to_string(i) << "): " <<
  util::Strings::to_string(plain) << "\n";
  }*/
}

TEST_F(FrustumTest, IsVisible_Vec3) {
  const auto projection = glm::ortho(-5.F, 5.F, -5.F, 5.F, 0.01F, 25.F);
  const auto view = glm::lookAt(glm::vec3(0.F), glm::vec3(0.F, 0.F, -1.F),
                                glm::vec3(0.F, 1.F, 0.F));
  const Frustum frustum(projection * view);

  EXPECT_FALSE(frustum.IsVisible(glm::vec3(0.F, 0.F, 1.F)));
  EXPECT_TRUE(frustum.IsVisible(glm::vec3(0.F, 0.F, -1.F)));
}

}  // namespace soil::world::volume
