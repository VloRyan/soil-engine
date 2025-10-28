#ifndef SOIL_STAGE_SCENE_RENDER_INSTANCING_H
#define SOIL_STAGE_SCENE_RENDER_INSTANCING_H
#include <string>
#include <unordered_map>

#include "stage/scene/component/instance_data.h"
#include "stage/scene/hook/hook.h"
#include "video/render/batch.h"
#include "video/render/container.h"
#include "video/render/vertex_array.h"

namespace soil::stage::scene::render {
class Instancing : public hook::Hook {
 public:
  class BatchObject : public video::render::RenderableObject {
   public:
    explicit BatchObject(video::render::Batch* batch);

    ~BatchObject() override = default;

    float DistanceTo(const glm::vec3& point) override;

    virtual void AddChangedInstance(
        video::render::instance::Instance* instance);

    virtual void AddNewInstance(video::render::instance::Instance* instance);

    virtual bool RemoveInstance(video::render::instance::Instance* instance);

    virtual void Update();

    void Render(video::render::State& state) override;

    [[nodiscard]] bool IsSortable() const override;

   protected:
    BatchObject();  // unit test
   private:
    video::render::Batch* batch_;
  };

  struct BatchObjectCreationArgs {
    video::mesh::Data* Mesh{nullptr};
    video::shader::Shader* Shader{nullptr};
    std::vector<video::vertex::VertexAttribDescriptor> VertexAttribDescriptors;
    int PreserveInstances{1000};
    video::render::StateDef State{};
  };

  static BatchObject* DefaultCreateBatchObject(
      const BatchObjectCreationArgs& args);

  struct StateBatches {
    BatchObject* Opaque{nullptr};
    BatchObject* NonOpaque{nullptr};
  };

  explicit Instancing(
      video::render::Container* renderContainer,
      std::function<BatchObject*(const BatchObjectCreationArgs& args)>
          creatorFunc = DefaultCreateBatchObject);

  ~Instancing() override;

  void AddRenderBatch(const std::string& key,
                      const BatchObjectCreationArgs& args);

  virtual void Perform(hook::Hook::Trigger_t trigger) override;

  void Handle(const event::Component& event) override;

  StateBatches* GetStateBatches(const std::string& name);

 protected:
  BatchObject* createBatch(const std::string& key);

  void OnAdded(component::InstanceData* data);

  void OnChanged(component::InstanceData* data);

  void OnRemoved(component::InstanceData* data);

 private:
  std::function<BatchObject*(const BatchObjectCreationArgs& args)>
      batchObjectCreatorFunc_;
  video::render::Container* renderContainer_;
  std::unordered_map<std::string, StateBatches> renderBatchPerKey_;
  std::vector<component::InstanceData*> addedData_;
  std::unordered_map<std::string, BatchObjectCreationArgs> batchCreationArgs_;
  std::vector<BatchObject*> addedOpaqueBatches_;
  std::vector<BatchObject*> addedNonOpaqueBatches_;
  std::vector<BatchObject*> removedOpaqueBatches_;
  std::vector<BatchObject*> removedNonOpaqueBatches_;
};
}  // namespace soil::stage::scene::render
#endif
