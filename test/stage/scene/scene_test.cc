#include "stage/scene/scene.h"
#include "gtest/gtest.h"
#include "stage/manager.h"
#include "stage/scene/viewer/ortho.h"
#include "stage/stage.h"

namespace soil::stage::scene {
    static Manager *sm = nullptr;

    class UpdateNode final : Node {
    public:
        UpdateNode(Node *parent, const Type type, const std::vector<ReceiverType> &receiveTypeFlags) :
            Node(parent, type, receiveTypeFlags) {
            Node::SetLocalTransform(glm::mat4(1));
            SetUpdateType(UpdateType::Active);
        }

        void Update() override {
            Updated = true;
            Node::Update();
        }

        bool Updated = false;
    };

    class SceneTest : public testing::Test {
    protected:
        static void SetUpTestSuite() {
            sm = new Manager();
        }

        static void TearDownTestSuite() {
            delete sm;
        }

        void SetUp() override {
            stage = new Stage(sm);
        }

        Stage *stage = nullptr;
    };

    TEST_F(SceneTest, Contruct) {
        const auto scene = new Scene(stage);

        ASSERT_EQ(scene->GetStage(), stage);
    }

    TEST_F(SceneTest, Update_active_node) {
        const auto scene = new Scene(stage);

        const auto updateNode = new UpdateNode(scene, Node::Type::Game, {});
        scene->Update();

        ASSERT_EQ(updateNode->Updated, true);
    }

    TEST_F(SceneTest, Add_viewer) {
        const auto scene = new Scene(stage);

        auto *viewer = new viewer::Ortho(scene, glm::ivec2(800, 600));

        ASSERT_EQ(scene->GetViewer(), viewer);
    }
} // namespace soil::stage::scene
