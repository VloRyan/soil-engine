
#ifndef SOIL_STAGE_SCENE_GRID_SCENE_H
#define SOIL_STAGE_SCENE_GRID_SCENE_H
#include "stage/scene/scene.h"


namespace soil::stage::scene {
    class GridScene : public Scene {
    public:
        explicit GridScene(Stage *stage, float size, int maxLevel = 512, float minChildSize = 6.0F);

        ~GridScene() override = default;

        void ForNodesAt(glm::vec2 pos, const std::function<void(const component::BoundingVolume *volume)> &f) const;

        [[nodiscard]] space::QuadTree *GetQuadTree() const;

    protected:
        void ComponentStateChanged(component::Component *component) override;

        void ComponentAdded(component::Component *component) override;

        void ComponentRemoved(component::Component *component) override;

        void NodeAdded(Node *node) override;

        void NodeRemoved(const Node *node) override;

    private:
        space::QuadTree *quadTree_;
    };
} // namespace soil::stage::scene


#endif // SOIL_STAGE_SCENE_GRID_SCENE_H
