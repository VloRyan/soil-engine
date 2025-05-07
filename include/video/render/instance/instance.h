#ifndef SOIL_VIDEO_RENDER_INSTANCE_INSTANCE_H_
#define SOIL_VIDEO_RENDER_INSTANCE_INSTANCE_H_


#include "video/buffer/cursor.h"
#include "video/mesh/mesh.h"

namespace soil::video::render::instance {
    enum class DrawMode : std::uint8_t {
        Points = GL_POINTS,
        Lines = GL_LINES,
        Triangles = GL_TRIANGLES,
        TriangleStrip = GL_TRIANGLE_STRIP,
        Quads = GL_QUADS,
    };

    struct Definition {
        mesh::Definition MeshDefinition;
        std::string ShaderName;
        DrawMode Mode;
        bool Opaque;

        [[nodiscard]] std::string ToString() const {
            return MeshDefinition.ToString() + ShaderName + "_" + std::to_string(static_cast<int>(Mode)) + "_" +
                std::to_string(static_cast<int>(Opaque));
        }
    };

    class Instance {
    public:
        explicit Instance();

        virtual ~Instance() = default;

        Instance(const Instance &other) = delete;

        virtual void WriteData(buffer::Cursor *cursor, int instanceId) const = 0;

        virtual float DistanceTo(glm::vec3 point) = 0;

        [[nodiscard]] int GetIndex() const;

        void SetIndex(int index);

    private:
        int index_;
    };
} // namespace soil::video::render::instance

#endif // SOIL_VIDEO_RENDER_INSTANCE_INSTANCE_H_
