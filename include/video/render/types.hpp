#ifndef SOIL_VIDEO_RENDER_TYPES_HPP
#define SOIL_VIDEO_RENDER_TYPES_HPP

#include <GL/glcorearb.h>

#include <cstdint>
#include <functional>
#include <glm/glm.hpp>
namespace soil::video::render {
enum class DepthFunc : std::int16_t {
  Never = GL_NEVER,
  Less = GL_LESS,
  Equal = GL_EQUAL,
  LessEqual = GL_LEQUAL,
  Greater = GL_GREATER,
  NotEqual = GL_NOTEQUAL,
  GreaterEqual = GL_GEQUAL,
};

enum class BufferBits : std::uint16_t {
  ColorBuffer = GL_COLOR_BUFFER_BIT,
  DepthBuffer = GL_DEPTH_BUFFER_BIT,
  StencilBuffer = GL_STENCIL_BUFFER_BIT,
};

struct BufferBitDescription {
  bool Color{false};
  bool Depth{false};
  bool Stencil{false};
};

enum class DrawMode : std::uint8_t {
  Points = GL_POINTS,
  Lines = GL_LINES,
  Triangles = GL_TRIANGLES,
  TriangleStrip = GL_TRIANGLE_STRIP,
  Quads = GL_QUADS,
};

enum class DrawCommand : std::uint8_t {
  Elements,
  ElementsInstanced,
};

struct Rect {
  glm::ivec2 LowerLeftPosition{};
  glm::ivec2 Size{};

  friend bool operator==(const Rect& lhs, const Rect& rhs) {
    return lhs.LowerLeftPosition == rhs.LowerLeftPosition &&
           lhs.Size == rhs.Size;
  }

  friend bool operator!=(const Rect& lhs, const Rect& rhs) {
    return !(lhs == rhs);
  }
};
}  // namespace soil::video::render
#endif
