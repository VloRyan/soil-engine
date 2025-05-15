#ifndef SOIL_VIDEO_VERTEX_VERTEX_H_
#define SOIL_VIDEO_VERTEX_VERTEX_H_
#include <glm/glm.hpp>

#include "attribute_pointer.h"

namespace soil::video::vertex {
    enum class IndexType { TYPE_USHORT = sizeof(ushort), TYPE_UINT = sizeof(uint) };

    enum VertexType { VERT_2D = 0, VERT_3D = 1 };

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
} // namespace soil::video::vertex
#endif // SOIL_VIDEO_VERTEX_VERTEX_H_
