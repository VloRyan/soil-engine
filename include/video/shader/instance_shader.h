
#ifndef SOIL_VIDEO_SHADER_INSTANCE_SHADER_H
#define SOIL_VIDEO_SHADER_INSTANCE_SHADER_H
#include "shader.h"

namespace soil::video::shader {
    class InstanceShader : public Shader {
    public:
        explicit InstanceShader(const std::string &name, const std::string &path) : Shader(name, path) {}

        explicit InstanceShader(const Definition &def) : Shader(def) {}

        virtual size_t GetInstanceSize() const = 0;

        virtual std::vector<vertex::VertexAttribDescriptor> GetInstanceVertexDescriptors() = 0;

        virtual DrawMode GetDrawMode() = 0;

        virtual void PrepareRender() {}
    };
} // namespace soil::video::shader

#endif
