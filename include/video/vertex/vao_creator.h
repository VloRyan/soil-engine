
#ifndef SOIL_VIDEO_VERTEX_VAO_CREATOR_H
#define VAO_CREATOR_H
#include "vao.h"
#include "vertex.h"

namespace soil::video::vertex {
    class VaoCreator {
    public:
        static Vao* Create(VertexType type, const byte* data, gl_size_t dataSize, int vertexCount);
        VaoCreator() = delete;
    };
} // namespace soil::video::vertex


#endif // VAO_CREATOR_H
