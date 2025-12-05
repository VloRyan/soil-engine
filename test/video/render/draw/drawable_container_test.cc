

#include "video/render/draw/drawable_container.h"

#include <gmock/gmock-matchers.h>

#include "gtest/gtest.h"
#include "video/render/draw/mocks.hpp"
#include "video/render/mocks.hpp"

namespace soil::video::render::draw {
class DrawableContainerTest : public testing::Test {
 protected:
  static constexpr StateIdentifier opaqueStateId = {.State = {.Blend = false}};
  static constexpr StateIdentifier transparentStateId = {
      .State = {.Blend = true}};
};

TEST_F(DrawableContainerTest, SortNearToFar) {
  auto container = DrawableContainer();

  auto opaqueDrawableFar = video::render::draw::DrawableMock(opaqueStateId);
  opaqueDrawableFar.FixDistance = 1.F;

  auto opaqueDrawableMiddle = video::render::draw::DrawableMock(opaqueStateId);
  opaqueDrawableMiddle.FixDistance = 0.8F;

  auto opaqueDrawableNear = video::render::draw::DrawableMock(opaqueStateId);
  opaqueDrawableNear.FixDistance = 0.2F;

  auto transparentDrawable =
      video::render::draw::DrawableMock(transparentStateId);
  transparentDrawable.FixDistance = 1.F;

  auto opaqueUnsortable = video::render::draw::DrawableMock(opaqueStateId);
  opaqueUnsortable.Sortable = false;

  container.Insert(&opaqueUnsortable);
  container.Insert(&opaqueDrawableFar);
  container.Insert(&transparentDrawable);
  container.Insert(&opaqueDrawableMiddle);
  container.Insert(&opaqueDrawableNear);

  container.Sort(DrawableContainer::Kind::Opaque,
                 DrawableContainer::SortDirection::NearToFar);

  EXPECT_THAT(container.GetDrawables(DrawableContainer::Kind::Opaque),
              testing::ElementsAre(       // near < far; sortable < unsortable
                  &opaqueDrawableNear,    //
                  &opaqueDrawableMiddle,  //
                  &opaqueDrawableFar,     //
                  &opaqueUnsortable));
}

TEST_F(DrawableContainerTest, SortFarToNear) {
  auto container = DrawableContainer();

  auto transparentDrawableFar =
      video::render::draw::DrawableMock(transparentStateId);
  transparentDrawableFar.FixDistance = 1.F;

  auto transparentDrawableMiddle =
      video::render::draw::DrawableMock(transparentStateId);
  transparentDrawableMiddle.FixDistance = 0.8F;

  auto transparentDrawableNear =
      video::render::draw::DrawableMock(transparentStateId);
  transparentDrawableNear.FixDistance = 0.2F;

  auto opaqueDrawable = video::render::draw::DrawableMock(opaqueStateId);
  opaqueDrawable.FixDistance = 1.F;

  auto transparentUnsortable =
      video::render::draw::DrawableMock(transparentStateId);
  transparentUnsortable.Sortable = false;

  container.Insert(&transparentUnsortable);
  container.Insert(&transparentDrawableNear);
  container.Insert(&opaqueDrawable);
  container.Insert(&transparentDrawableMiddle);
  container.Insert(&transparentDrawableFar);

  container.Sort(DrawableContainer::Kind::Transparent,
                 DrawableContainer::SortDirection::FarToNear);

  EXPECT_THAT(container.GetDrawables(DrawableContainer::Kind::Transparent),
              testing::ElementsAre(         // far < near; sortable < unsortable
                  &transparentDrawableFar,  //
                  &transparentDrawableMiddle,  //
                  &transparentDrawableNear,    //
                  &transparentUnsortable));
}

TEST_F(DrawableContainerTest, Remove) {
  auto container = DrawableContainer();

  auto opaqueDrawable = video::render::draw::DrawableMock(opaqueStateId);
  auto transparentDrawable =
      video::render::draw::DrawableMock(transparentStateId);
  container.Insert(&opaqueDrawable);
  container.Insert(&transparentDrawable);
  EXPECT_THAT(container.GetDrawables(DrawableContainer::Kind::Opaque),
              testing::ElementsAre(&opaqueDrawable));
  EXPECT_THAT(container.GetDrawables(DrawableContainer::Kind::Transparent),
              testing::ElementsAre(&transparentDrawable));

  EXPECT_TRUE(opaqueDrawable.Container() == &container);
  EXPECT_TRUE(transparentDrawable.Container() == &container);

  container.Remove(&transparentDrawable);
  EXPECT_THAT(container.GetDrawables(DrawableContainer::Kind::Opaque),
              testing::ElementsAre(&opaqueDrawable));
  EXPECT_TRUE(
      container.GetDrawables(DrawableContainer::Kind::Transparent).empty());
  EXPECT_TRUE(opaqueDrawable.Container() == &container);
  EXPECT_TRUE(transparentDrawable.Container() == nullptr);

  container.Remove(&opaqueDrawable);
  EXPECT_TRUE(container.GetDrawables(DrawableContainer::Kind::Opaque).empty());
  EXPECT_TRUE(
      container.GetDrawables(DrawableContainer::Kind::Transparent).empty());
  EXPECT_TRUE(opaqueDrawable.Container() == nullptr);
  EXPECT_TRUE(transparentDrawable.Container() == nullptr);
}

TEST_F(DrawableContainerTest, UpdateStateByEvent) {
  auto container = DrawableContainer();

  auto drawable = video::render::draw::DrawableMock(opaqueStateId);
  container.Insert(&drawable);
  auto stateChangedEvent = event::DrawableEvent::MakeStateChangedEvent(
      &drawable, opaqueStateId.State);

  drawable.State.State.Blend = true;

  container.Handle(stateChangedEvent);

  EXPECT_THAT(container.GetDrawables(DrawableContainer::Kind::Transparent),
              testing::ElementsAre(&drawable));
}

}  // namespace soil::video::render::draw
