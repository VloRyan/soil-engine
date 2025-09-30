#include "shape_instance.h"

#include "video/vertex/vertex.h"

namespace soil_samples::instancing {
std::vector<soil::video::vertex::VertexAttribDescriptor>
    ShapeInstance::ATTRIBS = {
        // location(1): aMatrix
        {.Elements = 4,
         .Type = soil::video::vertex::AttributePointer::DataType::Float},
        // location(2): aMatrix
        {.Elements = 4,
         .Type = soil::video::vertex::AttributePointer::DataType::Float},
        // location(3): aMatrix
        {.Elements = 4,
         .Type = soil::video::vertex::AttributePointer::DataType::Float},
        // location(4): aMatrix
        {.Elements = 4,
         .Type = soil::video::vertex::AttributePointer::DataType::Float},
        // location(5): aSize
        {.Elements = 2,
         .Type = soil::video::vertex::AttributePointer::DataType::Float},
        // location(6): aColor
        {.Elements = 4,
         .Type = soil::video::vertex::AttributePointer::DataType::Float},
        // location(7): aTextureIndex
        {.Elements = 1,
         .Type = soil::video::vertex::AttributePointer::DataType::UInt},
};

ShapeInstance::ShapeInstance(const bool isOpaque)
    : InstanceData(BATCH_NAME, isOpaque) {}

uint ShapeInstance::GetTextureIndex() const { return data_.TextureIndex; }

void ShapeInstance::SetTextureIndex(const uint index) {
  if (data_.TextureIndex == index) {
    return;
  }
  data_.TextureIndex = index;
  SetDirty();
}

glm::vec2 ShapeInstance::GetSize() const { return data_.Size; }

void ShapeInstance::SetSize(const glm::vec2& size) {
  if (data_.Size == size) {
    return;
  }
  data_.Size = size;
  SetDirty();
}

glm::vec4 ShapeInstance::GetColor() const { return data_.Color; }

void ShapeInstance::SetColor(const glm::vec4& color) {
  if (data_.Color == color) {
    return;
  }
  data_.Color = color;
  SetDirty();
}

void ShapeInstance::UpdateTransform(const glm::mat4& matrix) {
  InstanceData::UpdateTransform(matrix);
  data_.Matrix = matrix;
}

void ShapeInstance::WriteData(soil::video::buffer::Cursor* cursor) const {
  cursor->Write(data_.Matrix);
  cursor->Write(data_.Size);
  cursor->Write(data_.Color);
  cursor->Write(data_.TextureIndex);
}
}  // namespace soil_samples::instancing
