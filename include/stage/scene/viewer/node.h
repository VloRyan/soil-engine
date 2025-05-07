#ifndef SOIL_STAGE_SCENE_VIEWER_NODE_H
#define SOIL_STAGE_SCENE_VIEWER_NODE_H

#include "glm/glm.hpp"
#include "stage/scene/component/renderable_instance.h"
#include "stage/scene/node.h"
#include "stage/scene/space/quad_tree.h"
#include "video/render/batch.h"
#include "video/render/pipeline.h"
#include "video/viewer.h"
#include "world/collision/frustum.h"

namespace soil::stage::scene::viewer {
    class Node : public scene::Node, public video::Viewer {
    public:
        ~Node() override = default;

        [[nodiscard]] glm::mat4 GetViewMatrix() const override = 0;

        [[nodiscard]] glm::mat4 GetProjectionMatrix() const override = 0;

        virtual world::collision::Frustum *GetFrustum() = 0;

        [[nodiscard]] video::render::Pipeline *GetPipeline() const;


        [[nodiscard]] bool IsViewChanged() const;

        void AddRenderable(component::RenderableInstance *renderable);

        void AddChangedRenderable(component::RenderableInstance *renderable);

        void RemoveRenderable(component::RenderableInstance *renderable);

        void SetQuadTree(space::QuadTree *quadTree);

        void Update() override;

        void Render(video::render::State &state) override;

        [[nodiscard]] video::render::StateDef GetStateDef() const;

        void SetStateDef(const video::render::StateDef &stateDef);

        [[nodiscard]] int GetUboMatricesBindingTarget() const;

        void SetUboMatricesBindingTarget(int uboMatricesBindingTarget);

    protected:
        explicit Node(scene::Node *parent);

        Node(scene::Node *parent, const std::vector<scene::Node::ReceiverType> &receiverTypes);

        void updateVisibilityOnTreeNode(int index);

        video::render::Pipeline *pipeline_;
        bool viewChanged_;
        space::QuadTree *quadTree_;
        HashMap<std::string, video::render::Batch *> batchMap_;
        std::vector<video::render::Renderable *> renderables_;
        std::vector<bool> nodesVisibility_;
        video::render::StateDef stateDef_;
        int uboMatricesBindingTarget_;
    };
} // namespace soil::stage::scene::viewer
#endif // SOIL_STAGE_SCENE_VIEWER_NODE_H
