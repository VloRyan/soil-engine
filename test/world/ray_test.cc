#include "world/ray.h"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

#include "gtest/gtest.h"
namespace soil::world {
class RayTest : public testing::Test {};

TEST_F(RayTest, TestCalculateOrtho) {
  constexpr auto mousePos = glm::ivec2(100);
  constexpr auto windowSize = glm::ivec2(200, 200);
  const auto projection = glm::ortho(-100.F, 100.F, -100.F, 100.F, 0.0F, 100.F);
  auto expected = Ray{.Origin = glm::vec3(0.F, 0.F, -49.F),
                      .Direction = glm::vec3(0.F, 0.F, -1.F)};
  auto actual =
      Ray::Calculate(mousePos, windowSize,
                     glm::lookAt(glm::vec3(0.F, 0.F, 1.F), glm::vec3(0.F),
                                 glm::vec3(0.F, 1.F, 0.F)),
                     projection);
  EXPECT_EQ(actual, (Ray{.Origin = glm::vec3(0.F, 0.F, -49.F),
                         .Direction = glm::vec3(0.F, 0.F, -1.F)}));

  EXPECT_EQ(Ray::Calculate(mousePos, windowSize,
                           glm::lookAt(glm::vec3(0.F, 1.F, 0.F), glm::vec3(0.F),
                                       glm::vec3(0.F, 0.F, -1.F)),
                           projection),
            (Ray{.Origin = glm::vec3(0.F, -49.F, 0.F),
                 .Direction = glm::vec3(0.F, -1.F, 0.F)}));

  EXPECT_EQ(Ray::Calculate(mousePos, windowSize,
                           glm::lookAt(glm::vec3(1.F, 0.F, 0.F), glm::vec3(0.F),
                                       glm::vec3(0.F, 1.F, 0.F)),
                           projection),
            (Ray{.Origin = glm::vec3(-49.F, 0.F, 0.F),
                 .Direction = glm::vec3(-1.F, 0.F, 0.F)}));
}
// TODO
TEST_F(RayTest, TestCalculatePerspective) {
  /* constexpr auto mousePos = glm::ivec2(100);
   constexpr auto windowSize = glm::ivec2(200, 200);
   const auto projection = glm::perspective(45.0f, 1.0f, 0.1f, 100.0f);

   EXPECT_EQ(Ray::Calculate(mousePos, windowSize,
                            glm::lookAt(glm::vec3(0.F, 0.F, 1.F),
   glm::vec3(0.F), glm::vec3(0.F, 1.F, 0.F)), projection), glm::vec3(0.F, 0.F,
   -1.F));

   EXPECT_EQ(Ray::Calculate(mousePos, windowSize,
                            glm::lookAt(glm::vec3(0.F, 1.F, 0.F),
   glm::vec3(0.F), glm::vec3(0.F, 0.F, -1.F)), projection), glm::vec3(0.F, -1.F,
   0.F));

   EXPECT_EQ(Ray::Calculate(mousePos, windowSize,
                            glm::lookAt(glm::vec3(1.F, 0.F, 0.F),
   glm::vec3(0.F), glm::vec3(0.F, 1.F, 0.F)), projection), glm::vec3(-1.F, 0.F,
   0.F));

   const auto ray =
       Ray::Calculate(mousePos, windowSize,
                      glm::lookAt(glm::vec3(0.F, 1.F, 1.F), glm::vec3(0.F),
                                  glm::vec3(0.F, 1.F, 0.F)),
                      projection);
   EXPECT_EQ(glm::length(ray.Direction), 1.F);*/
}
}  // namespace soil::world
