#ifndef SOIL_STAGE_SCENE_COMPONENT_INSTANCE_DATA_H
#define SOIL_STAGE_SCENE_COMPONENT_INSTANCE_DATA_H
#include "video/render/instance/instance.h"
#include "visual_component.h"

namespace soil::stage::scene::component {
    class InstanceData : public VisualComponent, public video::render::instance::Instance {
    public:
        InstanceData(std::string batchKey, bool opaque);
        ~InstanceData() override = default;
        std::string GetBatchKey();
        [[nodiscard]] video::render::RenderableObject* GetRenderable() override;

    private:
        std::string batchKey_;
    };
} // namespace soil::stage::scene::component


#endif
