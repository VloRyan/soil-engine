#include "stage/scene/space/quad_tree.h"

#include "stage/scene/component/bounding_volume.h"

namespace soil::stage::scene::space {
    QuadTree::QuadTree(const float size, const int maxLevel, const float minChildSize, IDebugDrawer *debugDrawer) :
        maxLevel_(maxLevel), minChildSize_(minChildSize), debugDrawer_(debugDrawer) {
        const auto radius = size * 0.5F;
        nodes_.emplace_back(-1, 0, glm::vec2(0.0F), radius);
    }

    void QuadTree::Insert(const component::BoundingVolume *volume) { insert(volume, 0); }

    bool QuadTree::insert(const component::BoundingVolume *volume, const int index) {
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
            if (const float childSize = glm::length(nodes_[index].Max.x - nodes_[index].Min.x) / 2;
                nodes_[index].Level + 1 <= maxLevel_ && childSize >= minChildSize_) {
                split(index);
                firstChildIndex = nodes_[index].FirstChildIndex;
            } else {
                nodes_[index].Contained.push_back(volume);
                volumeRegister_.insert(std::pair{volume, index});
                return true;
            }
        }
        if (firstChildIndex > -1) {
        }
        for (auto i = 0; i < 4; ++i) {
            if (const auto childIdx = firstChildIndex + i; insert(volume, childIdx)) {
                return true;
            }
        }
        nodes_[index].Contained.push_back(volume);
        volumeRegister_.insert(std::pair{volume, index});
        return true;
    }

    bool QuadTree::Remove(const component::BoundingVolume *volume) {
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

    bool QuadTree::remove(const component::BoundingVolume *volume, const int index) {
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

    const QuadTree::Node *QuadTree::FindNodeAt(const glm::vec2 point) { return findNodeAt(point, 0); }

    std::vector<const QuadTree::Node *> QuadTree::FindNodesAt(const glm::vec2 point) {
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

    QuadTree::Node *QuadTree::findNodeAt(const glm::vec2 point, const int index) {
        if (!nodes_[index].Contains(point)) {
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

    void QuadTree::SetDebugDrawer(IDebugDrawer *const debugDrawer) { debugDrawer_ = debugDrawer; }

    const QuadTree::Node *QuadTree::GetNode(const int index) const {
        if (index < 0 || index >= nodes_.size()) {
            return nullptr;
        }
        return &nodes_[index];
    }

    size_t QuadTree::GetNodeCount() const { return nodes_.size(); }

    void QuadTree::split(const int index) {
        const auto firstChildIndex = static_cast<int>(nodes_.size());
        nodes_[index].FirstChildIndex = firstChildIndex;
        nodes_.reserve(nodes_.size() + 4);

        const float childWidth = (nodes_[index].Max.x - nodes_[index].Min.x) / 2.0F;
        const float childRadius = childWidth / 2.0F;
        const glm::vec2 centerParent = nodes_[index].Max - glm::vec2(childWidth);
        for (auto childIdx = 0; childIdx < 4; childIdx++) {
            glm::vec2 centerChild = centerParent;
            centerChild[0] += (childIdx & 1) != 0 ? childRadius : -childRadius;
            centerChild[1] += (childIdx & 2) != 0 ? childRadius : -childRadius;
            nodes_.emplace_back(index, nodes_[index].Level + 1, centerChild, childRadius);
        }

        auto newVolumes = std::vector<const component::BoundingVolume *>();

        for (const auto *volume : nodes_[index].Contained) {
            bool moved = false;
            for (auto i = 0; i < 4; i++) {
                if (const auto childIdx = firstChildIndex + i; insert(volume, childIdx)) {
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

        if (debugDrawer_ != nullptr) {
            drawDebugLines(index, centerParent, childWidth);
        }
    }

    void QuadTree::drawDebugLines(const int index, glm::vec2 centerParent, float childWidth) const {
        glm::vec2 centerChild = centerParent;
        constexpr auto yOffset = 0.001F;
        auto color = glm::vec4(0.0F, 0.0F, 0.0F, 1.0F);
        switch (nodes_[index].Level) {
        case 0:
            color = glm::vec4(0.0F, 0.0F, 0.0F, 1.0F);
            break;
        case 1:
            color = glm::vec4(0.5F, 0.0F, 0.0F, 1.0F);
            break;
        case 2:
            color = glm::vec4(0.0F, 0.5F, 0.0F, 1.0F);
            break;
        case 3:
            color = glm::vec4(0.0F, 0.0F, 0.5F, 1.0F);
            break;
        case 4:
            color = glm::vec4(0.5F, 0.5F, 0.0F, 1.0F);
            break;
        case 5:
            color = glm::vec4(0.5F, 0.5F, 0.5F, 1.0F);
            break;
        case 6:
            color = glm::vec4(1.0F, 0.0F, 0.0F, 1.0F);
            break;
        default:;
        }
        debugDrawer_->Line(glm::vec3(centerChild.x, yOffset, centerChild.y - childWidth),
                           glm::vec3(centerChild.x, yOffset, centerChild.y + childWidth), color);
        debugDrawer_->Line(glm::vec3(centerChild.x - childWidth, yOffset, centerChild.y),
                           glm::vec3(centerChild.x + childWidth, yOffset, centerChild.y), color);
    }
} // namespace soil::stage::scene::space
