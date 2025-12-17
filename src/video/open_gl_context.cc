#include "video/open_gl_context.h"

#include <GL/gl3w.h>
#include <GL/glcorearb.h>
#include <GLFW/glfw3.h>

#include <sstream>
namespace soil::video {

OpenGLContext::OpenGLContext(const Context::CreateParameter& parameter)
    : Context(parameter) {
  glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
  glfwWindowHint(GLFW_SAMPLES, GLFW_DONT_CARE);
  glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_EGL_CONTEXT_API);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, parameter.OpenGLVersion[0]);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, parameter.OpenGLVersion[1]);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);
  glfwWindowHint(GLFW_CONTEXT_ROBUSTNESS, GLFW_NO_ROBUSTNESS);
  glfwWindowHint(GLFW_CONTEXT_RELEASE_BEHAVIOR, GLFW_ANY_RELEASE_BEHAVIOR);
  glfwWindowHint(GLFW_CONTEXT_NO_ERROR, GLFW_FALSE);

  /*if (parameter.Maximized) { TODO
    glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
  }*/

#ifdef DEBUG
  glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#else
  glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_FALSE);
#endif
  GLFWmonitor* monitor = nullptr;
  if (parameter.Fullscreen) {
    monitor = glfwGetPrimaryMonitor();
  }
  window_ = glfwCreateWindow(parameter.Size.x, parameter.Size.y,
                             parameter.WindowTitle.c_str(), monitor, nullptr);
  if (window_ == nullptr) {
    const int code = glfwGetError(nullptr);
    std::stringstream stream;
    stream << std::hex << code;
    throw std::runtime_error("Failed to create OpenGL context (0x" +
                             stream.str() + ")");
  }
}
OpenGLContext::~OpenGLContext() {
  glfwDestroyWindow(window_);
  window_ = nullptr;
}

GLFWwindow* OpenGLContext::Window() { return window_; }
bool OpenGLContext::IsEnabled(uint cap) { return glIsEnabled(cap); }
int OpenGLContext::GetInteger(uint name) {
  int i;
  glGetIntegerv(name, &i);
  return i;
}
void OpenGLContext::SetDepthFunc(render::DepthFunc depthFunc) {
  glDepthFunc(static_cast<GLenum>(depthFunc));
}
void OpenGLContext::Enable(uint cap) { glEnable(cap); }
void OpenGLContext::Disable(uint cap) { glDisable(cap); }
void OpenGLContext::Scissor(int x, int y, int width, int height) {
  glScissor(x, y, width, height);
}
void OpenGLContext::BindFramebuffer(uint target, uint framebuffer) {
  glBindFramebuffer(target, framebuffer);
}
void OpenGLContext::Viewport(int x, int y, int width, int height) {
  glViewport(x, y, width, height);
}
void OpenGLContext::ActiveTexture(uint texture) { glActiveTexture(texture); }
void OpenGLContext::BindTexture(uint target, uint texture) {
  glBindTexture(target, texture);
}
void OpenGLContext::Clear(uint mask) { glClear(mask); }
void OpenGLContext::ClearColor(float r, float g, float b, float a) {
  glClearColor(r, g, b, a);
}
void OpenGLContext::BindVertexArray(uint array) { glBindVertexArray(array); }
void OpenGLContext::UseProgram(uint program) { glUseProgram(program); }
std::vector<std::string> OpenGLContext::GetExtensions() {
  std::vector<std::string> extensions;
  auto numExtensions = GetInteger(GL_NUM_EXTENSIONS);
  extensions.resize(numExtensions);
  for (uint i = 0U; i < numExtensions; i++) {
    extensions[i] = GetStringIndexed(GL_EXTENSIONS, i);
  }
  return extensions;
}
std::string OpenGLContext::GetString(uint name) {
  return reinterpret_cast<const char*>(glGetString(name));
}
std::string OpenGLContext::GetStringIndexed(uint name, uint index) {
  return reinterpret_cast<const char*>(glGetStringi(name, index));
}
void OpenGLContext::SwapBuffers() { glfwSwapBuffers(window_); }
}  // namespace soil::video
