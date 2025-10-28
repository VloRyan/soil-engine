#include "stage/scene/component/transform_component.h"

#include "../mocks.hpp"
#include "gtest/gtest.h"
#include "mocks.hpp"
#include "stage/scene/component/component.h"

namespace soil::stage::scene::component {
class TransformComponentTest : public testing::Test {};

TEST_F(TransformComponentTest, SetPosition) {
  auto component = TransformComponent();
  ASSERT_EQ(component.GetPosition(), glm::vec3(0.F));

  component.SetPosition(glm::vec3(100.F));
  EXPECT_EQ(component.GetPosition(), glm::vec3(100.F));

  component.UpdateTransform(glm::mat4(1.F));
  EXPECT_EQ(component.GetPosition(), glm::vec3(100.F));

  auto parent = component.GetMatrix();
  component.UpdateTransform(parent);
  EXPECT_EQ(component.GetPosition(), glm::vec3(200.F));
}
/*
TEST_F(TransformComponentTest, SetDirection) {
  auto node = Node(Node::Type::Transform);
  node.SetDirection(glm::vec3(0.F, 1.F, 0.F));
  node.Update();

  EXPECT_EQ(node.GetDirection(), glm::vec3(0.F, 1.F, 0.F));

  auto* child = node.AddChild(new Node(Node::Type::Transform));
  node.Update();

  EXPECT_EQ(node.GetDirection(), glm::vec3(0.F, 1.F, 0.F));
  EXPECT_EQ(child->GetDirection(), node.GetDirection());

  child->SetDirection(glm::vec3(0.5F, 0.5F, 0.F));
  child->Update();
  EXPECT_EQ(node.GetDirection(), glm::vec3(0.F, 1.F, 0.F));
  EXPECT_EQ(child->GetDirection(), glm::vec3(0.5F, 0.5F, 0.F));
  EXPECT_EQ(child->GetLocalTransform()[2], glm::vec4(0.5F, 0.5F, 0.F, 0.F));

  child->SetDirection(glm::vec3(-0.25F, -0.25F, 0.5F));
  child->Update();
  EXPECT_EQ(node.GetDirection(), glm::vec3(0.F, 1.F, 0.F));
  // TODO
  //  EXPECT_EQ(child->GetDirection(), glm::vec3(-0.25F, -0.25F, 0.5F));
  /*EXPECT_EQ(child->GetLocalTransform()[2],
            glm::vec4(-0.25F, -0.25F, 0.5F, 0.F));
}
*/
/*
TEST_F(TransformComponentTest, SetDirection2) {
  auto m1 = glm::lookAt(glm::vec3(0.F), glm::vec3(0.5F, 0.5F, 0.F),
                        glm::vec3(0, 1, 0));
  // auto m1 = glm::mat4(1.F);
  m1[3] = glm::vec4(10, 15, 5, 1.F);
  // m1[2] = glm::vec4(0.F, -0.34F, 0.67F, 0.F);
  auto m2 = glm::mat4(1.F);
  // m2[2] = glm::vec4(-0.25F, -10.75F, 0.5F, 0.F);
  //  m2[0][2] = 0.F;
  m2[1][2] = 0.F;
  m2[2][2] = 1.492537313F;

auto m2T = m1;
m2T[2] = glm::vec4(-0.25F, -0.25F, 0.5F, 0.F);
m2 = glm::transpose(m2T) / m1;
// m2 = m1 / m2T;
//  m2[3] = glm::vec4(0.F, 0.F, 0.F, 1.F);
//   m2[2][2] = 0.F;
auto m3 = m1 * m2;
for (auto i = 0; i < 4; i++) {
  std::cout << std::fixed << std::setprecision(2) << m1[0][i] << ", "
            << m1[1][i] << ", " << m1[2][i] << ", " << m1[3][i];
  if (i == 0) {
    std::cout << "\tx ";
  } else {
    std::cout << "\t  ";
  }

  std::cout << std::fixed << std::setprecision(2) << m2[0][i] << ", "
            << m2[1][i] << ", " << m2[2][i] << ", " << m2[3][i];
  if (i == 0) {
    std::cout << "\t= ";
  } else {
    std::cout << "\t  ";
  }
  std::cout << std::fixed << std::setprecision(2) << m3[0][i] << ", "
            << m3[1][i] << ", " << m3[2][i] << ", " << m3[3][i] << std::endl;
}
}*/

}  // namespace soil::stage::scene::component
