#include "video/mesh/prefab.h"

#include <stdexcept>

namespace soil::video::mesh {
    std::string Prefab::Definition::TypeString() const {
        switch (Type) {
        case Type::Line:
            return "Line";
        case Type::Quad:
            return "Quad";
        case Type::Box:
            return "Box";
        }
        return "";
    }

    std::string Prefab::Definition::ToString() const {
        if (Identifier.empty()) {
            return TypeString();
        }
        return TypeString() + "_" + Identifier;
    }

    Data* Prefab::New(const Type type) {
        switch (type) {
        case Type::Line:
            {
                constexpr int vertices[2] {0, 1};
                constexpr GLushort indices[2] {0, 1};
                auto* meshData = new Data();
                meshData->SetDrawMode(shader::DrawMode::Lines);
                meshData->SetIndices(indices, 2);
                meshData->SetData(reinterpret_cast<const byte*>(vertices), 2 * sizeof(int),
                                  {{1, vertex::AttributePointer::DataType::Int}});
                return meshData;
            }
        case Type::Quad:
            {
                // The VBO containing the 4 vertices of the particles.
                constexpr float vertices[8] {
                    -0.5F, 0.5F, // Higher left
                    -0.5F, -0.5F, // Lower left
                    0.5F,  0.5F, // Higher right
                    0.5F,  -0.5F, // Lower right

                };

                // cube indices for index buffer object
                constexpr GLushort indices[4] {0, 1, 2, 3};

                auto* meshData = new Data();
                meshData->SetDrawMode(shader::DrawMode::TriangleStrip);
                meshData->SetIndices(indices, 4);
                meshData->SetData(reinterpret_cast<const byte*>(vertices), 4 * sizeof(glm::vec2),
                                  {{2, vertex::AttributePointer::DataType::Float}});
                return meshData;
            }
        case Type::Box:
            {
                constexpr GLushort indices[36] {0, 1, 2, 3, 4, 5, 5, 0, 2, 4, 6, 0, 6, 3, 1, 2, 3, 5,
                                                0, 6, 1, 3, 7, 4, 5, 4, 0, 4, 7, 6, 6, 7, 3, 2, 1, 3};
                constexpr float vertices[24] {
                    1.0, -1.0, 1.0, -1.0, -1.0, -1.0, 1.0,  -1.0, -1.0, -1.0, 1.0, -1.0,
                    1.0, 1.0,  1.0, 1.0,  1.0,  -1.0, -1.0, -1.0, 1.0,  -1.0, 1.0, 1.0,
                };
                /*return new Data(id, indices, 36, shader::DrawMode::Triangles,
                                reinterpret_cast<const byte *>(boxVertices), 24 * sizeof(float), 3 * sizeof(float),
                                vertex::VertexType::VERT_3D);*/
                auto* meshData = new Data();
                meshData->SetDrawMode(shader::DrawMode::Triangles);
                meshData->SetIndices(indices, 36);
                meshData->SetData(reinterpret_cast<const byte*>(vertices), 8 * sizeof(glm::vec3),
                                  {{3, vertex::AttributePointer::DataType::Float}});
                return meshData;
            }
        default:
            throw std::runtime_error("Unknown type");
        }
    }
} // namespace soil::video::mesh
