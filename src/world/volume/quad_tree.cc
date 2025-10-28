#include "world/volume/quad_tree.h"

#include <algorithm>
#include <cstring>
#include <debug/set>
#include <queue>
#include <stack>

namespace soil::world::volume {

QuadTree::QuadTree(const float size, const byte maxLevel,
                   const float minChildSize)
    : maxLevel_(maxLevel), minChildSize_(minChildSize) {
  const auto radius = size * 0.5F;
  nodes_.emplace_back(Node{.Min = glm::vec2(-radius, -radius),
                           .Max = glm::vec2(radius, radius)});
}

void QuadTree::ReserveSize(float size) {
  auto currentSize = nodes_[0].Max.x * 2.F;
  if (currentSize < size) {
    resize(size);
  }
}

void QuadTree::Insert(const entity::CollisionObject* object) {
  if (!object->IsInside(nodes_[0].Min, nodes_[0].Max)) {
    glm::vec3 min;
    glm::vec3 max;
    object->GetVolume()->GetAABB(min, max);
    min += object->GetPosition();
    max += object->GetPosition();
    auto maxDim = std::max(std::max(std::abs(min.x), max.x),
                           std::max(std::abs(min.z), max.z));
    auto newRadius = std::pow(2.F, std::ceilf(std::sqrt(maxDim)));
    resize(newRadius * 2.F);
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
      if (volumeIndex == Node::UNSET || nodeObjects_[volumeIndex].size() <= 3) {
        // nodes_[index].Contained.push_back(volume);
        if (volumeIndex == Node::UNSET) {
          volumeIndex = freeNodeObjectsIndex();
          nodes_[index].VolumesIndex = volumeIndex;
        }
        nodeObjects_[volumeIndex].push_back(object);
        continue;
      }
      if (level == maxLevel_ ||
          glm::length(nodes_[index].Max.x - nodes_[index].Min.x) / 2.F <
              minChildSize_) {
        nodeObjects_[volumeIndex].push_back(object);
        continue;
      }
      split(index);
      childrenStartIndex = nodes_[index].ChildrenStartIndex;
    }

    auto inserted = false;
    const auto nextLevel = static_cast<byte>(state.Level + 1);
    for (auto i = 0; i < 4; ++i) {
      const auto childIdx = childrenStartIndex + i;
      if (!object->IsInside(nodes_[childIdx].Min, nodes_[childIdx].Max)) {
        continue;
      }
      // enough space?
      if (nextFreeSlot >= bufferSize) {
        if (ni == 0) {
          bufferSize += defaultBufferSize;
          nodeIndices.resize(bufferSize);
        } else {
          if (ni + 1 == bufferSize) {
            nextFreeSlot = 0;
            ni = -1;
          } else {
            memcpy(nodeIndices.data(), &nodeIndices[ni + 1],
                   (bufferSize - ni - 1) * sizeof(state));
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
        volumeIndex = freeNodeObjectsIndex();
        nodes_[index].VolumesIndex = volumeIndex;
      }
      nodeObjects_[volumeIndex].push_back(object);
    }
  }
}

void QuadTree::Insert2(const entity::CollisionObject* object) {
  if (nodes_[0].VolumesIndex == Node::UNSET) {
    nodes_[0].VolumesIndex = 0;
    nodeObjects_.emplace_back();
  }
  insert(object, 0, 0);
}

void QuadTree::resize(float size) {
  const auto radius = size * 0.5F;
  auto prevObjects = nodeObjects_;
  Clear();
  nodes_[0].Min = glm::vec2(-radius);
  nodes_[0].Max = glm::vec2(radius);

  for (auto nodes : prevObjects) {
    for (auto node : nodes) {
      Insert(node);
    }
  }
}
bool QuadTree::insert(const entity::CollisionObject* object, const int index,
                      const byte level) {
  if (!object->IsInside(nodes_[index].Min, nodes_[index].Max)) {
    return false;
  }
  auto childrenStartIndex = nodes_[index].ChildrenStartIndex;
  if (childrenStartIndex == Node::UNSET) {
    if (nodeObjects_[index].size() <= 3) {
      nodeObjects_[index].push_back(object);
      return true;
    }
    const float childSize =
        glm::length(nodes_[index].Max.x - nodes_[index].Min.x) / 2;
    if (level < maxLevel_ && childSize >= minChildSize_) {
      split2(index, level);
      childrenStartIndex = nodes_[index].ChildrenStartIndex;
    } else {
      nodeObjects_[index].push_back(object);
      return true;
    }
  }

  auto inserted = false;
  for (auto i = 0; i < 4; ++i) {
    const auto childIdx = static_cast<std::int16_t>(childrenStartIndex + i);
    if (insert(object, childIdx, level + 1)) {
      inserted = true;
    }
  }
  if (inserted) {
    return true;
  }
  if (index == 0) {
    throw std::runtime_error("[QuadTree::Insert]: Node index out of range");
  }
  nodeObjects_[index].push_back(object);
  return true;
}

bool QuadTree::Remove(const entity::CollisionObject* object) {
  bool removed = false;
  if (!object->IsInside(nodes_[0].Min, nodes_[0].Max)) {
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
      for (auto vi = 0; vi < nodeObjects_[volIndex].size(); ++vi) {
        if (nodeObjects_[volIndex][vi] != object) {
          continue;
        }
        if (vi < nodeObjects_[volIndex].size() - 1) {
          memcpy(&nodeObjects_[volIndex][vi], &nodeObjects_[volIndex][vi + 1],
                 (nodeObjects_[volIndex].size() - vi - 1) * sizeof(Volume*));
        }
        nodeObjects_[volIndex].pop_back();
        if (nodeObjects_[volIndex].empty()) {
          freeNodeObjects_.push_back(volIndex);
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
        const auto childIdx =
            static_cast<std::uint16_t>(childrenStartIndex + i);
        if (!object->IsInside(nodes_[childIdx].Min, nodes_[childIdx].Max)) {
          continue;
        }
        // space for the all children?
        if (nextFreeSlot >= bufferSize) {
          memcpy(nodeIndices.data(), &nodeIndices[ni + 1],
                 (bufferSize - ni - 1) * sizeof(state));
          nextFreeSlot = bufferSize - ni - 1;
          ni = -1;
        }
        nodeIndices[nextFreeSlot++] = childIdx;
      }
    }
  }
  return removed;
}

void QuadTree::QueryObjectsAt(
    const glm::vec3& point,
    std::vector<const entity::CollisionObject*>& objects) const {
  QueryObjectsAt({point.x, point.z}, objects);
}

void QuadTree::QueryObjectsAt(
    const glm::vec2& point,
    std::vector<const entity::CollisionObject*>& objects) const {
  if (!nodes_[0].Contains(point)) {
    return;
  }
  std::queue<std::uint16_t> nodeIndices;
  std::set<const entity::CollisionObject*> objectSet;
  nodeIndices.push(0);
  while (!nodeIndices.empty()) {
    const auto index = nodeIndices.front();
    const auto& node = nodes_[index];
    nodeIndices.pop();
    if (node.VolumesIndex != Node::UNSET) {
      for (auto* obj : nodeObjects_[node.VolumesIndex]) {
        if (objectSet.contains(obj)) {
          continue;
        }
        if (obj->ContainsXZ(glm::vec3(point.x, 0.F, point.y))) {
          objectSet.insert(obj);
        }
      }
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
  objects.insert(objects.end(), objectSet.begin(), objectSet.end());
}

void QuadTree::QueryObjectsInRange(
    const glm::vec3& point, const float radius,
    std::vector<const entity::CollisionObject*>& objects) const {
  QueryObjectsInRange(glm::vec2(point.x, point.z), radius, objects);
}

void QuadTree::QueryObjectsInRange(
    const glm::vec2& point, const float radius,
    std::vector<const entity::CollisionObject*>& objects) const {
  if (!nodes_[0].Contains(point)) {
    return;
  }
  std::set<const entity::CollisionObject*> objectSet;
  std::queue<std::uint16_t> nodeIndices;
  nodeIndices.push(0);
  while (!nodeIndices.empty()) {
    const auto index = nodeIndices.front();
    const auto& node = nodes_[index];
    nodeIndices.pop();
    if (node.VolumesIndex != Node::UNSET) {
      for (auto* obj : nodeObjects_[node.VolumesIndex]) {
        if (objectSet.contains(obj)) {
          continue;
        }
        if (obj->IntersectsCircle(point, radius)) {
          objectSet.insert(obj);
        }
      }
    }
    if (node.ChildrenStartIndex != Node::UNSET) {
      for (std::uint16_t i = 0; i < 4; ++i) {
        const auto childIdx = node.ChildrenStartIndex + i;
        if (nodes_[childIdx].IntersectsCircle(point, radius)) {
          nodeIndices.push(childIdx);
        }
      }
    }
  }
  objects.insert(objects.end(), objectSet.begin(), objectSet.end());
}

bool QuadTree::isInsideCircle(const glm::vec2& point,
                              const glm::vec2& circleCenter,
                              const float radius) {
  // Calculate the squared distance from the center to the point
  const int distSq = (point.x - circleCenter.x) * (point.x - circleCenter.x) +
                     (point.y - circleCenter.y) * (point.y - circleCenter.y);

  // Compare the squared distance with the squared radius
  if (distSq <= radius * radius) {
    return true;  // Point is inside or on the circle
  } else {
    return false;  // Point is outside the circle
  }
}

void QuadTree::QueryNodeIndicesFor(const entity::CollisionObject* object,
                                   std::vector<int>& indices) const {
  if (!object->IsInside(nodes_[0].Min, nodes_[0].Max)) {
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
        const auto childIdx =
            static_cast<std::int16_t>(nodes_[index].ChildrenStartIndex + i);
        if (!object->IsInside(nodes_[childIdx].Min, nodes_[childIdx].Max)) {
          continue;
        }
        // space for the all children?
        if (nextFreeSlot >= bufferSize) {
          memcpy(nodeIndices.data(), &nodeIndices[ni + 1],
                 (bufferSize - ni - 1) * sizeof(state));
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
    maxIndexPerLevel =
        static_cast<std::uint16_t>(std::pow(4, i) + maxIndexPerLevel);
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

void QuadTree::GetNodeObjects(
    const int index,
    std::vector<const entity::CollisionObject*>& volumes) const {
  if (nodes_[index].VolumesIndex == Node::UNSET) {
    return;
  }
  const auto& nv = nodeObjects_[nodes_[index].VolumesIndex];
  volumes.insert(volumes.end(), nv.begin(), nv.end());
}
void QuadTree::WalkObjects(
    const std::function<void(const entity::CollisionObject*)> fun) const {
  for (const auto& node : nodes_) {
    if (node.VolumesIndex == Node::UNSET) {
      continue;
    }
    for (const auto* volume : nodeObjects_[node.VolumesIndex]) {
      fun(volume);
    }
  }
}

size_t QuadTree::GetNodeCount() const { return nodes_.size(); }

size_t QuadTree::GetVolumeCount() const {
  size_t result = 0;
  for (auto& v : nodeObjects_) {
    result += v.size();
  }
  return result;
}

size_t QuadTree::GetChildrenPerNode() const { return 4; }

void QuadTree::split(const std::uint16_t index) {
  const auto childrenStartIndex = static_cast<int>(nodes_.size());
  nodes_[index].ChildrenStartIndex = childrenStartIndex;
  nodes_.reserve(nodes_.size() + 4);
  const auto objectIndex = nodes_[index].VolumesIndex;

  const float childWidth = (nodes_[index].Max.x - nodes_[index].Min.x) / 2.0F;
  const float childRadius = childWidth / 2.0F;
  const auto centerParent =
      nodes_[index].Max - glm::vec2(childWidth, childWidth);
  for (auto childIdx = 0; childIdx < 4; childIdx++) {
    auto centerChild = centerParent;
    centerChild.x += (childIdx & 1) != 0 ? childRadius : -childRadius;
    centerChild.y += (childIdx & 2) != 0 ? childRadius : -childRadius;
    nodes_.emplace_back(Node{.Min = glm::vec2(centerChild.x - childRadius,
                                              centerChild.y - childRadius),
                             .Max = glm::vec2(centerChild.x + childRadius,
                                              centerChild.y + childRadius)});
  }
  if (objectIndex == Node::UNSET) {
    return;
  }
  auto newSize = 0;
  for (auto vi = 0; vi < nodeObjects_[objectIndex].size(); vi++) {
    auto volume = nodeObjects_[objectIndex][vi];
    if (volume == nullptr) {
      continue;
    }
    for (std::uint16_t i = 0; i < 4; i++) {
      const auto childIdx = childrenStartIndex + i;
      if (volume->IsInside(nodes_[childIdx].Min, nodes_[childIdx].Max)) {
        if (nodes_[childIdx].VolumesIndex == Node::UNSET) {
          nodes_[childIdx].VolumesIndex = freeNodeObjectsIndex();
        }
        nodeObjects_[nodes_[childIdx].VolumesIndex].push_back(volume);
        nodeObjects_[objectIndex][vi] = nullptr;
      }
    }
    if (nodeObjects_[objectIndex][vi] == nullptr) {
      if (vi < nodeObjects_[objectIndex].size() - 1) {
        nodeObjects_[objectIndex][vi] = nodeObjects_[objectIndex][vi + 1];
      }
    } else {
      newSize++;
    }
  }
  if (newSize == 0) {
    freeNodeObjects_.push_back(objectIndex);
    nodeObjects_[objectIndex].clear();
    nodes_[index].VolumesIndex = -1;
  } else {
    nodeObjects_[objectIndex].resize(newSize);
  }
}

void QuadTree::split2(const std::uint16_t index, const byte level) {
  const auto childrenStartIndex = static_cast<std::uint16_t>(nodes_.size());
  nodes_[index].ChildrenStartIndex = childrenStartIndex;
  nodes_.reserve(nodes_.size() + 4);
  nodeObjects_.reserve(nodeObjects_.size() + 4);

  const float childWidth = (nodes_[index].Max.x - nodes_[index].Min.x) / 2.0F;
  const float childRadius = childWidth / 2.0F;
  const auto centerParent =
      nodes_[index].Max - glm::vec2(childWidth, childWidth);
  for (auto childIdx = 0; childIdx < 4; childIdx++) {
    auto centerChild = centerParent;
    centerChild.x += (childIdx & 1) != 0 ? childRadius : -childRadius;
    centerChild.y += (childIdx & 2) != 0 ? childRadius : -childRadius;
    nodes_.emplace_back(Node{.VolumesIndex = static_cast<std::uint16_t>(
                                 childrenStartIndex + childIdx),
                             .Min = glm::vec2(centerChild.x - childRadius,
                                              centerChild.y - childRadius),
                             .Max = glm::vec2(centerChild.x + childRadius,
                                              centerChild.y + childRadius)});

    nodeObjects_.emplace_back();
  }

  auto newObjects = std::vector<const entity::CollisionObject*>();

  for (const auto* object : nodeObjects_[index]) {
    bool moved = false;
    for (auto i = 0; i < 4; i++) {
      const auto childIdx = childrenStartIndex + i;
      if (insert(object, childIdx, level + 1)) {
        moved = true;
      }
    }
    if (!moved) {
      newObjects.push_back(object);
    }
  }
  nodeObjects_[index] = std::move(newObjects);
}

std::uint16_t QuadTree::freeNodeObjectsIndex() {
  if (freeNodeObjects_.empty()) {
    const auto volumesIndex = static_cast<int>(nodeObjects_.size());
    nodeObjects_.emplace_back();
    return volumesIndex;
  }
  const auto volumesIndex = freeNodeObjects_.back();
  freeNodeObjects_.pop_back();
  return volumesIndex;
}
glm::vec3 QuadTree::GetSize() const {
  auto dim = nodes_[0].Max * glm::vec2(2.F);
  return glm::vec3(dim.x, 0.F, dim.y);
}
void QuadTree::Clear() {
  nodes_[0].VolumesIndex = Node::UNSET;
  nodes_[0].ChildrenStartIndex = Node::UNSET;
  nodes_.resize(1);
  nodeObjects_.clear();
  freeNodeObjects_.clear();
}
}  // namespace soil::world::volume
