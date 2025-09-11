#include "shape_tile_instance.h"

#include "video/vertex/vertex.h"

namespace soil_samples::gui::component {
    std::vector<soil::video::vertex::VertexAttribDescriptor> ShapeTileInstance::ATTRIBS = {
        // location(1): aMatrix
        {.Elements = 4, .Type = soil::video::vertex::AttributePointer::DataType::Float},
        // location(2): aMatrix
        {.Elements = 4, .Type = soil::video::vertex::AttributePointer::DataType::Float},
        // location(3): aMatrix
        {.Elements = 4, .Type = soil::video::vertex::AttributePointer::DataType::Float},
        // location(4): aMatrix
        {.Elements = 4, .Type = soil::video::vertex::AttributePointer::DataType::Float},
        // location(5): aSize
        {.Elements = 2, .Type = soil::video::vertex::AttributePointer::DataType::Float},
        // location(6): aColor
        {.Elements = 4, .Type = soil::video::vertex::AttributePointer::DataType::Float},
        // location(7): aTileIndex
        {.Elements = 1, .Type = soil::video::vertex::AttributePointer::DataType::Int},
        // location(8): aTileScale
        {.Elements = 2, .Type = soil::video::vertex::AttributePointer::DataType::Float},
    };

    ShapeTileInstance::ShapeTileInstance(const bool isOpaque) :
        InstanceData(BATCH_NAME, isOpaque), positionOffset_(0.F) {}

    void ShapeTileInstance::UpdateTransform(const glm::mat4& transform) {
        InstanceData::UpdateTransform(transform);
        data_.Transform = transform;
        data_.Transform[3] += glm::vec4(positionOffset_, 0.0F);
    }

    void ShapeTileInstance::WriteData(soil::video::buffer::Cursor* cursor) const {
        cursor->Write(&data_, sizeof(data_));
    }

    int ShapeTileInstance::GetTileIndex() const {
        return data_.TileIndex;
    }

    void ShapeTileInstance::SetTileIndex(const int index) {
        if (data_.TileIndex == index) {
            return;
        }
        data_.TileIndex = index;
        SetDirty();
    }

    void ShapeTileInstance::SetTileScale(const glm::vec2 scale) {
        if (data_.TileScale == scale) {
            return;
        }
        data_.TileScale = scale;
        SetDirty();
    }

    glm::vec2 ShapeTileInstance::GetTileScale() const {
        return data_.TileScale;
    }

    glm::vec2 ShapeTileInstance::GetSize() const {
        return data_.Size;
    }

    void ShapeTileInstance::SetSize(const glm::vec2& size) {
        if (data_.Size == size) {
            return;
        }
        data_.Size = size;
        SetDirty();
    }

    glm::vec4 ShapeTileInstance::GetColor() const {
        return data_.Color;
    }

    void ShapeTileInstance::SetColor(const glm::vec4& color) {
        if (data_.Color == color) {
            return;
        }
        data_.Color = color;
        SetOpaque(color.a == 1.0F);
        SetDirty();
    }

    void ShapeTileInstance::SetPositionOffset(glm::vec3 offset) {
        if (offset == positionOffset_) {
            return;
        }
        positionOffset_ = offset;
        SetDirty();
    }

    glm::vec3 ShapeTileInstance::GetPositionOffset() const {
        return positionOffset_;
    }


} // namespace soil_samples::gui::component
