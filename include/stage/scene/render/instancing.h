#ifndef SOIL_STAGE_SCENE_RENDER_INSTANCING_H
#define SOIL_STAGE_SCENE_RENDER_INSTANCING_H
#include <string>
#include <unordered_map>

#include "feature.h"
#include "video/render/batch.h"
#include "video/render/vertex_array.h"

namespace soil::stage::scene::render {
    class Instancing : public Feature {
    public:
        struct BatchCreationArgs {
            video::mesh::Data *Mesh{nullptr};
            video::shader::Shader *Shader{nullptr};
            std::vector<video::vertex::VertexAttribDescriptor> VertexAttribDescriptors;
            int PreserveInstances{1000};
        };

        struct StateBatches {
            video::render::Batch *Opaque{nullptr};
            video::render::Batch *NonOpaque{nullptr};
        };

        Instancing() = default;
        ~Instancing() override;
        void ComponentAdded(component::Component *component) override;
        void ComponentChanged(component::Component *component) override;
        void ComponentRemoved(component::Component *component) override;
        void Update() override;
        void AddRenderBatch(const std::string &key, const BatchCreationArgs &args);

    protected:
        video::render::Batch *createBatch(const std::string &key);

    private:
        std::unordered_map<std::string, StateBatches> renderBatchPerKey_;
        std::vector<video::render::VertexArray *> vertexArrays_;
        std::unordered_map<std::string, BatchCreationArgs> batchCreationArgs_;
        std::vector<video::render::Batch *> addedOpaqueBatches_;
        std::vector<video::render::Batch *> addedNonOpaqueBatches_;
        std::vector<video::render::Batch *> removedOpaqueBatches_;
        std::vector<video::render::Batch *> removedNonOpaqueBatches_;
    };
} // namespace soil::stage::scene::render
#endif // SOIL_STAGE_SCENE_RENDER_INSTANCING_H
