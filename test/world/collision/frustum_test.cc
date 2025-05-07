#include "world/collision/frustum.h"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <gtest/gtest.h>

namespace soil::world::collision {

    TEST(FrustumTest, Constructor) {
        auto view = glm::lookAt(glm::vec3(0.0F), glm::vec3(0.0F, 0.0F, -1.0F), glm::vec3(0.0F, 1.0F, 0.0F));
        Frustum frustum(view);
        EXPECT_EQ(frustum.GetPlane(Planes::Top), glm::vec4(0.0F, -1.0F, 0.0F, 1.0F));
        EXPECT_EQ(frustum.GetPlane(Planes::Bottom), glm::vec4(0.0F, 1.0F, 0.0F, 1.0F));
        EXPECT_EQ(frustum.GetPlane(Planes::Left), glm::vec4(1.0F, 0.0F, 0.0F, 1.0F));
        EXPECT_EQ(frustum.GetPlane(Planes::Right), glm::vec4(-1.0F, 0.0F, 0.0F, 1.0F));
        EXPECT_EQ(frustum.GetPlane(Planes::Far), glm::vec4(0.0F, 0.0F, -1.0F, 1.0F));
        EXPECT_EQ(frustum.GetPlane(Planes::Near), glm::vec4(0.0F, 0.0F, 1.0F, 1.0F));
    }

    TEST(FrustumTest, IsVisible_Vec3) {
        auto projection = glm::perspective(70.0F, 16.0F / 9.0F, 0.1F, 100.0F);
        auto view = glm::lookAt(glm::vec3(0.0F), glm::vec3(0.0F, 0.0F, -1.0F), glm::vec3(0.0F, 1.0F, 0.0F));
        Frustum frustum(view * projection);
        EXPECT_FALSE(frustum.IsVisible(glm::vec3(0, 0, 1)));
        EXPECT_TRUE(frustum.IsVisible(glm::vec3(0, 0, -1)));
    }

} // namespace soil::world::collision
