#ifndef SOIL_VIDEO_RENDER_STATE_IDENTIFIER_H
#define SOIL_VIDEO_RENDER_STATE_IDENTIFIER_H

#include <cstddef>
#include <functional>
#include <iomanip>
#include <iostream>
#include <string>
#include <unordered_set>
#include <video/render/types.hpp>

#include "state.h"
namespace soil::video::shader {
class Program;
}
namespace soil::video::render {

struct StateIdentifier final {
  video::shader::Program* Shader{nullptr};
  uint VAOId{0};
  const vertex::Vao* Vao{nullptr};
  enum DrawMode DrawMode { DrawMode::Points };
  StateDef State;

  bool operator()(const StateIdentifier& lhs,
                  const StateIdentifier& rhs) const noexcept {
    return lhs == rhs;
  }

  bool operator==(const StateIdentifier& rhs) const {
    return Shader == rhs.Shader && VAOId == rhs.VAOId &&
           DrawMode == rhs.DrawMode && State == rhs.State;
  }

  bool operator!=(const StateIdentifier& rhs) const { return !(rhs == *this); }

  bool operator<(const StateIdentifier& rhs) const {
    if (Shader < rhs.Shader) return true;
    if (rhs.Shader < Shader) return false;
    if (VAOId < rhs.VAOId) return true;
    if (rhs.VAOId < VAOId) return false;
    if (DrawMode < rhs.DrawMode) return true;
    if (rhs.DrawMode < DrawMode) return false;
    return State < rhs.State;
  }
  bool operator>(const StateIdentifier& rhs) const { return rhs < *this; }
  bool operator<=(const StateIdentifier& rhs) const { return !(rhs < *this); }
  bool operator>=(const StateIdentifier& rhs) const { return !(*this < rhs); }

  [[nodiscard]] int ChangeCount(const StateIdentifier& other) const {
    auto changes = 0;
    if (Shader != other.Shader) changes++;
    if (VAOId != other.VAOId) changes++;
    if (DrawMode != other.DrawMode) changes++;
    if (State.Blend != other.State.Blend) changes++;
    if (State.ScissorTest != other.State.ScissorTest) changes++;
    if (State.StencilTest != other.State.StencilTest) changes++;
    if (State.DepthTest != other.State.DepthTest) changes++;
    if (State.DepthFunc != other.State.DepthFunc) changes++;

    return changes;
  }
};
struct StateIdentifierEquality {
  bool operator()(const StateIdentifier& lhs,
                  const StateIdentifier& rhs) const noexcept {
    return lhs == rhs;
  }
  size_t operator()(StateIdentifier const& key) const {
    std::size_t seed = 0;
    hash_combine(seed, key.Shader);
    hash_combine(seed, key.VAOId);
    hash_combine(seed, key.DrawMode);
    hash_combine(seed, key.State.Blend);
    hash_combine(seed, key.State.ScissorTest);
    hash_combine(seed, key.State.StencilTest);
    hash_combine(seed, key.State.DepthTest);
    hash_combine(seed, key.State.DepthFunc);
    return seed;
  }
  template <class T>
  static inline void hash_combine(std::size_t& seed, const T& v) {
    std::hash<T> hasher;
    seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
  }
};
}  // namespace soil::video::render

#endif
