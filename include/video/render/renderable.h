#ifndef SOIL_VIDEO_RENDER_RENDERABLE_H
#define SOIL_VIDEO_RENDER_RENDERABLE_H
#include "state.h"
#include "state_identifier.hpp"
#include "video/render/data/writer.hpp"
#include "video/render/types.hpp"
#include "video/shader/shader.h"
#include "video/vertex/vao.h"
namespace soil::video::render {
class RenderableGroup;

class NoopWriter : public data::IWriter {
 public:
  void Write(const std::string& name, const glm::mat4& matrix) const override;
  void Write(const std::string& name, const glm::vec2& v) const override;
  void Write(const std::string& name, const glm::vec4& v) const override;
  void Write(const std::string& name, uint v) const override;
  void Write(const std::string& name, const glm::vec3& v) const override;
  void Write(const std::string& name, int v) const override;
  void Write(const std::string& name, float v) const override;
  void Write(const std::string& name, bool v) const override;
};

class Renderable {
 public:
  explicit Renderable(const StateIdentifier& stateId);

  virtual ~Renderable() = default;

  [[nodiscard]] virtual int GetContainerStateIndex() const;

  virtual void SetContainerStateIndex(int index);

  [[nodiscard]] const StateIdentifier& StateId();
  [[nodiscard]] const StateIdentifier& StateId() const;
  void SetStateId(const StateIdentifier& stateId);

  virtual void Render(soil::video::render::State& state, int count) = 0;

  [[nodiscard]] virtual const vertex::Vao* Vao() const = 0;
  [[nodiscard]] virtual enum DrawMode DrawMode() const = 0;
  [[nodiscard]] virtual RenderableGroup* NewGroup() const = 0;
  virtual void ApplyData(const data::IWriter& writer,
                         soil::video::render::State& state) const = 0;

 protected:
  StateIdentifier stateId_;

 private:
  int containerStateIndex_;
};
}  // namespace soil::video::render
#endif
