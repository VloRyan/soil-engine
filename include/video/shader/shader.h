
#ifndef SOIL_VIDEO_SHADER_SHADER_H
#define SOIL_VIDEO_SHADER_SHADER_H
#include <glm/glm.hpp>
#include <string>
#include <unordered_map>
#include <vector>

#include "base.h"
#include "video/shader/definition.h"
#include "video/shader/stage.h"
#include "video/texture/texture.h"
#include "video/vertex/vertex.h"

#define BUFFER_OFFSET_UINT(i) ((char *)NULL + (i) * sizeof(GLuint))
#define BUFFER_OFFSET_SHORT(i) ((char *)NULL + (i) * sizeof(GLshort))

#define UNIFORM_NOT_FOUND (-1)
using UniformLocation = int;

namespace soil::video::shader {
    static const std::string SHADOW_MAPPING = "ShadowMapping";
    static const std::string COLOR_CORRECTION = "ColorCorrection";
    static const std::string LIGHTING_PASS = "LightingPass";

    class Shader {
    public:
        explicit Shader(const std::string &name, const std::string &path);

        explicit Shader(const Definition &def);

        virtual ~Shader();

        bool IsLinked() const;

        uint GetId() const;

        virtual void Create();

        virtual void Use();

        virtual void Leave();

        UniformLocation GetUniformLocation(const std::string &name);

        bool UniformLocationExists(const std::string &name);

        void BindUniformBlock(const std::string &name, int target);

        static void DrawElements(uint mode, GLsizei indexCount, vertex::IndexType indexType);

        static void DrawElementsInstanced(uint mode, uint indexCount, vertex::IndexType indexType, int instanceCount,
                                          uint baseInstance);

        static void DrawElementsIndirect(uint mode, vertex::IndexType indexType);

        static void DispatchCompute(uint num_groups_x, uint num_groups_y = 1, uint num_groups_z = 1);

        void SetUniform(const std::string &name, int value);

        void SetUniform(const std::string &name, uint value);

        void SetUniform(const std::string &name, float value);

        void SetUniform(const std::string &name, glm::vec2 value);

        void SetUniform(const std::string &name, glm::vec3 value);

        void SetUniform(const std::string &name, glm::vec4 value);

        void SetUniform(const std::string &name, glm::ivec2 value);

        void SetUniform(const std::string &name, glm::ivec3 value);

        void SetUniform(const std::string &name, glm::ivec4 value);

        void SetUniform(const std::string &name, glm::uvec2 value);

        void SetUniform(const std::string &name, glm::uvec3 value);

        void SetUniform(const std::string &name, glm::uvec4 value);

        void SetUniform(const std::string &name, const glm::mat4 &value, bool transpose = false);

        void SetUniform(UniformLocation location, glm::mat4 value, bool transpose = false) const;

        void SetUniformHandle(const std::string &name, uint handle);

        void SetUniform(UniformLocation location, float value) const;

        static void SetPatchVertices(int value);

        void SetTextures2d(const std::vector<texture::Texture *> &textures, const std::string &uniformName,
                           byte maxTextures = 32);

        static void SetTextures2d(const std::vector<texture::Texture *> &textures, UniformLocation location,
                                  byte maxTextures = 32);

        void SetTextures(GLenum target, const std::vector<texture::Texture *> &textures, const std::string &uniformName,
                         byte maxTextures = 32);

        static void SetTextures(GLenum target, const std::vector<texture::Texture *> &textures,
                                UniformLocation location, byte maxTextures = 32);

        static void SetTexture(GLenum target, byte textureUnit, const texture::Texture *texture,
                               UniformLocation location);

        void SetTexture2d(byte textureUnit, const texture::Texture *texture, const std::string &uniformName);

        static void SetTexture2d(byte textureUnit, const texture::Texture *texture, UniformLocation location);

        void SetTextureCubeMap(byte textureUnit, const texture::Texture *texture, const std::string &uniformName);

        static void SetTextureCubeMap(byte textureUnit, const texture::Texture *texture, UniformLocation location);

        static uint GetVerticeCount();

        static uint GetDrawCount();

        static void ResetCounter();

        std::vector<Stage *> GetShaderStages() const;

        void SetName(std::string Name);

        std::string GetName() const;

        static void DrawArrays(uint mode, int count, int indexOffset = 0);

    private:
        virtual void Link();

        void AddStageIfExists(const std::string &basePath, const std::string &ext, uint type);

        static uint GetGLIndexType(vertex::IndexType indexType);

        static const void *GetBufferOffset(uint indexType, uint offset);

        std::string name_;
        uint id_;
        std::vector<Stage *> shaderStages_;
        bool linked_;
        std::unordered_map<std::string, UniformLocation> uniformLocator_;
        uint attribCount_;
        static uint vertexCount_;
        static uint drawCount_;
        static uint currentShaderId_;
    };
} // namespace soil::video::shader
#endif // SOIL_VIDEO_SHADER_SHADER_H
