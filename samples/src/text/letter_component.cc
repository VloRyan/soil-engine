#include "letter_component.h"

#include "node.h"

namespace soil_samples::text {
    std::vector<soil::video::vertex::VertexAttribDescriptor> LetterComponent::ATTRIBS = {
        // aPositionWorldspace
        {.Elements = 3, .Type = soil::video::vertex::AttributePointer::DataType::Float},
        // aSize
        {.Elements = 2, .Type = soil::video::vertex::AttributePointer::DataType::Float},
        // aColor
        {.Elements = 4, .Type = soil::video::vertex::AttributePointer::DataType::Float},
        // aBorderColor
        {.Elements = 3, .Type = soil::video::vertex::AttributePointer::DataType::Float},
        // aTexCoords
        {.Elements = 2, .Type = soil::video::vertex::AttributePointer::DataType::Float},
        // aTexCoordsMax
        {.Elements = 2, .Type = soil::video::vertex::AttributePointer::DataType::Float},
        // aCharOutline
        {.Elements = 2, .Type = soil::video::vertex::AttributePointer::DataType::Float},
        // aBorderOutline
        {.Elements = 2, .Type = soil::video::vertex::AttributePointer::DataType::Float}};

    LetterComponent::LetterComponent(const bool opaque, const soil::video::model::Letter* letter,
                                     const float lineHeight, const glm::vec2 offset, const float scale) :
        InstanceData(BATCH_NAME, opaque), localPosition(0.F) {
        SetLetter(letter, offset, lineHeight, scale);
    }

    void LetterComponent::UpdateTransform(const glm::mat4& matrix) {
        InstanceData::UpdateTransform(matrix);
        const auto parentPos = matrix[3];
        data_.Position = glm::vec3(localPosition.x + parentPos.x, localPosition.y + parentPos.y, parentPos.z);
    }

    void LetterComponent::SetOutline(const glm::vec2 outline) {
        if (data_.Outline == outline) {
            return;
        }
        data_.Outline = outline;
        SetDirty();
    }

    void LetterComponent::SetBorderOutline(const glm::vec2 borderOutline) {
        if (data_.BorderOutline == borderOutline) {
            return;
        }
        data_.BorderOutline = borderOutline;
        SetDirty();
    }

    void LetterComponent::SetColor(const glm::vec4 color) {
        if (data_.Color == color) {
            return;
        }
        data_.Color = color;
        SetDirty();
    }

    void LetterComponent::SetBorderColor(const glm::vec3 color) {
        if (data_.BorderColor == color) {
            return;
        }
        data_.BorderColor = color;
        SetDirty();
    }

    void LetterComponent::WriteData(soil::video::buffer::Cursor* cursor) const {
        cursor->Write(&data_, sizeof(Data));
    }

    void LetterComponent::SetLetter(const soil::video::model::Letter* letter, const glm::vec2& offset,
                                    const float lineHeight, float scale) {
        const glm::vec2 halfSize = (letter->Size * glm::vec2(0.5F)) * scale;
        const glm::vec2 fontOffset = letter->Offset * scale;

        const glm::vec2 centerPosition(fontOffset.x + halfSize.x, (lineHeight * scale) - fontOffset.y - halfSize.y);

        localPosition = glm::vec2(offset.x + centerPosition.x, offset.y + centerPosition.y);
        if (GetParent() != nullptr) {
            const auto parentPos = GetParent()->GetWorldPosition();
            data_.Position = glm::vec3(localPosition.x + parentPos.x, localPosition.y + parentPos.y, parentPos.z);
        } else {
            data_.Position = glm::vec3(localPosition.x, localPosition.y, 0.F);
        }

        data_.Size = letter->Size * scale;
        data_.TexCoords = letter->TextureCoord;
        data_.TexCoordsMax = letter->MaxTextureCoord;
        SetDirty();
    }
} // namespace soil_samples::text
