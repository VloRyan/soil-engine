#ifndef SOIL_ENGINE_VIDEO_SHADER_MOCKS_HPP
#define SOIL_ENGINE_VIDEO_SHADER_MOCKS_HPP
#include "video/shader/shader.h"
namespace soil::video::shader {
class ShaderMock : public Shader {
 public:
  ShaderMock() : Shader({}) {};
  ~ShaderMock() override = default;
  void Create() override { Calls.Create++; }
  void Use() override { Calls.Use++; }
  void Leave() override { Calls.Leave++; }
  void Prepare(render::State& state) override { Calls.Prepare++; }

  struct {
    int Create{0};
    int Use{0};
    int Leave{0};
    int Prepare{0};
  } Calls;
};
}  // namespace soil::video::shader
#endif
