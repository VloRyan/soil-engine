#include "stage/scene/render/feature.h"
#include "stage/scene/scene.h"

namespace soil::stage::scene::render {
    void Feature::OnAddedToScene(Scene* scene) {
        container_ = scene->GetRenderContainer();
    }

    component::VisualComponent* Feature::CastVisualComponent(component::Component* component) {
        if (component == nullptr || component->GetType() != component::Component::Type::Visual) {
            return nullptr;
        }
        return dynamic_cast<component::VisualComponent*>(component);
    }

    bool Feature::IsVisualComponentOf(component::Component* component, const render::Type renderType) {
        if (const auto* vComp = CastVisualComponent(component);
            vComp == nullptr || vComp->GetRenderType() != renderType) {
            return false;
        }
        return true;
    }

} // namespace soil::stage::scene::render
