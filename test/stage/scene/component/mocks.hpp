#ifndef TEST_STAGE_SCENE_COMPONENT_MOCKS
#define TEST_STAGE_SCENE_COMPONENT_MOCKS

#include "glm/glm.hpp"
#include "stage/scene/component/component.h"

namespace soil::stage::scene::component {
class ComponentMock : public Component {
 public:
  explicit ComponentMock(const Type type) : Component(type) {}

  ~ComponentMock() override = default;

  int UpdateCalledCount = 0;

  void Update() override {
    UpdateCalledCount++;
    Component::Update();
  }

  void ResetMocks() { UpdateCalledCount = 0; }
};
}  // namespace soil::stage::scene::component
#endif
