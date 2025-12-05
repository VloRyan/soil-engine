
#include <gmock/gmock-matchers.h>

#include "gtest/gtest.h"
#include "video/render/draw/mocks.hpp"
#include "video/render/forward/forward_rendering.h"
#include "video/render/mocks.hpp"
#include "video/shader/mocks.hpp"

namespace soil::video::render::forward {
class ForwardRenderTest : public testing::Test {
 protected:
  static constexpr StateIdentifier opaqueStateId = {.State = {.Blend = false}};
  static constexpr StateIdentifier transparentStateId = {
      .State = {.Blend = true}};
};

TEST_F(ForwardRenderTest, Render) {
  auto emptyContext = video::EmptyContext();
  auto anyState = State(emptyContext);

  auto algorythm = ForwardRendering();

  auto container = draw::DrawableContainer();

  std::vector<video::render::draw::Drawable*> drawOrder;
  auto opaqueDrawableFar = video::render::draw::DrawableMock(opaqueStateId);
  opaqueDrawableFar.FixDistance = 1.F;
  opaqueDrawableFar.Mock.Draw = [&drawOrder](draw::Drawable* it) {
    drawOrder.push_back(it);
  };

  auto opaqueDrawableNear = video::render::draw::DrawableMock(opaqueStateId);
  opaqueDrawableNear.FixDistance = 0.2F;
  opaqueDrawableNear.Mock.Draw = [&drawOrder](draw::Drawable* it) {
    drawOrder.push_back(it);
  };

  auto transparentDrawableFar =
      video::render::draw::DrawableMock(transparentStateId);
  transparentDrawableFar.FixDistance = 1.F;
  transparentDrawableFar.Mock.Draw = [&drawOrder](draw::Drawable* it) {
    drawOrder.push_back(it);
  };

  auto transparentDrawableNear =
      video::render::draw::DrawableMock(transparentStateId);
  transparentDrawableNear.FixDistance = 0.2F;
  transparentDrawableNear.Mock.Draw = [&drawOrder](draw::Drawable* it) {
    drawOrder.push_back(it);
  };

  auto opaqueUnsortable = video::render::draw::DrawableMock(opaqueStateId);
  opaqueUnsortable.Sortable = false;
  opaqueUnsortable.Mock.Draw = [&drawOrder](draw::Drawable* it) {
    drawOrder.push_back(it);
  };

  auto transparentUnsortable =
      video::render::draw::DrawableMock(transparentStateId);
  transparentUnsortable.Sortable = false;
  transparentUnsortable.Mock.Draw = [&drawOrder](draw::Drawable* it) {
    drawOrder.push_back(it);
  };

  container.Insert(&transparentUnsortable);
  container.Insert(&opaqueUnsortable);
  container.Insert(&opaqueDrawableFar);
  container.Insert(&transparentDrawableNear);
  container.Insert(&transparentDrawableFar);
  container.Insert(&opaqueDrawableNear);

  algorythm.Render(anyState, container);
  EXPECT_THAT(
      drawOrder,
      testing::ElementsAre(          // opaque: nearToFar, unsortable
          &opaqueDrawableNear,       //
          &opaqueDrawableFar,        //
          &opaqueUnsortable,         //
          &transparentDrawableFar,   // transparent: farToNear, unsortable
          &transparentDrawableNear,  //
          &transparentUnsortable));
}

}  // namespace soil::video::render::forward
