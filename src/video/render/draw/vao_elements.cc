#include "video/render/draw/vao_elements.h"

#include "video/shader/program.h"
namespace soil::video::render::draw {
VaoElements::VaoElements(const vertex::Vao* vao, video::shader::Program* shader,
                         DrawMode drawMode, Data& data, StateDef state)
    : stateId_({
          .Shader = shader,
          .Vao = vao,
          .State = state,
      }),
      drawMode_(drawMode),
      data_(data) {}

const StateIdentifier& VaoElements::StateId() const { return stateId_; }

void VaoElements::Draw(State& state) {
  state.Apply(stateId_.State);
  state.SetShader(stateId_.Shader);
  stateId_.Shader->Prepare(state);
  state.BindVao(stateId_.Vao);

  data_.BeforeDrawElements(state);
  auto* ebo = stateId_.Vao->GetEbo();
  shader::Program::DrawElements(static_cast<uint>(drawMode_),
                                ebo->GetIndexCount(), ebo->GetIndexType());
}

float VaoElements::DistanceTo(const glm::vec3& point) {
  return data_.DistanceTo(point);
}

bool VaoElements::IsSortable() { return true; }

void VaoElements::UpdateState(const StateDef& newState) {
  if (this->stateId_.State == newState) {
    return;
  }
  auto prevState = stateId_.State;
  this->stateId_.State = newState;
  fire(event::DrawableEvent::MakeStateChangedEvent(this, prevState));
}

}  // namespace soil::video::render::draw
