#ifndef SOIL_WORLD_VOLUME_CONTAINER_H
#define SOIL_WORLD_VOLUME_CONTAINER_H
#include <vector>
#include "volume.hpp"

namespace soil::world::volume {
    class Container {
    public:
        inline static auto NO_CHILDREN = -1;

        struct Node {
            [[nodiscard]] bool Contains(const glm::vec2& point) const {
                if (point.x < Min.x || //
                    point.x > Max.x || //
                    point.y < Min.y || //
                    point.y > Max.y) {
                    return false;
                }
                return true;
            }

            [[nodiscard]] bool Contains(const glm::vec2& min, const glm::vec2& max) const {
                if (Min.x > min.x && Max.x < max.x || //
                    Min.y > min.y && Max.y < max.y) {
                    return false;
                }

                return true;
            }

            inline static uint UNSET = -1;
            uint ChildrenStartIndex{UNSET};
            uint VolumesIndex{UNSET};
            glm::vec2 Min{0.F};
            glm::vec2 Max{0.F};
        };

        virtual ~Container() = default;

        virtual void Insert(const Volume* volume) = 0;
        virtual bool Remove(const Volume* volume) = 0;

        virtual void QueryVolumesAt(const glm::vec3& point, std::vector<const Volume*>& volumes) const = 0;
        virtual void QueryNodeIndicesFor(const Volume*, std::vector<int>& indices) const = 0;

        virtual void GetVolumes(int index, std::vector<const Volume*>& volumes) const = 0;


        [[nodiscard]] virtual const Node* GetNode(int index) const = 0;

        [[nodiscard]] virtual size_t GetNodeCount() const = 0;
        [[nodiscard]] virtual size_t GetChildrenPerNode() const = 0;

    protected:
        Container() = default;
    };
} // namespace soil::world::volume
#endif
