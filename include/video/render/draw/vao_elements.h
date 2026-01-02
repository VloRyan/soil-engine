#ifndef SOIL_VIDEO_RENDER_DRAW_VAO_ELEMENTS_H
#define SOIL_VIDEO_RENDER_DRAW_VAO_ELEMENTS_H
#include "drawable.hpp"
namespace soil::video::render::draw {
class VaoElements : public Drawable {
 public:
  class Data {
   public:
    Data() = default;
    virtual ~Data() = default;
    virtual float DistanceTo(const glm::vec3& point) = 0;
    virtual void BeforeDrawElements(State& state) {};
  };
  explicit VaoElements(const vertex::Vao* vao, video::shader::Program* shader,
                       DrawMode drawMode, Data& data, StateDef state = {});
  ~VaoElements() override = default;
  [[nodiscard]] const StateIdentifier& StateId() const override;
  void Draw(State& state) override;
  float DistanceTo(const glm::vec3& point) override;
  bool IsSortable() override;

  void UpdateState(const StateDef& newState);

 private:
  StateIdentifier stateId_;
  Data& data_;
  DrawMode drawMode_;
};
}  // namespace soil::video::render::draw
#endif
