#ifndef TEXT_NODE_H
#define TEXT_NODE_H
#include "gui/component/text.h"
#include "stage/scene/node.h"

namespace soil_samples::text {
    class Node final : public soil::stage::scene::Node {
    public:
        explicit Node(const std::string& fontPrefab, const std::string& text = "");
        ~Node() override = default;

        [[nodiscard]] gui::component::Text& Text() const;

    private:
        gui::component::Text* textComponent_;
    };
} // namespace soil_samples::text

#endif
