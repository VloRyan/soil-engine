#ifndef TEST_STAGE_SCENE_COMPONENT_MOCKS
#define TEST_STAGE_SCENE_COMPONENT_MOCKS

#include "glm/glm.hpp"
#include "stage/scene/component/component.h"
#include "stage/scene/node.h"
#include "stage/scene/scene.h"

namespace soil::stage::scene::component {

    class ComponentMock : public Component {
    public:
        explicit ComponentMock(const Type type) : Component(type) {}

        ~ComponentMock() override = default;
        int UpdateCalledCount = 0;
        int UpdateMatrixCalledCount = 0;
        std::vector<glm::mat4> UpdateMatrixParam;

        void Update() override {
            UpdateCalledCount++;
            Component::Update();
        }

        void UpdateTransform(const glm::mat4& matrix) override {
            UpdateMatrixCalledCount++;
            UpdateMatrixParam.push_back(matrix);
            Component::UpdateTransform(matrix);
        }

        void ResetMocks() {
            UpdateCalledCount = 0;
            UpdateMatrixCalledCount = 0;
            UpdateMatrixParam.clear();
        }

        void SetDirty() override {
            Component::SetDirty();
        }
    };
} // namespace soil::stage::scene::component
#endif
