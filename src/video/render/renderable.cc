#include "video/render/renderable.h"

namespace soil::video::render {
Renderable::Renderable(const StateIdentifier& stateId)
    : containerStateIndex_(-1), stateId_(stateId) {}

int Renderable::GetContainerStateIndex() const { return containerStateIndex_; }

void Renderable::SetContainerStateIndex(const int index) {
  containerStateIndex_ = index;
}

const StateIdentifier& Renderable::StateId() { return stateId_; }
const StateIdentifier& Renderable::StateId() const { return stateId_; }

void Renderable::SetStateId(const StateIdentifier& stateId) {
  stateId_ = stateId;
}

void NoopWriter::Write(const std::string& name, const glm::mat4& matrix) const {

}
void NoopWriter::Write(const std::string& name, const glm::vec2& v) const {}
void NoopWriter::Write(const std::string& name, const glm::vec4& v) const {}
void NoopWriter::Write(const std::string& name, const uint v) const {}
void NoopWriter::Write(const std::string& name, const glm::vec3& v) const {}
void NoopWriter::Write(const std::string& name, int v) const {}
void NoopWriter::Write(const std::string& name, float v) const {}
void NoopWriter::Write(const std::string& name, bool v) const {}
}  // namespace soil::video::render
