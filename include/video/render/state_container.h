#ifndef SOIL_VIDEO_RENDER_STATE_CONTAINER_H
#define SOIL_VIDEO_RENDER_STATE_CONTAINER_H
#include <unordered_map>
#include <vector>

#include "renderable.h"
#include "renderable_group.h"
#include "state_identifier.hpp"
namespace soil::video::render {

class StateContainer {
 public:
  explicit StateContainer(const std::function<bool(const StateIdentifier& a,
                                                   const StateIdentifier& b)>&
                              stateCompFunc = std::less());
  ~StateContainer();
  virtual void Insert(Renderable* renderable);
  virtual bool Remove(Renderable* renderable);
  const std::vector<StateIdentifier>& States();
  RenderableGroup* GroupByState(const StateIdentifier& id) const;

 private:
  void removeState(const StateIdentifier& id);
  std::vector<StateIdentifier> stateList_;
  std::unordered_map<const StateIdentifier, RenderableGroup*,
                     StateIdentifierEquality>
      stateToRenderableGroupMap_;

  std::function<bool(const StateIdentifier& a, const StateIdentifier& b)>
      stateCompFunc_;
};
}  // namespace soil::video::render
#endif
