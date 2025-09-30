#ifndef SOIL_VIDEO_VERTEX_VERTEX_H_
#define SOIL_VIDEO_VERTEX_VERTEX_H_
#include <glm/glm.hpp>

#include "attribute_pointer.h"

namespace soil::video::vertex {
enum class IndexType : std::int16_t {
  TYPE_USHORT = GL_UNSIGNED_SHORT,
  TYPE_UINT = GL_UNSIGNED_INT,
};

static size_t IndexSize(const IndexType type) {
  switch (type) {
    case IndexType::TYPE_USHORT:
      return 2;  // 16 bit
    case IndexType::TYPE_UINT:
      return 4;  // 32 bit
  }
  return -1;
}

enum VertexType : std::int8_t {
  VERT_2D = 0,
  VERT_3D = 1,
};

struct Vertex2d {
  glm::vec2 Position{glm::vec2(0.F)};
};

struct Vertex3d {
  glm::vec3 Position{glm::vec3(0.F)};
};

struct VertexAttribDescriptor {
  int Elements;
  AttributePointer::DataType Type;
};
}  // namespace soil::video::vertex
#endif
