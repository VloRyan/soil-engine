#ifndef SOIL_VIDEO_SHADER_STAGE_H
#define SOIL_VIDEO_SHADER_STAGE_H
#include <GL/glcorearb.h>

#include <list>
#include <string>

#define GLSL_SHADER_FILE_EXTENSION ".glsl"
#define GLSL_SHADER_FILE_VERTEX "vert"
#define GLSL_SHADER_FILE_FRAGMENT "frag"
#define GLSL_SHADER_FILE_GEOMETRY "geom"
#define GLSL_SHADER_FILE_TESS_CONTROL "tesc"
#define GLSL_SHADER_FILE_TESS_EVALUATION "tese"
#define GLSL_SHADER_FILE_COMPUTE "comp"

namespace soil::video::shader {

class Shader final {
 public:
  enum class Type {
    Vertex = GL_VERTEX_SHADER,
    Fragment = GL_FRAGMENT_SHADER,
    Geometry = GL_GEOMETRY_SHADER,
    TesselationControl = GL_TESS_CONTROL_SHADER,
    TesselationEvaluation = GL_TESS_EVALUATION_SHADER,
    Compute = GL_COMPUTE_SHADER,
  };
  Shader(Type type);
  ~Shader();

  bool Compile(const std::string& source);

  // static std::string Load(std::string fileName, uint type);
  static void PrintShaderInfoLog(uint shader);

  [[nodiscard]] uint Id() const;

  static std::string GetFileExt(enum Type type);

 protected:
  /*static std::string replacePlaceholder(const std::string& line,
                                        std::string& originFileName);*/

 private:
 protected:
  uint id_;
  Type type_;
};
}  // namespace soil::video::shader
#endif
