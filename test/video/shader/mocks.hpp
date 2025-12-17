#ifndef SOIL_ENGINE_VIDEO_SHADER_MOCKS_HPP
#define SOIL_ENGINE_VIDEO_SHADER_MOCKS_HPP
#include "video/shader/program.h"
namespace soil::video::shader {
class ProgramMock : public Program {
 public:
  ProgramMock() : Program({}) {};
  ~ProgramMock() override = default;
  void Prepare(render::State& state) override { Calls.Prepare++; }

  struct {
    int Use{0};
    int Leave{0};
    int Prepare{0};
  } Calls;
};
}  // namespace soil::video::shader
#endif
