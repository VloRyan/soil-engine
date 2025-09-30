#include "line_instance.h"

#include "stage/scene/node.h"
#include "video/vertex/vertex.h"

namespace soil_samples::line {
std::vector<soil::video::vertex::VertexAttribDescriptor> LineInstance::ATTRIBS{
    // aStart
    {.Elements = 3,
     .Type = soil::video::vertex::AttributePointer::DataType::Float},
    // aEnd
    {.Elements = 3,
     .Type = soil::video::vertex::AttributePointer::DataType::Float},
    // aColor
    {.Elements = 4,
     .Type = soil::video::vertex::AttributePointer::DataType::Float},
};

LineInstance::LineInstance(const std::string& batchKey,
                           const glm::vec3 StartPoint, const glm::vec3 EndPoint)
    : InstanceData(batchKey, true),
      data_({
          .Start = StartPoint,
          .End = EndPoint,
          .Color = glm::vec4(1.0F),
      }),
      localStartPoint_(StartPoint),
      localEndPoint_(EndPoint) {}

void LineInstance::WriteData(soil::video::buffer::Cursor* cursor) const {
  cursor->Write(&data_, sizeof(Data));
}

void LineInstance::Update() {
  data_.Start = localStartPoint_ + GetParent()->GetWorldPosition();
  data_.End = localEndPoint_ + GetParent()->GetWorldPosition();
  InstanceData::Update();
}

glm::vec4 LineInstance::GetColor() const { return data_.Color; }

void LineInstance::SetColor(const glm::vec4 color) {
  data_.Color = color;
  SetDirty();
}

float LineInstance::GetLength() const {
  return glm::length(localEndPoint_ - localStartPoint_);
}

void LineInstance::SetStartPoint(const glm::vec3 StartPoint) {
  if (localStartPoint_ == StartPoint) {
    return;
  }
  localStartPoint_ = StartPoint;
  SetDirty();
}

void LineInstance::SetEndPoint(const glm::vec3 EndPoint) {
  if (localEndPoint_ == EndPoint) {
    return;
  }
  localEndPoint_ = EndPoint;
  SetDirty();
}

glm::vec3 LineInstance::GetEndPoint() const { return localEndPoint_; }
}  // namespace soil_samples::line
