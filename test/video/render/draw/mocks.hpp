#ifndef TEST_VIDEO_RENDER_DRAW_MOCKS_HPP
#define TEST_VIDEO_RENDER_DRAW_MOCKS_HPP
#include "video/render/draw/drawable.hpp"
#include "video/render/draw/vao_elements_instanced.h"
namespace soil::video::render::draw {
class DrawableMock : public Drawable {
 public:
  DrawableMock() : Drawable() {};
  explicit DrawableMock(const StateIdentifier& id) : State(id) {};
  ~DrawableMock() override = default;
  bool IsSortable() override { return Sortable; }
  float DistanceTo(const glm::vec3& point) override { return FixDistance; }
  [[nodiscard]] const StateIdentifier& StateId() const override {
    return State;
  }
  void Draw(video::render::State& state) override {
    Calls.Draw++;
    if (Mock.Draw != nullptr) {
      Mock.Draw(this);
    }
  }
  struct {
    int Draw{0};
  } Calls;
  struct {
    std::function<void(Drawable* it)> Draw{nullptr};
  } Mock;

  void ResetMocks(bool onlyCounter = true) {
    Calls.Draw = 0;
    if (onlyCounter) {
      return;
    }
    Mock.Draw = nullptr;
  }
  float FixDistance{0.F};
  StateIdentifier State{};
  bool Sortable{true};
};

class InstanceDataMock : public VaoElementsInstanced::Data {
 public:
  void Write(const data::IWriter& writer) override { Calls.Write++; }
  void ResetMocks() { Calls.Write = 0; }
  struct {
    int Write{0};
  } Calls;
};
}  // namespace soil::video::render::draw
#endif
