#include "video/vertex/vertex.h"
#include "exception.h"

namespace soil::video::vertex {
    Vertex2d::Vertex2d() : m_Position(0.0F) {}

    Vertex3d::Vertex3d() : m_Position(0.0F) {}

    VertexTextured::VertexTextured() : m_TexCoords(0.0F), m_Normal(0.0F) {}

    NormalMappingVertex::NormalMappingVertex() : m_Tangent(0.0F), m_Bitangent(0.0F) {}

    BoneVertex::BoneVertex() : IDs(), Weights() {
        ZERO_MEM(IDs);
        ZERO_MEM(Weights);
    }

    NormalMappingBoneVertex::NormalMappingBoneVertex() : IDs(), Weights() {}

    void BoneVertex::AddBoneData(const uint BoneID, const float Weight) {
        for (int i = 0; i < std::size(IDs); ++i) {
            if (Weights[i] == 0.0F) {
                IDs[i] = BoneID;
                Weights[i] = Weight;
                return;
            }
        }

        // should never get here - more bones than we have space for
        throw Exception("More then 4 bones per vertex.");
    }

    void NormalMappingBoneVertex::AddBoneData(const uint BoneID, const float Weight) {
        for (int i = 0; i < std::size(IDs); ++i) {
            if (Weights[i] == 0.0F) {
                IDs[i] = BoneID;
                Weights[i] = Weight;
                return;
            }
        }

        // should never get here - more bones than we have space for
        throw Exception("More then 4 bones per vertex.");
    }
} // namespace soil::video::vertex
