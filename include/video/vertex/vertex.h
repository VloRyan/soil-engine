#ifndef SOIL_VIDEO_VERTEX_VERTEX_H_
#define SOIL_VIDEO_VERTEX_VERTEX_H_
#include <cstring>
#include <glm/glm.hpp>

#include "attribute_pointer.h"

#define NUM_BONES_PER_VERTEX 4
#define ZERO_MEM(a) memset(a, 0, sizeof(a))

namespace soil::video::vertex {
    enum class IndexType { TYPE_USHORT, TYPE_UINT };

    enum VertexType { VERT_2D = 0, VERT_3D = 1 };

    struct Vertex2d {
        glm::vec2 m_Position;

        Vertex2d();
    };

    struct Vertex3d {
        glm::vec3 m_Position;

        Vertex3d();
    };

    struct VertexTextured : Vertex3d {
        glm::vec2 m_TexCoords;
        glm::vec3 m_Normal;

        VertexTextured();
    };

    struct NormalMappingVertex : VertexTextured {
        /** tangent*/
        glm::vec3 m_Tangent;
        /** bitangent*/
        glm::vec3 m_Bitangent;

        NormalMappingVertex();
    };

    struct BoneVertex : VertexTextured {
        uint IDs[NUM_BONES_PER_VERTEX];
        float Weights[NUM_BONES_PER_VERTEX];

        void AddBoneData(uint BoneID, float Weight);

        BoneVertex();
    };

    struct NormalMappingBoneVertex : NormalMappingVertex {
        uint IDs[NUM_BONES_PER_VERTEX];
        float Weights[NUM_BONES_PER_VERTEX];

        void AddBoneData(uint BoneID, float Weight);

        NormalMappingBoneVertex();
    };

    struct VertexAttribDescriptor {
        int Elements;
        AttributePointer::DataType Type;
    };
} // namespace soil::video::vertex
#endif // SOIL_VIDEO_VERTEX_VERTEX_H_
