#ifndef SOIL_EXAMPLES_TEXT_NODE_H
#define SOIL_EXAMPLES_TEXT_NODE_H
#include "common/component/text_component.h"
#include "stage/scene/node.h"

namespace soil_samples::text {
class Node final : public soil::stage::scene::Node {
 public:
  explicit Node(const std::string& fontPrefab, const std::string& text = "");
  ~Node() override = default;

  [[nodiscard]] common::component::TextComponent& Text() const;

 private:
  common::component::TextComponent* textComponent_;
};
}  // namespace soil_samples::text

#endif
