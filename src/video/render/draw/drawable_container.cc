#include "video/render/draw/drawable_container.h"
namespace soil::video::render::draw {

void DrawableContainer::Insert(draw::Drawable *drawable) {
  if (drawable->StateId().State.Blend.value_or(false)) {
    transparentDrawables_.push_back(drawable);
  } else {
    opaqueDrawables_.push_back(drawable);
  }
  drawable->container_ = this;
  drawable->AddListener(this);
}

bool DrawableContainer::Remove(draw::Drawable *drawable) {
  if (drawable->Container() != this) {
    return false;
  }
  drawable->RemoveListener(this);
  if (drawable->StateId().State.Blend.value_or(false)) {
    if (RemoveFrom(transparentDrawables_, drawable)) {
      return true;
    }
    return RemoveFrom(opaqueDrawables_, drawable);
  }
  if (RemoveFrom(opaqueDrawables_, drawable)) {
    return true;
  }
  return RemoveFrom(transparentDrawables_, drawable);
}

const std::vector<draw::Drawable *> &DrawableContainer::GetDrawables(
    Kind kind) {
  switch (kind) {
    case Kind::Opaque:
      return opaqueDrawables_;
    case Kind::Transparent:
      return transparentDrawables_;
  }
}

void DrawableContainer::Sort(DrawableContainer::Kind kind,
                             DrawableContainer::SortDirection direction) {
  switch (kind) {
    case Kind::Opaque:
      Sort(opaqueDrawables_, direction);
      break;
    case Kind::Transparent:
      Sort(transparentDrawables_, direction);
      break;
  }
}

void DrawableContainer::Sort(std::vector<draw::Drawable *> &drawables,
                             SortDirection direction) {
  std::vector<SortableDrawable> sortables;
  std::vector<draw::Drawable *> unsortables;
  sortables.reserve(drawables.size());

  for (auto *drawable : drawables) {
    if (!drawable->IsSortable()) {
      unsortables.push_back(drawable);
      continue;
    }
    sortables.push_back({
        .Distance = drawable->DistanceTo(glm::vec3(0)),
        .Drawable = drawable,
    });
  }
  std::sort(sortables.begin(), sortables.end(),
            [direction](SortableDrawable &a, SortableDrawable &b) {
              if (direction == SortDirection::NearToFar) {
                return a.Distance < b.Distance;
              }
              return a.Distance > b.Distance;
            });

  for (auto i = 0; i < sortables.size(); i++) {
    drawables[i] = sortables[i].Drawable;
  }
  auto offset = drawables.size() - unsortables.size();
  for (auto i = 0; i < unsortables.size(); i++) {
    drawables[offset + i] = unsortables[i];
  }
}

bool DrawableContainer::RemoveFrom(std::vector<draw::Drawable *> &drawables,
                                   draw::Drawable *drawable) {
  for (auto itr = drawables.begin(); itr != drawables.end(); ++itr) {
    if (*itr == drawable) {
      drawables.erase(itr);
      drawable->container_ = nullptr;
      return true;
    }
  }
  return false;
}

void DrawableContainer::Handle(const event::DrawableEvent &event) {
  if (event.Cause == event::DrawableEvent::StateChanged) {
    if (event.Drawable->StateId().State.Blend.value_or(false) !=
        event.PrevState.Blend.value_or(false)) {
      if (event.PrevState.Blend.value_or(false)) {
        if (!RemoveFrom(transparentDrawables_, event.Drawable)) {
          throw std::runtime_error("failed to removed changed drawable");
        }
        opaqueDrawables_.push_back(event.Drawable);
      } else {
        if (!RemoveFrom(opaqueDrawables_, event.Drawable)) {
          throw std::runtime_error("failed to removed changed drawable");
        }
        transparentDrawables_.push_back(event.Drawable);
      }
    }
  }
}

}  // namespace soil::video::render::draw
