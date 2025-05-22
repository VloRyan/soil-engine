#include "video/shader/shader.h"
#include <glm/gtc/type_ptr.hpp>
#include <utility>
#include "GL/gl3w.h"
#include "plog/Log.h"
#include "util/files.h"

namespace soil::video::shader {
    uint Shader::vertexCount_ = 0;
    uint Shader::drawCount_ = 0;
    uint Shader::currentShaderId_ = 0;

    Shader::Shader(const std::string &name, const std::string &path) :
        name_(name), id_(0), linked_(false), attribCount_(0) {
        const auto filePath = path + name;
        AddStageIfExists(filePath, GLSL_SHADER_FILE_COMPUTE, GL_COMPUTE_SHADER);
        AddStageIfExists(filePath, GLSL_SHADER_FILE_VERTEX, GL_VERTEX_SHADER);
        AddStageIfExists(filePath, GLSL_SHADER_FILE_FRAGMENT, GL_FRAGMENT_SHADER);
        AddStageIfExists(filePath, GLSL_SHADER_FILE_GEOMETRY, GL_GEOMETRY_SHADER);
        AddStageIfExists(filePath, GLSL_SHADER_FILE_TESS_CONTROL, GL_TESS_CONTROL_SHADER);
        AddStageIfExists(filePath, GLSL_SHADER_FILE_TESS_EVALUATION, GL_TESS_EVALUATION_SHADER);
    }

    Shader::Shader(const Definition &def) : name_(def.ComputeName()), id_(0), linked_(false), attribCount_(0) {
        AddStageIfExists(def.GetComputeShaderFilePath(), GLSL_SHADER_FILE_COMPUTE, GL_COMPUTE_SHADER);
        AddStageIfExists(def.GetVertexShaderFilePath(), GLSL_SHADER_FILE_VERTEX, GL_VERTEX_SHADER);
        AddStageIfExists(def.GetFragmentShaderFilePath(), GLSL_SHADER_FILE_FRAGMENT, GL_FRAGMENT_SHADER);
        AddStageIfExists(def.GetGeometryShaderFilePath(), GLSL_SHADER_FILE_GEOMETRY, GL_GEOMETRY_SHADER);
        AddStageIfExists(def.GetTessellationControlShaderFilePath(), GLSL_SHADER_FILE_TESS_CONTROL,
                         GL_TESS_CONTROL_SHADER);
        AddStageIfExists(def.GetTessellationEvalStageShaderFilePath(), GLSL_SHADER_FILE_TESS_EVALUATION,
                         GL_TESS_EVALUATION_SHADER);
    }

    Shader::~Shader() {
        for (const Stage *stage : shaderStages_) {
            delete stage;
        }
        if (IsLinked()) {
            glDeleteProgram(this->GetId());
        }
        PLOG_DEBUG.printf("Deleted shader %s", name_.c_str());
    }

    void Shader::SetName(std::string Name) {
        name_ = std::move(Name);
    }

    std::string Shader::GetName() const {
        return name_;
    }

    void Shader::Create() {
#ifdef DEBUG
        if (id_ != 0) {
            throw std::runtime_error("shader program already created.");
        }
#endif
        // create the shader program
        id_ = glCreateProgram();
        for (auto *stage : shaderStages_) {
            stage->AttachToProgram(id_);
        }
        Link();
    }

    uint Shader::GetId() const {
        return id_;
    }

    void Shader::Use() {
        if (id_ == 0) {
            Create();
        }
        if (currentShaderId_ != GetId()) {
            glUseProgram(GetId());
            currentShaderId_ = GetId();
        }
    }

    void Shader::Leave() {
        glUseProgram(0);
        currentShaderId_ = 0;
    }

    bool Shader::UniformLocationExists(const std::string &name) {
        if (const UniformLocation location = GetUniformLocation(name); location == UNIFORM_NOT_FOUND) {
            PLOG_ERROR << GetName() << ": Uniform '" << name << "' does not exist.";
            return false;
        }
        return true;
    }

    void Shader::BindUniformBlock(const std::string &name, const int target) {
        Use();
        const auto location = glGetUniformBlockIndex(GetId(), name.c_str());
        if (location == UNIFORM_NOT_FOUND) {
            return;
        }
        glUniformBlockBinding(GetId(), location, target);
    }

    UniformLocation Shader::GetUniformLocation(const std::string &name) {
        if (const auto itr = uniformLocator_.find(name); itr != uniformLocator_.end()) {
            return itr->second;
        }

        UniformLocation location = glGetUniformLocation(this->GetId(), name.c_str());
        if (location == UNIFORM_NOT_FOUND) {
            PLOG_ERROR << GetName() << ": Uniform '" << name << "' does not exist.";
            return UNIFORM_NOT_FOUND;
        }
        uniformLocator_.insert({name, location});
        return location;
    }

    void Shader::Link() {
        if (this->GetId() == 0) {
            throw std::runtime_error("ShaderProgram is not created. Use create() before link().");
        }
        // Link our program, and set it as being actively used
        glLinkProgram(this->GetId());
        GLint IsLinked;
        glGetProgramiv(this->GetId(), GL_LINK_STATUS, &IsLinked);
        if (IsLinked == GL_FALSE) {
            GLint maxLength;
            glGetProgramiv(this->GetId(), GL_INFO_LOG_LENGTH, &maxLength);
            std::string errorMessage;
            if (maxLength > 0) {
                auto *const pLinkInfoLog = new char[maxLength];
                glGetProgramInfoLog(this->GetId(), maxLength, &maxLength, pLinkInfoLog);
                errorMessage.assign(pLinkInfoLog);
                delete[] pLinkInfoLog;
            } else {
                errorMessage = "Link failed for unknown reason.";
            }
            glDeleteProgram(this->GetId());
            id_ = 0;
            throw std::runtime_error(errorMessage);
        }
        glUseProgram(this->GetId());
        this->linked_ = true;
        // After link shaderStage will not be needed explicit and can be deleted
        for (const Stage *stage : shaderStages_) {
            delete stage;
        }
        shaderStages_.clear();
    }

    bool Shader::IsLinked() const {
        return this->linked_;
    }

    void Shader::DrawElements(const uint mode, const GLsizei indexCount, const vertex::IndexType indexType) {
        const GLenum glIndexType = GetGLIndexType(indexType);
        glDrawElements(mode, indexCount, glIndexType, nullptr);
        vertexCount_ += indexCount;
        ++drawCount_;
    }

    void Shader::DrawArrays(const uint mode, const int count, const int indexOffset) {
        glDrawArrays(mode, indexOffset, count);
        vertexCount_ += count;
        ++drawCount_;
    }

    void Shader::Prepare() {
        Use();
    }

    void Shader::DrawElementsInstanced(const uint mode, const uint indexCount, const vertex::IndexType indexType,
                                       const int instanceCount, const uint baseInstance) {
        const GLenum glIndexType = GetGLIndexType(indexType);
        glDrawElementsInstancedBaseInstance(mode, static_cast<GLsizei>(indexCount), glIndexType, nullptr, instanceCount,
                                            baseInstance);
        vertexCount_ += indexCount * instanceCount;
        ++drawCount_;
    }

    void Shader::DrawElementsIndirect(const uint mode, const vertex::IndexType indexType) {
        const GLenum glIndexType = GetGLIndexType(indexType);
        glDrawElementsIndirect(mode, glIndexType, static_cast<void *>(nullptr));
    }

    void Shader::SetUniform(const std::string &name, const int value) {
        const UniformLocation location = GetUniformLocation(name);
#ifdef DEBUG
        if (location == UNIFORM_NOT_FOUND) {
            PLOG_ERROR << GetName() << ": Uniform '" << name << "' does not exist.";
            return;
        }
#endif
        glUniform1i(location, value);
    }

    void Shader::SetUniform(const std::string &name, const uint value) {
        const UniformLocation location = GetUniformLocation(name);
#ifdef DEBUG
        if (location == UNIFORM_NOT_FOUND) {
            PLOG_ERROR << GetName() << ": Uniform '" << name << "' does not exist.";
            return;
        }
#endif
        glUniform1ui(location, value);
    }

    void Shader::SetUniform(const std::string &name, const float value) {
        const UniformLocation location = GetUniformLocation(name);
#ifdef DEBUG
        if (location == UNIFORM_NOT_FOUND) {
            PLOG_ERROR << GetName() << ": Uniform '" << name << "' does not exist.";
            return;
        }
#endif
        glUniform1f(location, value);
    }

    void Shader::SetUniform(const std::string &name, const glm::vec2 value) {
        const UniformLocation location = GetUniformLocation(name);
#ifdef DEBUG
        if (location == UNIFORM_NOT_FOUND) {
            PLOG_ERROR << GetName() << ": Uniform '" << name << "' does not exist.";
            return;
        }
#endif
        glUniform2fv(location, 1, &value[0]);
    }

    void Shader::SetUniform(const std::string &name, const glm::vec3 value) {
        const UniformLocation location = GetUniformLocation(name);
#ifdef DEBUG
        if (location == UNIFORM_NOT_FOUND) {
            PLOG_ERROR << GetName() << ": Uniform '" << name << "' does not exist.";
            return;
        }
#endif
        glUniform3fv(location, 1, &value[0]);
    }

    void Shader::SetUniform(const std::string &name, const glm::vec4 value) {
        const UniformLocation location = GetUniformLocation(name);
#ifdef DEBUG
        if (location == UNIFORM_NOT_FOUND) {
            PLOG_ERROR << GetName() << ": Uniform '" << name << "' does not exist.";
            return;
        }
#endif
        glUniform4fv(location, 1, &value[0]);
    }

    void Shader::SetUniform(const std::string &name, const glm::ivec2 value) {
        const UniformLocation location = GetUniformLocation(name);
#ifdef DEBUG
        if (location == UNIFORM_NOT_FOUND) {
            PLOG_ERROR << GetName() << ": Uniform '" << name << "' does not exist.";
            return;
        }
#endif
        glUniform2iv(location, 1, &value[0]);
    }

    void Shader::SetUniform(const std::string &name, const glm::ivec3 value) {
        const UniformLocation location = GetUniformLocation(name);
#ifdef DEBUG
        if (location == UNIFORM_NOT_FOUND) {
            PLOG_ERROR << GetName() << ": Uniform '" << name << "' does not exist.";
            return;
        }
#endif
        glUniform3iv(location, 1, &value[0]);
    }

    void Shader::SetUniform(const std::string &name, const glm::ivec4 value) {
        const UniformLocation location = GetUniformLocation(name);
#ifdef DEBUG
        if (location == UNIFORM_NOT_FOUND) {
            PLOG_ERROR << GetName() << ": Uniform '" << name << "' does not exist.";
            return;
        }
#endif
        glUniform4iv(location, 1, &value[0]);
    }

    void Shader::SetUniform(const std::string &name, const glm::uvec2 value) {
        const UniformLocation location = GetUniformLocation(name);
#ifdef DEBUG
        if (location == UNIFORM_NOT_FOUND) {
            PLOG_ERROR << GetName() << ": Uniform '" << name << "' does not exist.";
            return;
        }
#endif
        glUniform2uiv(location, 1, &value[0]);
    }

    void Shader::SetUniform(const std::string &name, const glm::uvec3 value) {
        const UniformLocation location = GetUniformLocation(name);
#ifdef DEBUG
        if (location == UNIFORM_NOT_FOUND) {
            PLOG_ERROR << GetName() << ": Uniform '" << name << "' does not exist.";
            return;
        }
#endif
        glUniform3uiv(location, 1, &value[0]);
    }

    void Shader::SetUniform(const std::string &name, const glm::uvec4 value) {
        const UniformLocation location = GetUniformLocation(name);
#ifdef DEBUG
        if (location == UNIFORM_NOT_FOUND) {
            PLOG_ERROR << GetName() << ": Uniform '" << name << "' does not exist.";
            return;
        }
#endif
        glUniform4uiv(location, 1, &value[0]);
    }

    void Shader::SetUniform(const std::string &name, const glm::mat4 &value, const bool transpose) {
        const UniformLocation location = GetUniformLocation(name);
#ifdef DEBUG
        if (location == UNIFORM_NOT_FOUND) {
            PLOG_ERROR << GetName() << ": Uniform '" << name << "' does not exist.";
            return;
        }
#endif
        glUniformMatrix4fv(location, 1, static_cast<GLboolean>(transpose), glm::value_ptr(value));
    }

    void Shader::SetUniform(const UniformLocation location, glm::mat4 value, const bool transpose) const {
#ifdef DEBUG
        if (location == -1) {
            PLOG_ERROR << GetName() << ": Uniform locator does not exist.";
            return;
        }
#endif
        glUniformMatrix4fv(location, 1, static_cast<GLboolean>(transpose), glm::value_ptr(value));
    }

    void Shader::SetUniform(const UniformLocation location, const float value) const {
#ifdef DEBUG
        if (location == -1) {
            PLOG_ERROR << GetName() << ": Uniform locator does not exist.";
            return;
        }
#endif
        glUniform1f(location, value);
    }

    void Shader::SetUniformHandle(const std::string &name, const uint handle) {
#ifdef DEBUG
        const UniformLocation location = GetUniformLocation(name);
        if (location == UNIFORM_NOT_FOUND) {
            PLOG_ERROR << GetName() << ": Uniform '" << name << "' does not exist.";
            return;
        }
#endif
        glProgramUniformHandleui64ARB(GetId(), GetUniformLocation(name), handle);
    }

    void Shader::SetTextures2d(const std::vector<texture::Texture *> &textures, const std::string &uniformName,
                               const byte maxTextures) {
        const auto location = GetUniformLocation(uniformName);
        for (auto i = 0; i < textures.size() && i < maxTextures; ++i) {
            SetTexture2d(static_cast<byte>(i), textures.at(i), location + i);
        }
    }

    void Shader::SetTextures2d(const std::vector<texture::Texture *> &textures, const UniformLocation location,
                               const byte maxTextures) {
        for (auto i = 0; i < textures.size() && i < maxTextures; ++i) {
            SetTexture2d(static_cast<byte>(i), textures.at(i), location + i);
        }
    }

    void Shader::SetTextures(const GLenum target, const std::vector<texture::Texture *> &textures,
                             const std::string &uniformName, const byte maxTextures) {
        const auto location = GetUniformLocation(uniformName);
        for (auto i = 0; i < textures.size() && i < maxTextures; ++i) {
            SetTexture(target, static_cast<byte>(i), textures.at(i), location + i);
        }
    }

    void Shader::SetTextures(const GLenum target, const std::vector<texture::Texture *> &textures,
                             const UniformLocation location, const byte maxTextures) {
        for (auto i = 0; i < textures.size() && i < maxTextures; ++i) {
            SetTexture(target, static_cast<byte>(i), textures.at(i), location + i);
        }
    }

    void Shader::SetTexture2d(const byte textureUnit, const texture::Texture *texture, const std::string &uniformName) {
        const UniformLocation location = GetUniformLocation(uniformName);
        SetTexture(GL_TEXTURE_2D, textureUnit, texture, location);
    }

    void Shader::SetTexture2d(const byte textureUnit, const texture::Texture *texture, const UniformLocation location) {
        SetTexture(GL_TEXTURE_2D, textureUnit, texture, location);
    }

    void Shader::SetTexture(const GLenum target, const byte textureUnit, const texture::Texture *texture,
                            const UniformLocation location) {
#ifdef DEBUG
        if (location == UNIFORM_NOT_FOUND) {
            PLOG_ERROR << "Texture Uniform does not exist.";
            return;
        }
#endif
        uint texId = 0;
        if (texture != nullptr) {
            texId = texture->GetId();
        }
        glActiveTexture(GL_TEXTURE0 + textureUnit * (GL_TEXTURE1 - GL_TEXTURE0));
        glBindTexture(target, texId);
        glUniform1i(location, textureUnit);
    }

    void Shader::SetTextureCubeMap(const byte textureUnit, const texture::Texture *texture,
                                   const std::string &uniformName) {
        SetTextureCubeMap(textureUnit, texture, GetUniformLocation(uniformName));
    }

    void Shader::SetTextureCubeMap(const byte textureUnit, const texture::Texture *texture,
                                   const UniformLocation location) {
#ifdef DEBUG
        if (location == UNIFORM_NOT_FOUND) {
            PLOG_ERROR << "TextureCubeMap Uniform does not exist.";
            return;
        }
#endif
        uint texId = 0;
        if (texture != nullptr) {
            texId = texture->GetId();
        }
        glActiveTexture(GL_TEXTURE0 + textureUnit * (GL_TEXTURE1 - GL_TEXTURE0));
        glBindTexture(GL_TEXTURE_CUBE_MAP, texId);
        glUniform1i(location, textureUnit); // 0 for GL_TEXTURE0
    }

    const void *Shader::GetBufferOffset(const GLenum indexType, const uint offset) {
        switch (indexType) {
        case GL_UNSIGNED_INT:
            {
                return BUFFER_OFFSET_UINT(offset);
            }
        case GL_UNSIGNED_SHORT:
        default:
            {
                return BUFFER_OFFSET_SHORT(offset);
            }
        }
    }

    GLenum Shader::GetGLIndexType(const vertex::IndexType indexType) {
        switch (indexType) {
        case vertex::IndexType::TYPE_UINT:
            {
                return GL_UNSIGNED_INT;
            }
        case vertex::IndexType::TYPE_USHORT:
        default:
            {
                return GL_UNSIGNED_SHORT;
            }
        }
    }

    uint Shader::GetDrawCount() {
        return drawCount_;
    }

    uint Shader::GetVerticeCount() {
        return vertexCount_;
    }

    void Shader::ResetCounter() {
        vertexCount_ = 0;
        drawCount_ = 0;
    }

    std::vector<Stage *> Shader::GetShaderStages() const {
        return shaderStages_;
    }

    void Shader::SetPatchVertices(const int value) {
        glPatchParameteri(GL_PATCH_VERTICES, value);
    }

    void Shader::DispatchCompute(const uint num_groups_x, const uint num_groups_y, const uint num_groups_z) {
        glDispatchCompute(num_groups_x, num_groups_y, num_groups_z);
    }

    void Shader::AddStageIfExists(const std::string &basePath, const std::string &ext, const uint type) {
        if (const auto filePath = basePath + "." + ext + GLSL_SHADER_FILE_EXTENSION; util::Files::Exists(filePath)) {
            auto *const stage = Stage::load(filePath, type);
            shaderStages_.push_back(stage);
        }
    }
} // namespace soil::video::shader
