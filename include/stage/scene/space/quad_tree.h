#ifndef ENGINE_STAGE_SCENE_QUAD_TREE_H_
#define ENGINE_STAGE_SCENE_QUAD_TREE_H_

#include <functional>

#include "base.h"
#include "stage/scene/component/bounding_volume.h"
#include "world/collision/frustum.h"

namespace stage::scene::space {
    class QuadTree {
    public:
        struct Node {
            bool Contains(const glm::vec2 &point) {
                for (auto i = 0; i < 2; ++i) {
                    if (point[i] < Min[i] || point[i] > Max[i]) {
                        return false;
                    }
                }
                return true;
            }

            int32_t ParentIndex;
            int32_t FirstChildIndex;
            int32_t Level;
            std::vector<const component::BoundingVolume *> Contained;
            glm::vec2 Min;
            glm::vec2 Max;

            explicit Node(const int32_t parentIndex, const int32_t level, const glm::vec2 &center,
                          const float radius): ParentIndex(parentIndex),
                                               FirstChildIndex(-1), Level(level), Min(glm::vec2(center - radius)),
                                               Max(glm::vec2(center + radius)) {
            }
        };

        explicit QuadTree(float size, int maxLevel = 512, float minChildSize = 6.0F,
                          IDebugDrawer *debugDrawer = nullptr);

        void Insert(const component::BoundingVolume *volume);

        bool Remove(const component::BoundingVolume *volume);

        const Node* FindNodeAt(glm::vec2 point);

        std::vector<const Node *> FindNodesAt(glm::vec2 point);

        void SetDebugDrawer(IDebugDrawer *debugDrawer);

        const Node* GetNode(int index) const;

        size_t GetNodeCount() const;

    private:
        bool insert(const component::BoundingVolume *volume, int index);

        bool remove(const component::BoundingVolume *volume, int index);

        void split(int index);

        Node* findNodeAt(glm::vec2 point, int index);

        void drawDebugLines(int index, glm::vec2 centerParent, float childWidth) const;

        std::vector<Node> nodes_;
        HashMap<const component::BoundingVolume *, int> volumeRegister_;

        int32_t maxLevel_;
        float minChildSize_;
        IDebugDrawer *debugDrawer_;
    };
} // stage::scene

#endif //ENGINE_STAGE_SCENE_QUAD_TREE_H_

