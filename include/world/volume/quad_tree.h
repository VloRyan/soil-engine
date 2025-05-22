#ifndef SOIL_STAGE_SCENE_VOLUME_QUAD_TREE_H
#define SOIL_STAGE_SCENE_VOLUME_QUAD_TREE_H

#include "base.h"
#include "container.h"
#include "stage/scene/component/bounding_volume.h"

namespace soil::world::volume {
    class QuadTree : public Container {
    public:
        explicit QuadTree(float size, int maxLevel = 512, float minChildSize = 6.0F);

        void Insert(const Volume *volume) override;

        bool Remove(const Volume *volume) override;

        const Node *FindNodeAt(glm::vec3 point) override;
        const Node *FindNodeFor(const Volume *volume) override;
        int FindNodeIndexFor(const Volume *volume) override;

        std::vector<const Node *> FindNodesAt(glm::vec3 point) override;

        const Node *GetNode(int index) const override;

        size_t GetNodeCount() const override;
        size_t GetChildrenPerNode() const override;

    private:
        bool insert(const Volume *volume, int index);

        bool remove(const Volume *volume, int index);

        void split(int index);

        Node *findNodeAt(glm::vec3 point, int index);

        std::vector<Node> nodes_;
        HashMap<const Volume *, int> volumeRegister_;

        int32_t maxLevel_;
        float minChildSize_;
        float nodeHeight_;
    };
} // namespace soil::world::volume

#endif // SOIL_STAGE_SCENE_VOLUME_QUAD_TREE_H
