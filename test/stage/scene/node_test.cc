#include "stage/scene/node.h"
#include "gmock/gmock-matchers.h"
#include "gtest/gtest.h"
#include "stage/manager.h"
#include "stage/scene/scene.h"
#include "stage/stage.h"

namespace soil::stage::scene {
    static Manager *sm = nullptr;

    class NodeTest : public testing::Test {
    protected:
        static void SetUpTestSuite() { sm = new Manager(); }

        static void TearDownTestSuite() { delete sm; }

        void SetUp() override {
            const auto stage = new Stage(sm);
            scene = new Scene(stage);
        }
        Scene *scene = nullptr;
    };

    TEST_F(NodeTest, Contruct) {
        const auto node = Node(nullptr, Node::Type::Game, {});

        ASSERT_EQ(node.GetParent(), nullptr);
        ASSERT_EQ(node.GetType(), Node::Type::Game);
        ASSERT_EQ(node.GetReceiveTypes(), 0);
        ASSERT_EQ(node.GetName(), "");
    }

    TEST_F(NodeTest, AddChildByConstructor) {
        auto parent = Node(scene, Node::Type::Game, {});

        const auto child = Node(&parent, Node::Type::Game, {});

        ASSERT_EQ(child.GetParent(), &parent);
        ASSERT_THAT(parent.GetChildren(), testing::ElementsAre(&child));
    }

    TEST_F(NodeTest, AddChildBySetParent) {
        auto parent = Node(scene, Node::Type::Game, {});

        auto child = Node(nullptr, Node::Type::Game, {});

        child.SetParent(&parent);

        ASSERT_EQ(child.GetParent(), &parent);
        ASSERT_THAT(parent.GetChildren(), testing::ElementsAre(&child));
    }

    TEST_F(NodeTest, ChildDestrucor) {
        auto parent = Node(scene, Node::Type::Game, {});
        const auto *child = new Node(&parent, Node::Type::Game, {});

        ASSERT_FALSE(parent.GetChildren().empty());
        delete child;
        ASSERT_TRUE(parent.GetChildren().empty());
    }
} // namespace soil::stage::scene
