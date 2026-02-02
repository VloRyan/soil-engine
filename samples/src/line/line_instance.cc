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

LineInstance::LineInstance(const std::string& pileName,
                           const glm::vec3 StartPoint, const glm::vec3 EndPoint)
    : MeshInstanceComponent(pileName),
      data_({
          .Start = StartPoint,
          .End = EndPoint,
          .Color = glm::vec4(1.0F),
      }),
      localStartPoint_(StartPoint),
      localEndPoint_(EndPoint) {}

void LineInstance::Write(const soil::video::render::data::IWriter& writer) {
  writer.Write("aStart", data_.Start);
  writer.Write("aEnd", data_.End);
  writer.Write("aColor", data_.Color);
}

void LineInstance::Update() {
  data_.Start = localStartPoint_ + GetParent()->GetPosition();
  data_.End = localEndPoint_ + GetParent()->GetPosition();
  MeshInstanceComponent::Update();
  SignalChanged(
      soil::stage::scene::component::DrawableComponent::ChangeDetails::Data);
}

glm::vec4 LineInstance::GetColor() const { return data_.Color; }

void LineInstance::SetColor(const glm::vec4 color) {
  data_.Color = color;
  SignalChanged(
      soil::stage::scene::component::DrawableComponent::ChangeDetails::Data);
}

float LineInstance::GetLength() const {
  return glm::length(localEndPoint_ - localStartPoint_);
}

void LineInstance::SetStartPoint(const glm::vec3 StartPoint) {
  if (localStartPoint_ == StartPoint) {
    return;
  }
  localStartPoint_ = StartPoint;
  SignalChanged(
      soil::stage::scene::component::DrawableComponent::ChangeDetails::Data);
}

void LineInstance::SetEndPoint(const glm::vec3 EndPoint) {
  if (localEndPoint_ == EndPoint) {
    return;
  }
  localEndPoint_ = EndPoint;
  SignalChanged(
      soil::stage::scene::component::DrawableComponent::ChangeDetails::Data);
}

glm::vec3 LineInstance::GetEndPoint() const { return localEndPoint_; }

}  // namespace soil_samples::line
