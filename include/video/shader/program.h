#ifndef SOIL_VIDEO_SHADER_PROGRAM_H
#define SOIL_VIDEO_SHADER_PROGRAM_H
#include <string>
#include <unordered_map>
#include <vector>

#include "base.h"
#include "file_loader.h"
#include "glm/glm.hpp"
#include "video/render/state.h"
#include "video/shader/shader.h"
#include "video/texture/texture.h"
#include "video/vertex/vertex.h"

#define UNIFORM_NOT_FOUND (-1)
using UniformLocation = int;

namespace soil::video::shader {
class Program {
 public:
  struct Definition {
    std::string FilePath;
    std::string Name;
    std::string ComputeShaderName;
    std::string VertexShaderName;
    std::string FragmentShaderName;
    std::string GeometryShaderName;
    std::string TessellationControlShaderName;
    std::string TessellationEvalShaderName;
  };
  explicit Program(const std::string& name, const std::string& path);
  explicit Program(const Definition& def);
  virtual ~Program();

  // TODO: Create with context (decouple from opengl context)
  void Create();

  bool IsValid() const;
  uint GetId() const;
  UniformLocation GetUniformLocation(const std::string& name);
  bool UniformLocationExists(const std::string& name);
  void BindUniformBlock(const std::string& name, int target) const;
  static void DrawElements(uint mode, GLsizei indexCount,
                           vertex::IndexType indexType);
  static void DrawElementsInstanced(uint mode, uint indexCount,
                                    vertex::IndexType indexType,
                                    int instanceCount, uint baseInstance);
  static void DrawElementsIndirect(uint mode, vertex::IndexType indexType);
  static void DispatchCompute(uint num_groups_x, uint num_groups_y = 1,
                              uint num_groups_z = 1);
  void SetUniform(const std::string& name, bool value);
  void SetUniform(const std::string& name, byte value);
  void SetUniform(const std::string& name, int value);
  void SetUniform(const std::string& name, uint value);
  void SetUniform(const std::string& name, float value);
  void SetUniform(const std::string& name, glm::vec2 value);
  void SetUniform(const std::string& name, glm::vec3 value);
  void SetUniform(const std::string& name, glm::vec4 value);
  void SetUniform(const std::string& name, glm::ivec2 value);
  void SetUniform(const std::string& name, glm::ivec3 value);
  void SetUniform(const std::string& name, glm::ivec4 value);
  void SetUniform(const std::string& name, glm::uvec2 value);
  void SetUniform(const std::string& name, glm::uvec3 value);
  void SetUniform(const std::string& name, glm::uvec4 value);
  void SetUniform(const std::string& name, const glm::mat4& value,
                  bool transpose = false);
  void SetUniform(UniformLocation location, glm::mat4 value,
                  bool transpose = false) const;
  void SetUniformHandle(const std::string& name, uint handle);
  void SetUniform(UniformLocation location, float value) const;
  static void SetPatchVertices(int value);
  static uint GetVerticeCount();
  static uint GetDrawCount();
  static void ResetCounter();
  std::vector<Shader*> GetShaderStages() const;
  void SetName(std::string Name);
  std::string GetName() const;
  static void DrawArrays(uint mode, int count, int indexOffset = 0);
  virtual void Prepare(render::State& state);

 private:
  void AddShaderIfExists(const FileLoader& loader, const std::string& name,
                         Shader::Type type);
  std::string name_;
  uint id_;
  std::vector<Shader*> shaders_;
  std::unordered_map<std::string, UniformLocation> uniformLocator_;
  uint attribCount_;
  static uint vertexCount_;
  static uint drawCount_;
  static uint currentShaderId_;
};
}  // namespace soil::video::shader
#endif
