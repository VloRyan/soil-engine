#include "world/world.h"

#include <gtest/gtest.h>

#include "world/volume/aabb.h"
#include "world/volume/mocks.hpp"
namespace soil::world {
class WorldTest : public testing::Test {
 protected:
  [[nodiscard]] static entity::CollisionObject* NewObjectAt(
      const glm::vec3 pos, const glm::vec3 dim) {
    auto* volume = new volume::AABB(dim);
    auto* obj = new entity::CollisionObject(
        volume, entity::CollisionObject::ContactType::StaticObject);
    obj->SetPosition(pos);
    return obj;
  }
};

TEST_F(WorldTest, Update) {
  auto container = volume::ContainerMock();
  auto world = World({.Friction = 0.5F}, &container);
  auto* object = NewObjectAt(glm::vec3(0.F), glm::vec3(1.F));
  object->SetVelocity(glm::vec3(0.F, -1.F, 0.F));
  world.Insert(object);

  world.Update();

  EXPECT_EQ(object->GetPosition(), glm::vec3(0.F, -1.F, 0.F));
  EXPECT_EQ(object->GetVelocity(), glm::vec3(0.F, -0.5F, 0.F));
  EXPECT_EQ(object->GetState(),
            soil::world::entity::CollisionObject::State::Active);
}

TEST_F(WorldTest, Activate) {
  auto container = volume::ContainerMock();
  auto world = World({.Friction = 0.5F}, &container);
  auto* object = NewObjectAt(glm::vec3(0.F), glm::vec3(1.F));
  world.Insert(object);
  object->SetVelocity(glm::vec3(0.F, -1.F, 0.F));

  world.Update();
  EXPECT_EQ(object->GetPosition(), glm::vec3(0.F, 0.F, 0.F));
  EXPECT_EQ(object->GetState(),
            soil::world::entity::CollisionObject::State::Inactive);

  world.Activate(object);

  world.Update();
  EXPECT_EQ(object->GetPosition(), glm::vec3(0.F, -1.F, 0.F));
  EXPECT_EQ(object->GetState(),
            soil::world::entity::CollisionObject::State::Active);
}

TEST_F(WorldTest, Inactivate) {
  auto container = volume::ContainerMock();
  auto world = World({.Friction = 0.5F}, &container);
  auto* object = NewObjectAt(glm::vec3(0.F), glm::vec3(1.F));
  object->SetVelocity(glm::vec3(0.F, -1.F, 0.F));
  world.Insert(object);
  world.SetMaxIdleTime(2);

  world.Update();
  EXPECT_EQ(object->GetState(),
            soil::world::entity::CollisionObject::State::Active);

  object->SetVelocity(glm::vec3(0.F, 0.F, 0.F));
  world.Update();
  EXPECT_EQ(object->GetState(),
            soil::world::entity::CollisionObject::State::Idle);

  world.Update();
  EXPECT_EQ(object->GetState(),
            soil::world::entity::CollisionObject::State::Inactive);

  object->SetVelocity(glm::vec3(0.F, -1.F, 0.F));
  world.Update();
  EXPECT_EQ(object->GetState(),
            soil::world::entity::CollisionObject::State::Inactive);
}

}  // namespace soil::world
