#include "stage/scene/component/collision_object_component.h"

#include <gtest/gtest.h>

namespace soil::stage::scene::component {
class CollisionObjectComponentTest : public testing::Test {};

TEST_F(CollisionObjectComponentTest, Contruct) {
  const auto volume = CollisionObjectComponent(
      nullptr, soil::world::entity::CollisionObject::ContactType::Object);
}
}  // namespace soil::stage::scene::component
