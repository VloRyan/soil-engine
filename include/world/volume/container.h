#ifndef SOIL_WORLD_VOLUME_CONTAINER_H
#define SOIL_WORLD_VOLUME_CONTAINER_H
#include <vector>


#include "stage/scene/component/bounding_volume.h"
#include "volume.hpp"

namespace soil::world::volume {
    class Container {
    public:
        struct Node {
            [[nodiscard]] bool Contains(const glm::vec3 &point) const {
                for (auto i = 0; i < 3; ++i) {
                    if (point[i] < Min[i] || point[i] > Max[i]) {
                        return false;
                    }
                }
                return true;
            }

            [[nodiscard]] bool ContainsXZ(const glm::vec3 &point) const {
                if (point.x < Min.x || point.x > Max.x) {
                    return false;
                }
                if (point.z < Min.z || point.z > Max.z) {
                    return false;
                }
                return true;
            }

            std::int32_t ParentIndex{-1};
            std::int32_t FirstChildIndex{-1};
            std::int32_t Level{-1};
            std::vector<const Volume *> Contained{};
            glm::vec3 Min{0};
            glm::vec3 Max{0};
        };

        virtual ~Container() = default;

        virtual void Insert(const Volume *volume) = 0;
        virtual bool Remove(const Volume *volume) = 0;

        virtual const Node *FindNodeAt(glm::vec3 point) = 0;
        virtual const Node *FindNodeFor(const Volume *volume) = 0;
        virtual int FindNodeIndexFor(const Volume *volume) = 0;

        virtual std::vector<const Node *> FindNodesAt(glm::vec3 point) = 0;

        [[nodiscard]] virtual const Node *GetNode(int index) const = 0;

        [[nodiscard]] virtual size_t GetNodeCount() const = 0;
        [[nodiscard]] virtual size_t GetChildrenPerNode() const = 0;

    protected:
        Container() = default;
    };
} // namespace soil::world::volume
#endif // SOIL_WORLD_VOLUME_CONTAINER_H
