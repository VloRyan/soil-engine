#ifndef TEXT_LETTER_COMPONENT_H
#define TEXT_LETTER_COMPONENT_H

#include "stage/scene/component/instance_data.h"
#include "video/model/letter.h"
#include "video/shader/shader.h"

namespace soil_samples::text {
    class LetterComponent final : public soil::stage::scene::component::InstanceData {
    public:
        LetterComponent(bool opaque, const soil::video::model::Letter* letter, float lineHeight, glm::vec2 offset,
                        float scale);


        void SetLetter(const soil::video::model::Letter* letter, const glm::vec2& offset, float lineHeight,
                       float scale);

        void SetColor(glm::vec4 color);
        void SetBorderColor(glm::vec3 color);

        void SetOutline(glm::vec2 outline);
        void SetBorderOutline(glm::vec2 borderOutline);


        void UpdateTransform(const glm::mat4& matrix) override;

        void WriteData(soil::video::buffer::Cursor* cursor) const override;

        inline static const auto BATCH_NAME = std::string("Letter");
        static std::vector<soil::video::vertex::VertexAttribDescriptor> ATTRIBS;

    private:
        struct Data {
            glm::vec3 Position{0.F};
            glm::vec2 Size{1.F};
            glm::vec4 Color{1.F};
            glm::vec3 BorderColor{0.F};
            glm::vec2 TexCoords{0.F};
            glm::vec2 TexCoordsMax{1.F};
            glm::vec2 Outline{0.F};
            glm::vec2 BorderOutline{0.F};
        };

        Data data_;
        glm::vec2 localPosition;
    };
} // namespace soil_samples::text

#endif
