#ifndef GUI_TEXT_H
#define GUI_TEXT_H
#include "stage/scene/component/text/abstract_text.h"
#include "stage/scene/node.h"
#include "stage/scene/text/parser.h"
#include "video/texture/texture.h"

namespace soil_samples::gui::component {
    class Text : public soil::stage::scene::component::text::AbstractText {
    public:
        explicit Text(const std::string& prefab, const std::string& text = "");
        ~Text() override = default;

        void Render(soil::video::render::State& state) override;
        float DistanceTo(const glm::vec3& point) override;
    };
} // namespace soil_samples::gui::component

#endif
