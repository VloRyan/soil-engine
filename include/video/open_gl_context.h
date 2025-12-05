#ifndef SOIL_ENGINE_VIDEO_OPEN_GL_CONTEXT_H
#define SOIL_ENGINE_VIDEO_OPEN_GL_CONTEXT_H
#include "context.hpp"
#include "video/render/types.hpp"
namespace soil::video {
class OpenGLContext : public Context {
 public:
  explicit OpenGLContext(const CreateParameter& parameter);
  ~OpenGLContext() override;
  GLFWwindow* Window() override;

  bool IsEnabled(uint cap) override;
  int GetInteger(uint name) override;
  std::string GetString(uint name) override;
  std::string GetStringIndexed(uint name, uint index) override;
  void SetDepthFunc(render::DepthFunc depthFunc) override;
  void Enable(uint cap) override;
  void Disable(uint cap) override;
  void Scissor(int x, int y, int width, int height) override;
  void BindFramebuffer(uint target, uint framebuffer) override;
  void Viewport(int x, int y, int width, int height) override;
  void ActiveTexture(uint texture) override;
  void BindTexture(uint target, uint texture) override;
  void Clear(uint mask) override;
  void ClearColor(float r, float g, float b, float a) override;
  void BindVertexArray(uint array) override;
  void UseProgram(uint program) override;
  std::vector<std::string> GetExtensions() override;

 private:
  GLFWwindow* window_;
};
}  // namespace soil::video
#endif
