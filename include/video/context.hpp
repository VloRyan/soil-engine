#ifndef SOIL_ENGINE_VIDEO_CONTEXT_HPP
#define SOIL_ENGINE_VIDEO_CONTEXT_HPP

#include <string>

#include "glm/glm.hpp"
#include "video/render/types.hpp"
class GLFWwindow;
namespace soil::video {
class Context {
 public:
  struct CreateParameter {
    glm::ivec2 Size{800, 600};
    int RefreshRate{-1};
    glm::ivec2 OpenGLVersion{3, 3};
    bool Fullscreen{false};
    std::string WindowTitle;
  };
  virtual ~Context() = default;
  virtual GLFWwindow* Window() = 0;

  virtual bool IsEnabled(uint cap) = 0;
  virtual int GetInteger(uint name) = 0;
  virtual std::string GetString(uint name) = 0;
  virtual std::string GetStringIndexed(uint name, uint index) = 0;
  virtual void SetDepthFunc(render::DepthFunc depthFunc) = 0;
  virtual void Enable(uint cap) = 0;
  virtual void Disable(uint cap) = 0;
  virtual void Scissor(int x, int y, int width, int height) = 0;
  virtual void BindFramebuffer(uint target, uint framebuffer) = 0;
  virtual void Viewport(int x, int y, int width, int height) = 0;
  virtual void ActiveTexture(uint texture) = 0;
  virtual void BindTexture(uint target, uint texture) = 0;
  virtual void Clear(uint mask) = 0;
  virtual void ClearColor(float r, float g, float b, float a) = 0;
  virtual void BindVertexArray(uint array) = 0;
  virtual void UseProgram(uint program) = 0;

  virtual std::vector<std::string> GetExtensions() = 0;

 protected:
  explicit Context(const CreateParameter&) {};
};

class EmptyContext : public Context {
 public:
  EmptyContext() : Context({}) {};
  ~EmptyContext() override = default;
  GLFWwindow* Window() override { return nullptr; }
  bool IsEnabled(uint cap) override { return false; }
  int GetInteger(uint name) override { return 0; }
  std::string GetString(uint name) override { return {}; }
  std::string GetStringIndexed(uint name, uint index) override { return {}; }
  void SetDepthFunc(render::DepthFunc depthFunc) override {}
  void Enable(uint cap) override {}
  void Disable(uint cap) override {}
  void Scissor(int x, int y, int width, int height) override {}
  void BindFramebuffer(uint target, uint framebuffer) override {}
  void Viewport(int x, int y, int width, int height) override {}
  void ActiveTexture(uint texture) override {}
  void BindTexture(uint target, uint texture) override {}
  void Clear(uint mask) override {}
  void ClearColor(float r, float g, float b, float a) override {}
  void BindVertexArray(uint array) override {}
  void UseProgram(uint program) override {}
  std::vector<std::string> GetExtensions() override { return {}; }
};
}  // namespace soil::video
#endif
