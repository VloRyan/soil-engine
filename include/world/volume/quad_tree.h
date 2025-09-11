#ifndef QUAD_TREE_H
#define QUAD_TREE_H

#include "base.h"
#include "container.h"
#include "stage/scene/component/bounding_volume.h"

namespace soil::world::volume {


    class QuadTree : public Container {
    public:
        explicit QuadTree(float size, byte maxLevel = 4, float minChildSize = 6.0F);

        void Insert(const Volume* volume) override;
        void Insert2(const Volume* volume);
        bool Remove(const Volume* volume) override;

        void QueryVolumesAt(const glm::vec3& point, std::vector<const Volume*>& volumes) const override;
        void QueryVolumesAt(const glm::vec2& point, std::vector<const Volume*>& volumes) const;

        void QueryNodeIndicesFor(const Volume*, std::vector<int>& indices) const override;

        const Node* GetNode(int index) const override;
        void GetVolumes(int index, std::vector<const Volume*>& volumes) const override;
        size_t GetVolumeCount() const;

        size_t GetNodeCount() const override;
        size_t GetChildrenPerNode() const override;

        byte DetermineLevel(std::uint16_t index) const;

    private:
        bool insert(const Volume* volume, int index, byte level);

        void split(std::uint16_t index);
        void split2(std::uint16_t index, byte level);

        std::uint16_t freeNodeVolumesIndex();

        std::vector<Node> nodes_;
        std::vector<std::vector<const Volume*>> nodeVolumes_;
        std::vector<std::uint16_t> freeNodeVolumes_;

        byte maxLevel_;
        float minChildSize_;
    };
} // namespace soil::world::volume

#endif
