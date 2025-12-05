#ifndef SOIL_VIDEO_RENDER_DRAW_VAO_ELEMENTS_INSTANCED_H
#define SOIL_VIDEO_RENDER_DRAW_VAO_ELEMENTS_INSTANCED_H
#include "drawable.hpp"
#include "util/cache.hpp"
#include "video/render/data/buffer_writer.h"

namespace soil::video::render::draw {
class InstanceDataBuffer;
class VaoElementsInstanced : public Drawable {
 public:
  static constexpr auto UnsetId = -1;
  struct PileDescriptor {
    std::string Name;
    video::mesh::Data* MeshData{nullptr};
    shader::Shader* Shader{nullptr};
    std::vector<vertex::VertexAttribDescriptor> VertexAttribDescriptors{};
    int PreserveInstances{100};
  };
  class Data {
   public:
    Data() = default;
    virtual ~Data() = default;
    virtual void Write(const data::IWriter& writer) = 0;
    friend class InstanceDataBuffer;

   private:
    size_t index_{static_cast<size_t>(UnsetId)};
    bool dirty_{false};
  };

  ~VaoElementsInstanced() override = default;
  [[nodiscard]] const StateIdentifier& StateId() const override;
  void Bind(State& state) override;
  void Draw() override;
  float DistanceTo(const glm::vec3& point) override;
  bool IsSortable() override;

  void Insert(Data* instance);
  bool Remove(Data* instance);
  void SetDirty(Data* instance);

  static VaoElementsInstanced* Prepare(const PileDescriptor& pileDescriptor,
                                       StateDef state = {});
  static VaoElementsInstanced* GetPile(const std::string& name);

 private:
  explicit VaoElementsInstanced(const PileDescriptor& pileDescriptor,
                                StateDef state = {});
  static GLsizei InstanceSize(
      const std::vector<vertex::VertexAttribDescriptor>& attribs);
  void Update();

  StateIdentifier stateId_;
  DrawMode drawMode_;
  InstanceDataBuffer* instanceBuffer_;
  static util::Cache<VaoElementsInstanced> CACHE;
};
}  // namespace soil::video::render::draw
#endif
