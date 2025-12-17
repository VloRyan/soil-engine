#include "video/shader/shader.h"

#include <algorithm>
#include <fstream>

#include "GL/gl3w.h"
#include "plog/Log.h"
#include "util/files.h"

namespace soil::video::shader {
Shader::Shader(const Type type) : id_(0), type_(type) {}

Shader::~Shader() {
  if (id_ > 0) {
    glDeleteShader(id_);
  }
}

bool Shader::Compile(const std::string& source) {
  if (id_ == 0) {
    id_ = glCreateShader(static_cast<uint>(type_));
  }
  GLchar const* files[] = {source.c_str()};
  GLint lengths[] = {static_cast<GLint>(source.size())};
  glShaderSource(id_, 1, files, lengths);
  glCompileShader(id_);
  GLint compiled;
  glGetShaderiv(id_, GL_COMPILE_STATUS, &compiled);
  return compiled == GL_TRUE;
}

void Shader::PrintShaderInfoLog(const uint shader) {
  int infoLogLen = 0;
  int charsWritten = 0;

  glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLen);

  if (infoLogLen > 0) {
    GLchar* infoLog = nullptr;
    infoLog = new GLchar[infoLogLen];
    // error check for fail to allocate memory omitted
    glGetShaderInfoLog(shader, infoLogLen, &charsWritten, infoLog);
    PLOG_INFO << infoLog;
    delete[] infoLog;
  }
}

uint Shader::Id() const { return id_; }

std::string Shader::GetFileExt(Shader::Type type) {
  switch (type) {
    case Type::Vertex:
      return ".vert.glsl";
    case Type::Fragment:
      return ".frag.glsl";
    case Type::Geometry:
      return ".geom.glsl";
    case Type::TesselationControl:
      return ".tesc.glsl";
    case Type::TesselationEvaluation:
      return ".tese.glsl";
    case Type::Compute:
      return ".comp.glsl";
  }
  throw std::runtime_error("unknown type");
}
}  // namespace soil::video::shader
