#include "video/shader/program.h"

#include <glm/gtc/type_ptr.hpp>
#include <utility>

#include "GL/gl3w.h"
#include "plog/Log.h"
#include "util/files.h"
#include "video/render/state.h"
#include "video/shader/file_loader.h"

namespace soil::video::shader {
uint Program::vertexCount_ = 0;
uint Program::drawCount_ = 0;

Program::Program(const std::string& name, const std::string& path)
    : name_(name), id_(0), attribCount_(0) {
  std::string shaderDir = util::Files::GetDirectory(path);
  std::string includeDir = shaderDir + "includes/";
  auto fileLoader = FileLoader(shaderDir, includeDir);
  AddShaderIfExists(fileLoader, name, Shader::Type::Compute);
  AddShaderIfExists(fileLoader, name, Shader::Type::Vertex);
  AddShaderIfExists(fileLoader, name, Shader::Type::Fragment);
  AddShaderIfExists(fileLoader, name, Shader::Type::Geometry);
  AddShaderIfExists(fileLoader, name, Shader::Type::TesselationControl);
  AddShaderIfExists(fileLoader, name, Shader::Type::TesselationEvaluation);
}

Program::Program(const Definition& def)
    : name_(def.Name), id_(0), attribCount_(0) {
  std::string shaderDir = util::Files::GetDirectory(def.FilePath);
  std::string includeDir = shaderDir + "includes/";
  auto fileLoader = FileLoader(shaderDir, includeDir);
  AddShaderIfExists(fileLoader, def.ComputeShaderName, Shader::Type::Compute);
  AddShaderIfExists(fileLoader, def.VertexShaderName, Shader::Type::Vertex);
  AddShaderIfExists(fileLoader, def.FragmentShaderName, Shader::Type::Fragment);
  AddShaderIfExists(fileLoader, def.GeometryShaderName, Shader::Type::Geometry);
  AddShaderIfExists(fileLoader, def.TessellationControlShaderName,
                    Shader::Type::TesselationControl);
  AddShaderIfExists(fileLoader, def.TessellationEvalShaderName,
                    Shader::Type::TesselationEvaluation);
}

Program::~Program() {
  for (const Shader* shader : shaders_) {
    if (shader->Id() != 0) {
      glDetachShader(id_, shader->Id());
    }
    delete shader;
  }
  if (id_ != 0) {
    glDeleteProgram(id_);
  }
  PLOG_DEBUG.printf("Deleted shader program %s", name_.c_str());
}

void Program::SetName(std::string Name) { name_ = std::move(Name); }

std::string Program::GetName() const { return name_; }

void Program::Create() {
#ifdef DEBUG
  if (id_ != 0) {
    throw std::runtime_error("shader program already created.");
  }
#endif
  // create the shader program
  id_ = glCreateProgram();
  for (auto* shader : shaders_) {
    glAttachShader(id_, shader->Id());
  }
  // Link our program, and set it as being actively used
  glLinkProgram(id_);
  GLint IsLinked;
  glGetProgramiv(id_, GL_LINK_STATUS, &IsLinked);
  if (IsLinked == GL_FALSE) {
    GLint maxLength;
    glGetProgramiv(id_, GL_INFO_LOG_LENGTH, &maxLength);
    std::string errorMessage;
    if (maxLength > 0) {
      auto* const pLinkInfoLog = new char[maxLength];
      glGetProgramInfoLog(id_, maxLength, &maxLength, pLinkInfoLog);
      errorMessage.assign(pLinkInfoLog);
      delete[] pLinkInfoLog;
    } else {
      errorMessage = "link shader program failed for unknown reason.";
    }
    glDeleteProgram(id_);
    id_ = 0;
    throw std::runtime_error(errorMessage);
  }
  // After link shader will not be needed explicit and can be deleted
  for (const Shader* shader : shaders_) {
    delete shader;
  }
  shaders_.clear();
}

uint Program::GetId() const { return id_; }
/*
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
*/
bool Program::UniformLocationExists(const std::string& name) {
  if (const UniformLocation location = GetUniformLocation(name);
      location == UNIFORM_NOT_FOUND) {
    PLOG_ERROR << GetName() << ": Uniform '" << name << "' does not exist.";
    return false;
  }
  return true;
}

void Program::BindUniformBlock(const std::string& name,
                               const int target) const {
  const auto location = glGetUniformBlockIndex(GetId(), name.c_str());
  if (location == UNIFORM_NOT_FOUND) {
    return;
  }
  glUniformBlockBinding(GetId(), location, target);
}

UniformLocation Program::GetUniformLocation(const std::string& name) {
  if (const auto itr = uniformLocator_.find(name);
      itr != uniformLocator_.end()) {
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

bool Program::IsValid() const { return this->id_ != 0; }

void Program::DrawElements(const uint mode, const GLsizei indexCount,
                           const vertex::IndexType indexType) {
  const auto glIndexType = static_cast<GLenum>(indexType);
  glDrawElements(mode, indexCount, glIndexType, nullptr);
  vertexCount_ += indexCount;
  ++drawCount_;
}

void Program::DrawArrays(const uint mode, const int count,
                         const int indexOffset) {
  glDrawArrays(mode, indexOffset, count);
  vertexCount_ += count;
  ++drawCount_;
}

void Program::Prepare(render::State& state) {
  /*if (id_ == 0) {
    Create();
  }
  state.SetShader(this);*/
}

void Program::DrawElementsInstanced(const uint mode, const uint indexCount,
                                    const vertex::IndexType indexType,
                                    const int instanceCount,
                                    const uint baseInstance) {
  const auto glIndexType = static_cast<GLenum>(indexType);
  glDrawElementsInstancedBaseInstance(mode, static_cast<GLsizei>(indexCount),
                                      glIndexType, nullptr, instanceCount,
                                      baseInstance);
  vertexCount_ += indexCount * instanceCount;
  ++drawCount_;
}

void Program::DrawElementsIndirect(const uint mode,
                                   const vertex::IndexType indexType) {
  const auto glIndexType = static_cast<GLenum>(indexType);
  glDrawElementsIndirect(mode, glIndexType, static_cast<void*>(nullptr));
}

void Program::SetUniform(const std::string& name, const byte value) {
  SetUniform(name, static_cast<int>(value));
}

void Program::SetUniform(const std::string& name, const bool value) {
  SetUniform(name, value ? 1 : 0);
}

void Program::SetUniform(const std::string& name, const int value) {
  const UniformLocation location = GetUniformLocation(name);
#ifdef DEBUG
  if (location == UNIFORM_NOT_FOUND) {
    PLOG_ERROR << GetName() << ": Uniform '" << name << "' does not exist.";
    return;
  }
#endif
  glUniform1i(location, value);
}

void Program::SetUniform(const std::string& name, const uint value) {
  const UniformLocation location = GetUniformLocation(name);
#ifdef DEBUG
  if (location == UNIFORM_NOT_FOUND) {
    PLOG_ERROR << GetName() << ": Uniform '" << name << "' does not exist.";
    return;
  }
#endif
  glUniform1ui(location, value);
}

void Program::SetUniform(const std::string& name, const float value) {
  const UniformLocation location = GetUniformLocation(name);
#ifdef DEBUG
  if (location == UNIFORM_NOT_FOUND) {
    PLOG_ERROR << GetName() << ": Uniform '" << name << "' does not exist.";
    return;
  }
#endif
  glUniform1f(location, value);
}

void Program::SetUniform(const std::string& name, const glm::vec2 value) {
  const UniformLocation location = GetUniformLocation(name);
#ifdef DEBUG
  if (location == UNIFORM_NOT_FOUND) {
    PLOG_ERROR << GetName() << ": Uniform '" << name << "' does not exist.";
    return;
  }
#endif
  glUniform2fv(location, 1, &value[0]);
}

void Program::SetUniform(const std::string& name, const glm::vec3 value) {
  const UniformLocation location = GetUniformLocation(name);
#ifdef DEBUG
  if (location == UNIFORM_NOT_FOUND) {
    PLOG_ERROR << GetName() << ": Uniform '" << name << "' does not exist.";
    return;
  }
#endif
  glUniform3fv(location, 1, &value[0]);
}

void Program::SetUniform(const std::string& name, const glm::vec4 value) {
  const UniformLocation location = GetUniformLocation(name);
#ifdef DEBUG
  if (location == UNIFORM_NOT_FOUND) {
    PLOG_ERROR << GetName() << ": Uniform '" << name << "' does not exist.";
    return;
  }
#endif
  glUniform4fv(location, 1, &value[0]);
}

void Program::SetUniform(const std::string& name, const glm::ivec2 value) {
  const UniformLocation location = GetUniformLocation(name);
#ifdef DEBUG
  if (location == UNIFORM_NOT_FOUND) {
    PLOG_ERROR << GetName() << ": Uniform '" << name << "' does not exist.";
    return;
  }
#endif
  glUniform2iv(location, 1, &value[0]);
}

void Program::SetUniform(const std::string& name, const glm::ivec3 value) {
  const UniformLocation location = GetUniformLocation(name);
#ifdef DEBUG
  if (location == UNIFORM_NOT_FOUND) {
    PLOG_ERROR << GetName() << ": Uniform '" << name << "' does not exist.";
    return;
  }
#endif
  glUniform3iv(location, 1, &value[0]);
}

void Program::SetUniform(const std::string& name, const glm::ivec4 value) {
  const UniformLocation location = GetUniformLocation(name);
#ifdef DEBUG
  if (location == UNIFORM_NOT_FOUND) {
    PLOG_ERROR << GetName() << ": Uniform '" << name << "' does not exist.";
    return;
  }
#endif
  glUniform4iv(location, 1, &value[0]);
}

void Program::SetUniform(const std::string& name, const glm::uvec2 value) {
  const UniformLocation location = GetUniformLocation(name);
#ifdef DEBUG
  if (location == UNIFORM_NOT_FOUND) {
    PLOG_ERROR << GetName() << ": Uniform '" << name << "' does not exist.";
    return;
  }
#endif
  glUniform2uiv(location, 1, &value[0]);
}

void Program::SetUniform(const std::string& name, const glm::uvec3 value) {
  const UniformLocation location = GetUniformLocation(name);
#ifdef DEBUG
  if (location == UNIFORM_NOT_FOUND) {
    PLOG_ERROR << GetName() << ": Uniform '" << name << "' does not exist.";
    return;
  }
#endif
  glUniform3uiv(location, 1, &value[0]);
}

void Program::SetUniform(const std::string& name, const glm::uvec4 value) {
  const UniformLocation location = GetUniformLocation(name);
#ifdef DEBUG
  if (location == UNIFORM_NOT_FOUND) {
    PLOG_ERROR << GetName() << ": Uniform '" << name << "' does not exist.";
    return;
  }
#endif
  glUniform4uiv(location, 1, &value[0]);
}

void Program::SetUniform(const std::string& name, const glm::mat4& value,
                         const bool transpose) {
  const UniformLocation location = GetUniformLocation(name);
#ifdef DEBUG
  if (location == UNIFORM_NOT_FOUND) {
    PLOG_ERROR << GetName() << ": Uniform '" << name << "' does not exist.";
    return;
  }
#endif
  glUniformMatrix4fv(location, 1, static_cast<GLboolean>(transpose),
                     glm::value_ptr(value));
}

void Program::SetUniform(const UniformLocation location, glm::mat4 value,
                         const bool transpose) const {
#ifdef DEBUG
  if (location == -1) {
    PLOG_ERROR << GetName() << ": Uniform locator does not exist.";
    return;
  }
#endif
  glUniformMatrix4fv(location, 1, static_cast<GLboolean>(transpose),
                     glm::value_ptr(value));
}

void Program::SetUniform(const UniformLocation location,
                         const float value) const {
#ifdef DEBUG
  if (location == -1) {
    PLOG_ERROR << GetName() << ": Uniform locator does not exist.";
    return;
  }
#endif
  glUniform1f(location, value);
}

void Program::SetUniformHandle(const std::string& name, const uint handle) {
#ifdef DEBUG
  const UniformLocation location = GetUniformLocation(name);
  if (location == UNIFORM_NOT_FOUND) {
    PLOG_ERROR << GetName() << ": Uniform '" << name << "' does not exist.";
    return;
  }
#endif
  glProgramUniformHandleui64ARB(GetId(), GetUniformLocation(name), handle);
}

uint Program::GetDrawCount() { return drawCount_; }

uint Program::GetVerticeCount() { return vertexCount_; }

void Program::ResetCounter() {
  vertexCount_ = 0;
  drawCount_ = 0;
}

std::vector<Shader*> Program::GetShaderStages() const { return shaders_; }

void Program::SetPatchVertices(const int value) {
  glPatchParameteri(GL_PATCH_VERTICES, value);
}

void Program::DispatchCompute(const uint num_groups_x, const uint num_groups_y,
                              const uint num_groups_z) {
  glDispatchCompute(num_groups_x, num_groups_y, num_groups_z);
}

void Program::AddShaderIfExists(const FileLoader& loader,
                                const std::string& name,
                                const Shader::Type type) {
  auto shaderFile = loader.BaseDir() + name + Shader::GetFileExt(type);

  if (!util::Files::Exists(shaderFile)) {
    return;
  }
  auto* const shader = new Shader(type);
  auto content = loader.Load(shaderFile);
  if (!shader->Compile(content)) {
    std::string typeStr;
    switch (type) {
      case Shader::Type::Vertex:
        typeStr = "Vertex";
        break;
      case Shader::Type::Fragment:
        typeStr = "Fragment";
        break;
      case Shader::Type::Geometry:
        typeStr = "Geometry";
        break;
      case Shader::Type::TesselationControl:
        typeStr = "Tesselation Control";
        break;
      case Shader::Type::TesselationEvaluation:
        typeStr = "Tesselation Evaluation";
        break;
      case Shader::Type::Compute:
        typeStr = "Compute";
        break;
    }

    PLOG_ERROR.printf("%s shader not compiled.", typeStr.c_str());
    Shader::PrintShaderInfoLog(id_);
    delete shader;
    std::string message =
        "Impossible to compile the shader file '" + shaderFile + "'";
    throw std::runtime_error(message);
  }
  shaders_.push_back(shader);
}
}  // namespace soil::video::shader
