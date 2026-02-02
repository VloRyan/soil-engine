#ifndef SOIL_EXAMPLES_COMMON_NODE_LABEL_H
#define SOIL_EXAMPLES_COMMON_NODE_LABEL_H
#include "common/component/text_component.h"
#include "pane.h"
#include "text/node.h"

namespace soil_samples::common::node {
class Label final : public Pane {
 public:
  struct TextStyle {
    glm::vec4 Color{1.F};
    glm::vec4 ColorMouseOver{1.F};
  };

  explicit Label(const std::string& text = "",
                 const std::string& font = "Calibri");
  ~Label() override = default;
  void SetText(const std::string& text);
  component::TextComponent& Text() const;
  const TextStyle* GetTextStyle() const;
  void SetTextStyle(const TextStyle& textStyle);

  static inline auto HoverStyle = TextStyle{
      .Color = glm::vec4(1.0F, 1.0F, 1.0F, 0.7F),
      .ColorMouseOver = glm::vec4(1.F, 1.F, 1.F, 1.0f),
  };
  static inline auto DefaultStyle = Pane::Style{
      .BackgroundColor = glm::vec4(0.F),
      .BackgroundColorMouseOver = glm::vec4(0.F),
  };

 protected:
  void BeforeNodeUpdate() override;
  void OnMouseOver(const glm::ivec2& pos) override;
  void OnMouseOut() override;
  glm::ivec2 CalculateChildrenSize(const glm::ivec2& maxSize) override;

 private:
  component::TextComponent* text_;
  std::optional<TextStyle> textStyle_;
};
}  // namespace soil_samples::common::node

#endif
