#include "world/volume/quad_tree.h"

#include <algorithm>
#include <cstring>
#include <queue>
#include <stack>

namespace soil::world::volume {
    QuadTree::QuadTree(const float size, const byte maxLevel, const float minChildSize) :
        maxLevel_(maxLevel), minChildSize_(minChildSize) {
        const auto radius = size * 0.5F;
        nodes_.emplace_back(Node{.Min = glm::vec2(-radius, -radius), .Max = glm::vec2(radius, radius)});
    }

    struct state {
        uint Index;
        byte Level;
    };

    void QuadTree::Insert(const Volume* volume) {
        if (!volume->IsInside(nodes_[0].Min, nodes_[0].Max)) {
            return;
        }
        constexpr auto defaultBufferSize = 8;
        auto bufferSize = defaultBufferSize;
        std::vector<state> nodeIndices;
        nodeIndices.resize(bufferSize);
        nodeIndices[0] = {.Index = 0, .Level = 0};
        auto nextFreeSlot = 1;
        auto ni = 0;
        for (; ni != nextFreeSlot; ++ni) {
            const auto state = nodeIndices[ni];
            const auto index = state.Index;
            const auto level = state.Level;
            auto volumeIndex = nodes_[index].VolumesIndex;
            auto childrenStartIndex = nodes_[index].ChildrenStartIndex;
            if (childrenStartIndex == Node::UNSET) {
                if (volumeIndex == Node::UNSET || nodeVolumes_[volumeIndex].size() <= 3) {
                    // nodes_[index].Contained.push_back(volume);
                    if (volumeIndex == Node::UNSET) {
                        volumeIndex = freeNodeVolumesIndex();
                        nodes_[index].VolumesIndex = volumeIndex;
                    }
                    nodeVolumes_[volumeIndex].push_back(volume);
                    continue;
                }
                if (level == maxLevel_ ||
                    glm::length(nodes_[index].Max.x - nodes_[index].Min.x) / 2.F < minChildSize_) {
                    nodeVolumes_[volumeIndex].push_back(volume);
                    continue;
                }
                split(index);
                childrenStartIndex = nodes_[index].ChildrenStartIndex;
            }

            auto inserted = false;
            const auto nextLevel = static_cast<byte>(state.Level + 1);
            for (auto i = 0; i < 4; ++i) {
                const auto childIdx = childrenStartIndex + i;
                if (!volume->IsInside(nodes_[childIdx].Min, nodes_[childIdx].Max)) {
                    continue;
                }
                if (nextFreeSlot >= bufferSize) { // enough space?
                    /*memcpy(nodeIndices.data(), &nodeIndices[ni + 1], (bufferSize - ni - 1) * sizeof(state));
                    nextFreeSlot = bufferSize - ni - 1;
                    ni = -1;*/

                    if (ni == 0) {
                        bufferSize += defaultBufferSize;
                        nodeIndices.resize(bufferSize);
                    } else {
                        if (ni + 1 == bufferSize) {
                            nextFreeSlot = 0;
                            ni = -1;
                        } else {
                            memcpy(nodeIndices.data(), &nodeIndices[ni + 1], (bufferSize - ni - 1) * sizeof(state));
                            nextFreeSlot = bufferSize - ni - 1;
                            ni = -1;
                        }
                    }
                }
                nodeIndices[nextFreeSlot++] = {.Index = childIdx, .Level = nextLevel};
                inserted = true;
            }
            if (!inserted) {
                if (volumeIndex == Node::UNSET) {
                    volumeIndex = freeNodeVolumesIndex();
                    nodes_[index].VolumesIndex = volumeIndex;
                }
                nodeVolumes_[volumeIndex].push_back(volume);
            }
        }
    }

    void QuadTree::Insert2(const Volume* volume) {
        if (nodes_[0].VolumesIndex == Node::UNSET) {
            nodes_[0].VolumesIndex = 0;
            nodeVolumes_.emplace_back();
        }
        insert(volume, 0, 0);
    }

    bool QuadTree::insert(const Volume* volume, const int index, const byte level) {
        if (!volume->IsInside(nodes_[index].Min, nodes_[index].Max)) {
            return false;
        }
        auto childrenStartIndex = nodes_[index].ChildrenStartIndex;
        if (childrenStartIndex == Node::UNSET) {
            if (nodeVolumes_[index].size() <= 3) {
                nodeVolumes_[index].push_back(volume);
                return true;
            }
            const float childSize = glm::length(nodes_[index].Max.x - nodes_[index].Min.x) / 2;
            if (level < maxLevel_ && childSize >= minChildSize_) {
                split2(index, level);
                childrenStartIndex = nodes_[index].ChildrenStartIndex;
            } else {
                nodeVolumes_[index].push_back(volume);
                return true;
            }
        }

        auto inserted = false;
        for (auto i = 0; i < 4; ++i) {
            const auto childIdx = static_cast<std::int16_t>(childrenStartIndex + i);
            if (insert(volume, childIdx, level + 1)) {
                inserted = true;
            }
        }
        if (inserted) {
            return true;
        }
        if (index == 0) {
            throw std::runtime_error("[QuadTree::Insert]: Node index out of range");
        }
        nodeVolumes_[index].push_back(volume);
        return true;
    }

    bool QuadTree::Remove(const Volume* volume) {
        bool removed = false;
        if (!volume->IsInside(nodes_[0].Min, nodes_[0].Max)) {
            return removed;
        }
        constexpr auto bufferSize = 1024;
        std::vector<int> nodeIndices;
        nodeIndices.resize(bufferSize);
        nodeIndices[0] = 0;
        auto nextFreeSlot = 1;
        auto ni = 0;
        for (; ni < nextFreeSlot; ++ni) {
            const auto index = nodeIndices[ni];
            auto volIndex = nodes_[index].VolumesIndex;
            if (volIndex != Node::UNSET) {
                for (auto vi = 0; vi < nodeVolumes_[volIndex].size(); ++vi) {
                    if (nodeVolumes_[volIndex][vi] != volume) {
                        continue;
                    }
                    if (vi < nodeVolumes_[volIndex].size() - 1) {
                        memcpy(&nodeVolumes_[volIndex][vi], &nodeVolumes_[volIndex][vi + 1],
                               (nodeVolumes_[volIndex].size() - vi - 1) * sizeof(Volume*));
                    }
                    nodeVolumes_[volIndex].pop_back();
                    if (nodeVolumes_[volIndex].empty()) {
                        freeNodeVolumes_.push_back(volIndex);
                        volIndex = -1;
                        nodes_[index].VolumesIndex = volIndex;
                    }
                    removed = true;
                    break;
                }
            }
            const auto childrenStartIndex = nodes_[index].ChildrenStartIndex;
            if (childrenStartIndex != Node::UNSET) {
                for (auto i = 0; i < 4; ++i) {
                    const auto childIdx = static_cast<std::uint16_t>(childrenStartIndex + i);
                    if (!volume->IsInside(nodes_[childIdx].Min, nodes_[childIdx].Max)) {
                        continue;
                    }
                    if (nextFreeSlot >= bufferSize) { // space for the all children?
                        memcpy(nodeIndices.data(), &nodeIndices[ni + 1], (bufferSize - ni - 1) * sizeof(state));
                        nextFreeSlot = bufferSize - ni - 1;
                        ni = -1;
                    }
                    nodeIndices[nextFreeSlot++] = childIdx;
                }
            }
        }
        return removed;
    }

    void QuadTree::QueryVolumesAt(const glm::vec3& point, std::vector<const Volume*>& volumes) const {
        QueryVolumesAt({point.x, point.z}, volumes);
    }

    void QuadTree::QueryVolumesAt(const glm::vec2& point, std::vector<const Volume*>& volumes) const {
        if (!nodes_[0].Contains(point)) {
            return;
        }
        std::queue<std::uint16_t> nodeIndices;
        nodeIndices.push(0);
        while (!nodeIndices.empty()) {
            const auto index = nodeIndices.front();
            const auto& node = nodes_[index];
            nodeIndices.pop();
            if (node.VolumesIndex != Node::UNSET) {
                volumes.insert(volumes.end(), nodeVolumes_[node.VolumesIndex].begin(),
                               nodeVolumes_[node.VolumesIndex].end());
            }
            if (node.ChildrenStartIndex != Node::UNSET) {
                for (std::uint16_t i = 0; i < 4; ++i) {
                    const auto childIdx = node.ChildrenStartIndex + i;
                    if (nodes_[childIdx].Contains(point)) {
                        nodeIndices.push(childIdx);
                    }
                }
            }
        }
    }

    void QuadTree::QueryNodeIndicesFor(const Volume* volume, std::vector<int>& indices) const {
        if (!volume->IsInside(nodes_[0].Min, nodes_[0].Max)) {
            return;
        }
        constexpr auto bufferSize = 1024;
        std::vector<int> nodeIndices;
        nodeIndices.resize(bufferSize);
        nodeIndices[0] = 0;
        auto nextFreeSlot = 1;
        auto ni = 0;
        for (; ni < nextFreeSlot; ++ni) {
            const auto index = nodeIndices[ni];
            if (nodes_[index].ChildrenStartIndex != Node::UNSET) {
                for (auto i = 0; i < 4; ++i) {
                    const auto childIdx = static_cast<std::int16_t>(nodes_[index].ChildrenStartIndex + i);
                    if (!volume->IsInside(nodes_[childIdx].Min, nodes_[childIdx].Max)) {
                        continue;
                    }
                    if (nextFreeSlot >= bufferSize) { // space for the all children?
                        memcpy(nodeIndices.data(), &nodeIndices[ni + 1], (bufferSize - ni - 1) * sizeof(state));
                        nextFreeSlot = bufferSize - ni - 1;
                        ni = -1;
                    }
                    nodeIndices[nextFreeSlot++] = childIdx;
                }
            } else {
                indices.push_back(index);
            }
        }
    }

    byte QuadTree::DetermineLevel(const std::uint16_t index) const {
        if (index == 0) {
            return 0;
        }
        std::uint16_t maxIndexPerLevel = 0;
        for (byte i = 1; i <= maxLevel_; ++i) {
            maxIndexPerLevel = static_cast<std::uint16_t>(std::pow(4, i) + maxIndexPerLevel);
            if (index < maxIndexPerLevel) {
                return i;
            }
        }
        return -1;
    }

    const Container::Node* QuadTree::GetNode(const int index) const {
        if (index < 0 || index >= nodes_.size()) {
            return nullptr;
        }
        return &nodes_[index];
    }

    void QuadTree::GetVolumes(const int index, std::vector<const Volume*>& volumes) const {
        if (nodes_[index].VolumesIndex == Node::UNSET) {
            return;
        }
        const auto& nv = nodeVolumes_[nodes_[index].VolumesIndex];
        volumes.insert(volumes.end(), nv.begin(), nv.end());
    }

    size_t QuadTree::GetNodeCount() const {
        return nodes_.size();
    }

    size_t QuadTree::GetVolumeCount() const {
        size_t result = 0;
        for (auto& v : nodeVolumes_) {
            result += v.size();
        }
        return result;
    }

    size_t QuadTree::GetChildrenPerNode() const {
        return 4;
    }

    void QuadTree::split(const std::uint16_t index) {
        const auto childrenStartIndex = static_cast<int>(nodes_.size());
        nodes_[index].ChildrenStartIndex = childrenStartIndex;
        nodes_.reserve(nodes_.size() + 4);
        const auto volumeIndex = nodes_[index].VolumesIndex;

        const float childWidth = (nodes_[index].Max.x - nodes_[index].Min.x) / 2.0F;
        const float childRadius = childWidth / 2.0F;
        const auto centerParent = nodes_[index].Max - glm::vec2(childWidth, childWidth);
        for (auto childIdx = 0; childIdx < 4; childIdx++) {
            auto centerChild = centerParent;
            centerChild.x += (childIdx & 1) != 0 ? childRadius : -childRadius;
            centerChild.y += (childIdx & 2) != 0 ? childRadius : -childRadius;
            nodes_.emplace_back(Node{.Min = glm::vec2(centerChild.x - childRadius, centerChild.y - childRadius),
                                     .Max = glm::vec2(centerChild.x + childRadius, centerChild.y + childRadius)});
        }
        if (volumeIndex == Node::UNSET) {
            return;
        }
        auto newSize = 0;
        for (auto vi = 0; vi < nodeVolumes_[volumeIndex].size(); vi++) {
            auto volume = nodeVolumes_[volumeIndex][vi];
            if (volume == nullptr) {
                continue;
            }
            for (std::uint16_t i = 0; i < 4; i++) {
                const auto childIdx = childrenStartIndex + i;
                if (volume->IsInside(nodes_[childIdx].Min, nodes_[childIdx].Max)) {
                    if (nodes_[childIdx].VolumesIndex == Node::UNSET) {
                        nodes_[childIdx].VolumesIndex = freeNodeVolumesIndex();
                    }
                    nodeVolumes_[nodes_[childIdx].VolumesIndex].push_back(volume);
                    nodeVolumes_[volumeIndex][vi] = nullptr;
                }
            }
            if (nodeVolumes_[volumeIndex][vi] == nullptr) {
                if (vi < nodeVolumes_[volumeIndex].size() - 1) {
                    nodeVolumes_[volumeIndex][vi] = nodeVolumes_[volumeIndex][vi + 1];
                }
            } else {
                newSize++;
            }
        }
        if (newSize == 0) {
            freeNodeVolumes_.push_back(volumeIndex);
            nodeVolumes_[volumeIndex].clear();
            nodes_[index].VolumesIndex = -1;
        } else {
            nodeVolumes_[volumeIndex].resize(newSize);
        }
    }

    void QuadTree::split2(const std::uint16_t index, const byte level) {
        const auto childrenStartIndex = static_cast<std::uint16_t>(nodes_.size());
        nodes_[index].ChildrenStartIndex = childrenStartIndex;
        nodes_.reserve(nodes_.size() + 4);
        nodeVolumes_.reserve(nodeVolumes_.size() + 4);

        const float childWidth = (nodes_[index].Max.x - nodes_[index].Min.x) / 2.0F;
        const float childRadius = childWidth / 2.0F;
        const auto centerParent = nodes_[index].Max - glm::vec2(childWidth, childWidth);
        for (auto childIdx = 0; childIdx < 4; childIdx++) {
            auto centerChild = centerParent;
            centerChild.x += (childIdx & 1) != 0 ? childRadius : -childRadius;
            centerChild.y += (childIdx & 2) != 0 ? childRadius : -childRadius;
            nodes_.emplace_back(Node{.VolumesIndex = static_cast<std::uint16_t>(childrenStartIndex + childIdx),
                                     .Min = glm::vec2(centerChild.x - childRadius, centerChild.y - childRadius),
                                     .Max = glm::vec2(centerChild.x + childRadius, centerChild.y + childRadius)});

            nodeVolumes_.emplace_back();
        }

        auto newVolumes = std::vector<const Volume*>();

        for (const auto* volume : nodeVolumes_[index]) {
            bool moved = false;
            for (auto i = 0; i < 4; i++) {
                const auto childIdx = childrenStartIndex + i;
                if (insert(volume, childIdx, level + 1)) {
                    moved = true;
                }
            }
            if (!moved) {
                newVolumes.push_back(volume);
            }
        }
        nodeVolumes_[index] = std::move(newVolumes);
    }

    std::uint16_t QuadTree::freeNodeVolumesIndex() {
        if (freeNodeVolumes_.empty()) {
            const auto volumesIndex = static_cast<int>(nodeVolumes_.size());
            nodeVolumes_.emplace_back();
            return volumesIndex;
        }
        const auto volumesIndex = freeNodeVolumes_.back();
        freeNodeVolumes_.pop_back();
        return volumesIndex;
    }
} // namespace soil::world::volume
