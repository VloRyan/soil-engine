#include "gtest/gtest.h"

#include "gmock/gmock-matchers.h"
#include "stage/scene/component/aabb.h"
#include "stage/scene/node.h"


#include "glm/glm.hpp"
#include "stage/scene/scene.h"
#include "stage/stage.h"

namespace soil::stage::scene::space {
    static Manager *sm = nullptr;

    class QuadTreeTest : public testing::Test {
    protected:
        static void SetUpTestSuite() { sm = new Manager(); }

        static void TearDownTestSuite() { delete sm; }
        void SetUp() override {
            const auto stage = new Stage(sm);
            scene = new Scene(stage);
        }

        [[nodiscard]] component::BoundingVolume *NewBoundingVolumeAt(const glm::vec3 pos, const glm::vec3 dim) const {
            if (sm == nullptr) {
                throw std::runtime_error("sm is nullptr");
            }
            auto *const parentNode = new Node(scene, Node::Type::Game, {});
            parentNode->SetPosition(pos);
            parentNode->Update();
            auto *volume = new component::AABB(dim);
            parentNode->AddComponent(volume);
            parentNode->Update();
            return volume;
        }

    private:
        Scene *scene = nullptr;
    };

    TEST_F(QuadTreeTest, InsertOneItem) {
        auto *quadTree = new QuadTree(20, 10, 5.0F);
        auto *volume = NewBoundingVolumeAt({-5.0F, 0.0F, -5.0F}, glm::vec3(5.0F));

        quadTree->Insert(volume);

        ASSERT_EQ(quadTree->GetNodeCount(), 1);
        ASSERT_THAT(quadTree->GetNode(0)->Contained, testing::ElementsAre(volume));
    }

    TEST_F(QuadTreeTest, InsertFourItems) {
        auto *quadTree = new QuadTree(20, 10, 5.0F);
        const auto *topLeft = NewBoundingVolumeAt({-5.0F, 0.0F, -5.0F}, glm::vec3(5.0F));
        const auto *topRight = NewBoundingVolumeAt({5.0F, 0.0F, -5.0F}, glm::vec3(5.0F));
        const auto *bottomRight = NewBoundingVolumeAt({5.0F, 0.0F, 5.0F}, glm::vec3(5.0F));
        const auto *bottomLeft = NewBoundingVolumeAt({-5.0F, 0.0F, 5.0F}, glm::vec3(5.0F));

        quadTree->Insert(topLeft);
        quadTree->Insert(topRight);
        quadTree->Insert(bottomRight);
        quadTree->Insert(bottomLeft);

        ASSERT_EQ(quadTree->GetNodeCount(), 1);
        ASSERT_THAT(quadTree->GetNode(0)->Contained, testing::ElementsAre(topLeft, topRight, bottomRight, bottomLeft));
    }

    TEST_F(QuadTreeTest, InsertFiveItems) {
        auto *quadTree = new QuadTree(20, 10, 5.0F);
        const auto *topLeft = NewBoundingVolumeAt({-5.0F, 0.0F, -5.0F}, glm::vec3(5.0F));
        const auto *topLeft2 = NewBoundingVolumeAt({-5.0F, 0.0F, -5.0F}, glm::vec3(4.0F));
        const auto *topRight = NewBoundingVolumeAt({5.0F, 0.0F, -5.0F}, glm::vec3(5.0F));
        const auto *bottomRight = NewBoundingVolumeAt({5.0F, 0.0F, 5.0F}, glm::vec3(5.0F));
        const auto *bottomLeft = NewBoundingVolumeAt({-5.0F, 0.0F, 5.0F}, glm::vec3(5.0F));

        quadTree->Insert(topLeft);
        quadTree->Insert(topLeft2);
        quadTree->Insert(topRight);
        quadTree->Insert(bottomRight);
        quadTree->Insert(bottomLeft);

        ASSERT_EQ(quadTree->GetNodeCount(), 5);
        ASSERT_TRUE(quadTree->GetNode(0)->Contained.empty());
        ASSERT_THAT(quadTree->GetNode(1)->Contained, testing::ElementsAre(topLeft, topLeft2));
        ASSERT_THAT(quadTree->GetNode(2)->Contained, testing::ElementsAre(topRight));
        ASSERT_THAT(quadTree->GetNode(3)->Contained, testing::ElementsAre(bottomLeft));
        ASSERT_THAT(quadTree->GetNode(4)->Contained, testing::ElementsAre(bottomRight));
    };
} // namespace soil::stage::scene::space
