#include "video/render/step/sort.h"

#include <algorithm>

namespace soil::video::render::step {

Sort::Sort(const std::string& id, const Order order, Base* requiredStep)
    : Base(id, Type::Prepare, requiredStep), order_(order) {}

void Sort::Process(Context& context) {
  if (context.Renderables == nullptr || context.Renderables->empty()) {
    return;
  }
  switch (order_) {
    case Order::BackToFront:
      sortBackToFront(context.ReferencePoint, *context.Renderables);
      break;
    case Order::FrontToBack:
      sortFrontToBack(context.ReferencePoint, *context.Renderables);
      break;
  }
}

Sort::Order Sort::GetOrder() const { return order_; }

void Sort::sortFrontToBack(const glm::vec3& refPoint,
                           std::vector<RenderableObject*>& renderables) {
  std::ranges::sort(
      renderables,
      std::function<bool(RenderableObject * a, RenderableObject * b)>(
          [refPoint](RenderableObject* a, RenderableObject* b) {
            if (a->IsSortable() && b->IsSortable()) {
              const auto distA = a->DistanceTo(refPoint);
              const auto distB = b->DistanceTo(refPoint);
              return distA < distB;
            }
            if (!a->IsSortable() && !b->IsSortable()) {
              return a->GetContainerStateIndex() < b->GetContainerStateIndex();
            }
            if (!a->IsSortable()) {
              return true;
            }
            return false;
          }));
}

void Sort::sortBackToFront(const glm::vec3& refPoint,
                           std::vector<RenderableObject*>& renderables) {
  std::ranges::sort(
      renderables,
      std::function<bool(RenderableObject * a, RenderableObject * b)>(
          [refPoint](RenderableObject* a, RenderableObject* b) {
            if (a->IsSortable() && b->IsSortable()) {
              const auto distA = a->DistanceTo(refPoint);
              const auto distB = b->DistanceTo(refPoint);
              return distA > distB;
            }
            if (!a->IsSortable() && !b->IsSortable()) {
              return a->GetContainerStateIndex() < b->GetContainerStateIndex();
            }
            if (!a->IsSortable()) {
              return true;
            }
            return false;
          }));
}
}  // namespace soil::video::render::step
