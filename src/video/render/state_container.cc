#include "video/render/state_container.h"
namespace soil::video::render {
StateContainer::StateContainer(
    const std::function<bool(const StateIdentifier &, const StateIdentifier &)>
        &stateCompFunc)
    : stateCompFunc_(stateCompFunc) {}

StateContainer::~StateContainer() {
  for (auto pair : stateToRenderableGroupMap_) {
    delete pair.second;
  }
}

void StateContainer::Insert(soil::video::render::Renderable *renderable) {
  auto &id = renderable->StateId();
  auto itr = stateToRenderableGroupMap_.find(id);
  if (itr == stateToRenderableGroupMap_.end()) {
    auto g = renderable->NewGroup();
    g->Insert(renderable);
    stateToRenderableGroupMap_.insert({id, g});
    stateList_.push_back(id);
    std::sort(stateList_.begin(), stateList_.end(), stateCompFunc_);
  } else {
    itr->second->Insert(renderable);
  }
}

bool StateContainer::Remove(soil::video::render::Renderable *renderable) {
  auto &id = renderable->StateId();
  auto itr = stateToRenderableGroupMap_.find(id);
  if (itr != stateToRenderableGroupMap_.end()) {
    if (itr->second->Remove(renderable)) {
      if (itr->second->Empty()) {
        removeState(id);
      }
      return true;
    }
  }
  for (auto pair : stateToRenderableGroupMap_) {
    if (pair.first == id) {
      continue;
    }
    if (pair.second->Remove(renderable)) {
      if (pair.second->Empty()) {
        removeState(pair.first);
      }
      return true;
    }
  }
  return false;
}

RenderableGroup *StateContainer::GroupByState(const StateIdentifier &id) const {
  auto itr = stateToRenderableGroupMap_.find(id);
  if (itr == stateToRenderableGroupMap_.end()) {
    return nullptr;
  }
  return itr->second;
}

const std::vector<StateIdentifier> &StateContainer::States() {
  return stateList_;
}

void StateContainer::removeState(const StateIdentifier &id) {
  auto itr = stateToRenderableGroupMap_.find(id);
  if (itr != stateToRenderableGroupMap_.end()) {
    stateToRenderableGroupMap_.erase(itr);
  }
  for (auto stateItr = stateList_.begin(); stateItr != stateList_.end();
       ++stateItr) {
    if (*stateItr == id) {
      stateList_.erase(stateItr);
      return;
    }
  }
}

}  // namespace soil::video::render
