#ifndef TEST_STAGE_MOCKS
#define TEST_STAGE_MOCKS

#include "stage/stage.h"

namespace soil::stage {
class StageMock : public Stage {
 public:
  int HandleInputEventCalledCount = 0;
  int HandleWindowEventCalledCount = 0;
  int UpdateCalledCount = 0;
  int RenderCalledCount = 0;
  bool CallRealRender = false;

  void ResetMocks() {
    HandleInputEventCalledCount = 0;
    HandleWindowEventCalledCount = 0;
  }

  void Handle(const input::Event& event) override {
    HandleInputEventCalledCount++;
    Stage::Handle(event);
  }

  void Handle(const WindowEvent& event) override {
    HandleWindowEventCalledCount++;
    Stage::Handle(event);
  }

  void Update() override {
    UpdateCalledCount++;
    Stage::Update();
  }

  void Render(video::render::State& state) override {
    RenderCalledCount++;
    if (CallRealRender) {
      Stage::Render(state);
    }
  }
};
}  // namespace soil::stage
#endif
