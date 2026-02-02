#include "node.h"

#include "stage/scene/scene.h"

namespace soil_samples::text {
Node::Node(const std::string& fontPrefab, const std::string& text)
    : soil::stage::scene::Node(Type::Visual), textComponent_(nullptr) {
  textComponent_ =
      AddComponent(new common::component::TextComponent(fontPrefab, text));
}

[[nodiscard]] common::component::TextComponent& Node::Text() const {
  return *textComponent_;
};
}  // namespace soil_samples::text
