#ifndef SOIL_STAGE_SCENE_RENDER_INSTANCING_H
#define SOIL_STAGE_SCENE_RENDER_INSTANCING_H
#include <string>
#include <unordered_map>

#include "stage/scene/component/instance_data.h"
#include "stage/scene/hook/render_hook.h"
#include "video/render/batch.h"
#include "video/render/vertex_array.h"

namespace soil::stage::scene::render {
class Instancing : public hook::RenderHook {
 public:
  class BatchObject : public video::render::RenderableObject,
                      public video::render::Batch {
   public:
    explicit BatchObject(const video::render::BatchDescriptor& descriptor);

    ~BatchObject() override = default;

    float DistanceTo(const glm::vec3& point) override;

    void Render(video::render::State& state) override;

    bool IsSortable() const override;
  };

  struct BatchCreationArgs {
    video::mesh::Data* Mesh{nullptr};
    video::shader::Shader* Shader{nullptr};
    std::vector<video::vertex::VertexAttribDescriptor> VertexAttribDescriptors;
    int PreserveInstances{1000};
    video::render::StateDef State{};
  };

  struct StateBatches {
    BatchObject* Opaque{nullptr};
    BatchObject* NonOpaque{nullptr};
  };

  explicit Instancing(video::render::Container* renderContainer);

  ~Instancing() override;

  void AddRenderBatch(const std::string& key, const BatchCreationArgs& args);

  void OnRender(video::render::State& state) override;

  void Handle(const event::Component& event) override;

 protected:
  BatchObject* createBatch(const std::string& key);

  void OnAdded(component::InstanceData* data);

  void OnStateChanged(component::InstanceData* data);

  void OnRemoved(component::InstanceData* data);

 private:
  video::render::Container* renderContainer_;
  std::unordered_map<std::string, StateBatches> renderBatchPerKey_;
  std::vector<video::render::VertexArray*> vertexArrays_;
  std::unordered_map<std::string, BatchCreationArgs> batchCreationArgs_;
  std::vector<BatchObject*> addedOpaqueBatches_;
  std::vector<BatchObject*> addedNonOpaqueBatches_;
  std::vector<BatchObject*> removedOpaqueBatches_;
  std::vector<BatchObject*> removedNonOpaqueBatches_;
};
}  // namespace soil::stage::scene::render
#endif
