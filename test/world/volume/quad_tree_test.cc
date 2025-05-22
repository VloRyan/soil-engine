#include "gtest/gtest.h"

#include "gmock/gmock-matchers.h"
#include "stage/scene/node.h"
#include "world/volume/quad_tree.h"


#include "glm/glm.hpp"
#include "stage/scene/scene.h"
#include "stage/stage.h"
#include "world/volume/aabb.h"

namespace soil::world::volume {
    class QuadTreeTest : public testing::Test {
    protected:
        [[nodiscard]] static Volume *NewBoundingVolumeAt(const glm::vec2 pos, const glm::vec2 dim) {
            auto *volume = new AABB({dim.x, 0.F, dim.y});
            volume->SetPosition({pos.x, 0.F, pos.y});
            return volume;
        }
    };

    TEST_F(QuadTreeTest, InsertOneItem) {
        auto *quadTree = new QuadTree(20, 10, 5.0F);
        auto *volume = NewBoundingVolumeAt({-5.0F, -5.0F}, glm::vec3(5.0F));

        quadTree->Insert(volume);

        EXPECT_EQ(quadTree->GetNodeCount(), 1);
        EXPECT_THAT(quadTree->GetNode(0)->Contained, testing::ElementsAre(volume));
    }

    TEST_F(QuadTreeTest, InsertFourItems) {
        auto *quadTree = new QuadTree(20, 10, 5.0F);
        const auto *topLeft = NewBoundingVolumeAt({-5.0F, -5.0F}, glm::vec3(5.0F));
        const auto *topRight = NewBoundingVolumeAt({5.0F, -5.0F}, glm::vec3(5.0F));
        const auto *bottomRight = NewBoundingVolumeAt({5.0F, 5.0F}, glm::vec3(5.0F));
        const auto *bottomLeft = NewBoundingVolumeAt({-5.0F, 5.0F}, glm::vec3(5.0F));

        quadTree->Insert(topLeft);
        quadTree->Insert(topRight);
        quadTree->Insert(bottomRight);
        quadTree->Insert(bottomLeft);

        EXPECT_EQ(quadTree->GetNodeCount(), 1);
        EXPECT_THAT(quadTree->GetNode(0)->Contained, testing::ElementsAre(topLeft, topRight, bottomRight, bottomLeft));
    }

    TEST_F(QuadTreeTest, InsertFiveItems) {
        auto *quadTree = new QuadTree(20, 10, 5.0F);
        const auto *topLeft = NewBoundingVolumeAt({-5.0F, -5.0F}, glm::vec3(5.0F));
        const auto *topLeft2 = NewBoundingVolumeAt({-5.0F, -5.0F}, glm::vec3(4.0F));
        const auto *topRight = NewBoundingVolumeAt({5.0F, -5.0F}, glm::vec3(5.0F));
        const auto *bottomRight = NewBoundingVolumeAt({5.0F, 5.0F}, glm::vec3(5.0F));
        const auto *bottomLeft = NewBoundingVolumeAt({-5.0F, 5.0F}, glm::vec3(5.0F));

        quadTree->Insert(topLeft);
        quadTree->Insert(topLeft2);
        quadTree->Insert(topRight);
        quadTree->Insert(bottomRight);
        quadTree->Insert(bottomLeft);

        EXPECT_EQ(quadTree->GetNodeCount(), 5);
        EXPECT_TRUE(quadTree->GetNode(0)->Contained.empty());
        EXPECT_THAT(quadTree->GetNode(1)->Contained, testing::ElementsAre(topLeft, topLeft2));
        EXPECT_THAT(quadTree->GetNode(2)->Contained, testing::ElementsAre(topRight));
        EXPECT_THAT(quadTree->GetNode(3)->Contained, testing::ElementsAre(bottomLeft));
        EXPECT_THAT(quadTree->GetNode(4)->Contained, testing::ElementsAre(bottomRight));
    };

    TEST_F(QuadTreeTest, InsertOnEdges) {
        auto *quadTree = new QuadTree(64, 2, 4.0F);
        const auto *a = NewBoundingVolumeAt({-8.0, -8.0F}, glm::vec3(1.0F));
        const auto *b = NewBoundingVolumeAt({0, 8.0F}, glm::vec3(1.0F));
        const auto *c = NewBoundingVolumeAt({-8.0f, 0.0F}, glm::vec3(1.0F));
        const auto *d = NewBoundingVolumeAt({8.0F, 0.0F}, glm::vec3(1.0F));
        const auto *e = NewBoundingVolumeAt({0, -8.0F}, glm::vec3(1.0F));


        quadTree->Insert(a);
        quadTree->Insert(b);
        quadTree->Insert(c);
        quadTree->Insert(d);
        quadTree->Insert(e);

        EXPECT_EQ(quadTree->GetNodeCount(), 5);
        // TODO Not working
        //  ASSERT_TRUE(quadTree->GetNode(0)->Contained.empty());
        EXPECT_THAT(quadTree->GetNode(1)->Contained, testing::ElementsAre(a));
        // EXPECT_THAT(quadTree->GetNode(2)->Contained, testing::ElementsAre(b));
        // EXPECT_THAT(quadTree->GetNode(3)->Contained, testing::ElementsAre(c));
        // EXPECT_THAT(quadTree->GetNode(4)->Contained, testing::ElementsAre(d));
    };
} // namespace soil::world::volume
