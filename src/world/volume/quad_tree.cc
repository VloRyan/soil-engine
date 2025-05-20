#include "world/volume/quad_tree.h"

#include <stdexcept>

#include "stage/scene/component/bounding_volume.h"

namespace soil::world::volume {
    QuadTree::QuadTree(const float size, const int maxLevel, const float minChildSize) :
        maxLevel_(maxLevel), minChildSize_(minChildSize), nodeHeight_(std::numeric_limits<float>::max()) {
        const auto radius = size * 0.5F;
        nodes_.emplace_back(Node{.Level = 0,
                                 .Min = glm::vec3(-radius, -nodeHeight_, -radius),
                                 .Max = glm::vec3(radius, nodeHeight_, radius)});
    }

    void QuadTree::Insert(const Volume *volume) {
        insert(volume, 0);
    }

    bool QuadTree::insert(const Volume *volume, const int index) {
        if (!volume->IsInsideXZ(nodes_[index].Min, nodes_[index].Max)) {
            return false;
        }
        auto firstChildIndex = nodes_[index].FirstChildIndex;
        if (firstChildIndex == -1) {
            if (nodes_[index].Contained.size() <= 3) {
                nodes_[index].Contained.push_back(volume);
                volumeRegister_.insert(std::pair{volume, index});
                return true;
            }
            const float childSize = glm::length(nodes_[index].Max.x - nodes_[index].Min.x) / 2;
            if (nodes_[index].Level < maxLevel_ && childSize >= minChildSize_) {
                split(index);
                firstChildIndex = nodes_[index].FirstChildIndex;
            } else {
                nodes_[index].Contained.push_back(volume);
                volumeRegister_.insert(std::pair{volume, index});
                return true;
            }
        }
        for (auto i = 0; i < 4; ++i) {
            const auto childIdx = firstChildIndex + i;
            if (insert(volume, childIdx)) {
                return true;
            }
        }
        if (index == 0) {
            //  throw std::runtime_error("[QuadTree::Insert]: Node index out of range");
        }
        nodes_[index].Contained.push_back(volume);
        volumeRegister_.insert(std::pair{volume, index});
        return true;
    }

    bool QuadTree::Remove(const Volume *volume) {
        const auto registerItr = volumeRegister_.find(volume);
        if (registerItr == volumeRegister_.end()) {
            return false;
        }
        const auto index = registerItr->second;
        for (auto itr = nodes_.at(index).Contained.begin(); itr != nodes_.at(index).Contained.end(); ++itr) {
            if (*itr == volume) {
                nodes_.at(index).Contained.erase(itr);
                volumeRegister_.erase(registerItr);
                return true;
            }
        }
        return false;
    }

    bool QuadTree::remove(const Volume *volume, const int index) {
        if (!volume->IsInsideXZ(nodes_[index].Min, nodes_[index].Max)) {
            return false;
        }
        if (const auto firstChildIndex = nodes_[index].FirstChildIndex; firstChildIndex != -1) {
            for (auto i = 0; i < 4; ++i) {
                if (const auto childIdx = firstChildIndex + i; remove(volume, childIdx)) {
                    return true;
                }
            }
        }
        for (auto itr = nodes_[index].Contained.begin(); itr != nodes_[index].Contained.end(); ++itr) {
            if (*itr == volume) {
                nodes_[index].Contained.erase(itr);
                return true;
            }
        }
        return false;
    }

    const QuadTree::Node *QuadTree::FindNodeAt(const glm::vec3 point) {
        return findNodeAt(point, 0);
    }

    const Container::Node *QuadTree::FindNodeFor(const Volume *volume) {
        const auto index = FindNodeIndexFor(volume);
        if (index == -1) {
            return nullptr;
        }
        return &nodes_[index];
    }

    int QuadTree::FindNodeIndexFor(const Volume *volume) {
        const auto registerItr = volumeRegister_.find(volume);
        if (registerItr == volumeRegister_.end()) {
            return -1;
        }
        return registerItr->second;
    }

    std::vector<const QuadTree::Node *> QuadTree::FindNodesAt(const glm::vec3 point) {
        std::vector<const Node *> nodes;
        auto *node = FindNodeAt(point);
        if (node == nullptr) {
            return nodes;
        }
        nodes.push_back(node);
        const auto *parent = GetNode(node->ParentIndex);
        while (parent != nullptr) {
            nodes.push_back(parent);
            parent = GetNode(parent->ParentIndex);
        }
        return nodes;
    }

    QuadTree::Node *QuadTree::findNodeAt(const glm::vec3 point, const int index) {
        if (!nodes_[index].ContainsXZ(point)) {
            return nullptr;
        }
        const auto firstChildIndex = nodes_[index].FirstChildIndex;

        if (firstChildIndex == -1) {
            return &nodes_[index];
        }
        for (auto i = 0; i < 4; ++i) {
            const auto childIdx = firstChildIndex + i;
            if (auto *found = findNodeAt(point, childIdx); found != nullptr) {
                return found;
            }
        }
        return &nodes_[index];
    }

    const QuadTree::Node *QuadTree::GetNode(const int index) const {
        if (index < 0 || index >= nodes_.size()) {
            return nullptr;
        }
        return &nodes_[index];
    }

    size_t QuadTree::GetNodeCount() const {
        return nodes_.size();
    }

    size_t QuadTree::GetChildrenPerNode() const {
        return 4;
    }

    void QuadTree::split(const int index) {
        const auto firstChildIndex = static_cast<int>(nodes_.size());
        nodes_[index].FirstChildIndex = firstChildIndex;
        nodes_.reserve(nodes_.size() + 4);

        const float childWidth = (nodes_[index].Max.x - nodes_[index].Min.x) / 2.0F;
        const float childRadius = childWidth / 2.0F;
        const auto centerParent = nodes_[index].Max - glm::vec3(childWidth, 0.F, childWidth);
        for (auto childIdx = 0; childIdx < 4; childIdx++) {
            auto centerChild = centerParent;
            centerChild.x += (childIdx & 1) != 0 ? childRadius : -childRadius;
            centerChild.z += (childIdx & 2) != 0 ? childRadius : -childRadius;
            nodes_.emplace_back(
                Node{.ParentIndex = index,
                     .Level = nodes_[index].Level + 1,
                     .Min = glm::vec3(centerChild.x - childRadius, -nodeHeight_, centerChild.z - childRadius),
                     .Max = glm::vec3(centerChild.x + childRadius, nodeHeight_, centerChild.z + childRadius)});
        }

        auto newVolumes = std::vector<const Volume *>();

        for (const auto *volume : nodes_[index].Contained) {
            bool moved = false;
            for (auto i = 0; i < 4; i++) {
                const auto childIdx = firstChildIndex + i;
                if (insert(volume, childIdx)) {
                    volumeRegister_.at(volume) = childIdx;
                    moved = true;
                    break;
                }
            }
            if (!moved) {
                newVolumes.push_back(volume);
            }
        }
        nodes_[index].Contained = std::move(newVolumes);
    }
} // namespace soil::world::volume
