#include "gtest/gtest.h"
#include "stage/scene/gui/root.h"
#include "testing.h"
#include "stage/scene/gui/container/v_box.h"
#include "stage/scene/gui/layout/sizer.h"

namespace soil::stage::scene::gui {
class GuiTest : public testing::Test {
 protected:
  class FixedContentSizePane : public Rectangle {
   public:
    FixedContentSizePane(glm::ivec2 contentSize) : contentSize_(contentSize) {}

    glm::ivec2 CalculateSize(const glm::ivec2& maxSize) override {
      glm::ivec2 contentMinSize = glm::max(minSize_, contentSize_);
      switch (sizeType_) {
        case Rectangle::SizeTypes::Fixed:
          return glm::clamp(size_, contentMinSize, maxSize_);
        case Rectangle::SizeTypes::Relative: {
          return glm::clamp(layout::Sizer::CalculateRelativeSize(
                                GetRelativeSize(), GetAspectRatio(), maxSize,
                                contentMinSize),
                            contentMinSize, maxSize_);
        }
        case Rectangle::SizeTypes::GrowWithContent:
          return contentSize_;
      }
      return contentSize_;
    }

   private:
    glm::ivec2 contentSize_;
  };

  static float ZLayer(int layer) {
    return -Rectangle::TOP_Z_LAYER + static_cast<float>(layer) * Rectangle::LAYER_Z_INCREMENT;
  }
};
TEST_F(GuiTest, Layout) {
  auto root = Root(glm::ivec2(800, 600));
  auto* menu = root.AddChild(new Rectangle());
  menu->SetAnchor({layout::Alignment::Horizontal::Right, layout::Alignment::Vertical::Center});
  menu->SetVisible(false);
  menu->SetSizeType(Rectangle::SizeTypes::GrowWithContent);
  menu->SetMaxSize(glm::ivec2(500, 1024));
  menu->SetMinSize(glm::ivec2(300, 400));

  auto* menuContainer = menu->AddChild(new container::VBox());
  menuContainer->SetAnchor({layout::Alignment::Horizontal::Center, layout::Alignment::Vertical::Top});
  auto* list = menuContainer->AddChild(new container::VBox());

  auto* label = list->AddChild(new FixedContentSizePane(glm::ivec2(100, 60)));
  label->SetRelativeSize(glm::vec2(0.8F, 0.F));

  EXPECT_VEC_EQ(menu->GetSize(), glm::ivec2(0));
  EXPECT_VEC_EQ(list->GetSize(), glm::ivec2(0));
  EXPECT_VEC_EQ(label->GetSize(), glm::ivec2(0));

  EXPECT_EQ_RND(menu->GetPosition(), glm::vec3(400.F, 300.F, -Rectangle::TOP_Z_LAYER));
  EXPECT_EQ_RND(menuContainer->GetPosition(), glm::vec3(400.F, 300.F, -Rectangle::TOP_Z_LAYER));
  EXPECT_EQ_RND(list->GetPosition(), glm::vec3(400.F, 300.F, -Rectangle::TOP_Z_LAYER));
  EXPECT_EQ_RND(label->GetPosition(), glm::vec3(400.F, 300.F, -Rectangle::TOP_Z_LAYER));

  menu->SetVisible(true);
  root.Update();
  // x: 400 = menu->minSize.x(500) * label->relativeSize.x(0.8); y: menu->minSize
  EXPECT_VEC_EQ(menu->GetSize(), glm::ivec2(400, 400));
  EXPECT_VEC_EQ(menuContainer->GetSize(), glm::ivec2(400, 60));
  EXPECT_VEC_EQ(list->GetSize(), glm::ivec2(400, 60));
  EXPECT_VEC_EQ(label->GetSize(), glm::ivec2(400, 60));

  EXPECT_EQ_RND(menu->GetPosition(), glm::vec3(600.F, 300.F, ZLayer(1)));
  EXPECT_EQ_RND(menuContainer->GetPosition(), glm::vec3(600.F, 470.F, ZLayer(2)));
  EXPECT_EQ_RND(list->GetPosition(), glm::vec3(600.F, 470.F, ZLayer(3)));
  EXPECT_EQ_RND(label->GetPosition(), glm::vec3(600.F, 470.F, ZLayer(4)));
}
}
