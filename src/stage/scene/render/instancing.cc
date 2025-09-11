#include "stage/scene/render/instancing.h"

#include <ranges>
#include <stdexcept>

#include "stage/scene/component/instance_data.h"
#include "stage/scene/hook/hook.h"

namespace soil::stage::scene::render {
    Instancing::Instancing(video::render::Container* renderContainer) :
        RenderHook(HandlerType::Component), renderContainer_(renderContainer) {}

    Instancing::~Instancing() {
        for (const auto* va : vertexArrays_) {
            delete va;
        }
        for (const auto& states : renderBatchPerKey_ | std::views::values) {
            delete states.Opaque;
            delete states.NonOpaque;
        }
    }

    void Instancing::Handle(const event::Component& event) {
        if (!component::VisualComponent::IsRenderType(event.GetOrigin(), render::Type::Instancing)) {
            return;
        }
        auto* data = dynamic_cast<component::InstanceData*>(event.GetOrigin());
        switch (event.GetChangeType()) {
        case event::Component::ChangeType::Added:
            OnAdded(data);
            break;
        case event::Component::ChangeType::Removed:
            OnRemoved(data);
            break;
        case event::Component::ChangeType::State:
            OnStateChanged(data);
            break;
        }
    }

    void Instancing::OnAdded(component::InstanceData* data) {
        if (!data->IsVisible() || data->IsCulled()) {
            return;
        }
        const auto key = data->GetBatchKey();
        if (!renderBatchPerKey_.contains(key)) {
            renderBatchPerKey_[key] = StateBatches();
        }
        auto& states = renderBatchPerKey_[key];
        if (data->IsOpaque()) {
            if (states.Opaque == nullptr) {
                states.Opaque = createBatch(data->GetBatchKey());
                addedOpaqueBatches_.push_back(states.Opaque);
            }
            states.Opaque->AddNewInstance(data);
        } else {
            if (states.NonOpaque == nullptr) {
                states.NonOpaque = createBatch(data->GetBatchKey());
                addedNonOpaqueBatches_.push_back(states.NonOpaque);
            }
            states.NonOpaque->AddNewInstance(data);
        }
    }

    void Instancing::OnStateChanged(component::InstanceData* data) {
        if (data->IsDirty()) {
            return;
        }
        if (!data->IsVisible() || data->IsCulled()) {
            OnRemoved(data);
            return;
        }
        const auto key = data->GetBatchKey();
        if (!renderBatchPerKey_.contains(key)) {
            OnAdded(data);
            return;
        }
        const auto& states = renderBatchPerKey_[key];
        if (data->IsOpaque()) {
            if (states.NonOpaque != nullptr) {
                if (states.NonOpaque->RemoveInstance(data) == true) {
                    data->SetIndex(-1);
                    OnAdded(data);
                    return;
                }
            }
            if (states.Opaque == nullptr) {
                OnAdded(data);
                return;
            }
            if (data->GetIndex() != -1) {
                states.Opaque->AddChangedInstance(data);
            } else {
                states.Opaque->AddNewInstance(data);
            }
        } else {
            if (states.Opaque != nullptr) {
                if (states.Opaque->RemoveInstance(data) == true) {
                    data->SetIndex(-1);
                    OnAdded(data);
                    return;
                }
            }
            if (states.NonOpaque == nullptr) {
                OnAdded(data);
                return;
            }
            if (data->GetIndex() != -1) {
                states.NonOpaque->AddChangedInstance(data);
            } else {
                states.NonOpaque->AddNewInstance(data);
            }
        }
        // TODO remove batch if empty
    }

    void Instancing::OnRemoved(component::InstanceData* data) {
        if (data->GetIndex() == -1) {
            return;
        }
        const auto key = data->GetBatchKey();
        if (!renderBatchPerKey_.contains(key)) {
            throw std::runtime_error("Batch with key '" + key + "' unknown");
        }
        const auto& states = renderBatchPerKey_[key];
        auto removed = false;
        if (data->IsOpaque()) {
            if (states.Opaque != nullptr) {
                removed = states.Opaque->RemoveInstance(data);
            }
        } else {
            if (states.NonOpaque != nullptr) {
                removed = states.NonOpaque->RemoveInstance(data);
            }
        }
        if (!removed) {
            throw std::runtime_error("Failed to remove instance with key: " + key);
        }
        // TODO remove batch if empty
    }

    void Instancing::OnRender(video::render::State& state) {
        for (const auto& states : renderBatchPerKey_ | std::views::values) {
            if (states.Opaque != nullptr) {
                states.Opaque->Update({});
            }
            if (states.NonOpaque != nullptr) {
                states.NonOpaque->Update({});
            }
        }
        for (auto* batch : addedOpaqueBatches_) {
            renderContainer_->Add(batch, video::render::Container::OPAQUE);
        }
        addedOpaqueBatches_.clear();

        for (auto* batch : addedNonOpaqueBatches_) {
            renderContainer_->Add(batch, video::render::Container::NON_OPAQUE);
        }
        addedNonOpaqueBatches_.clear();

        for (auto* batch : removedOpaqueBatches_) {
            renderContainer_->Remove(batch, video::render::Container::OPAQUE);
        }
        removedOpaqueBatches_.clear();

        for (auto* batch : removedNonOpaqueBatches_) {
            renderContainer_->Remove(batch, video::render::Container::NON_OPAQUE);
        }
        removedNonOpaqueBatches_.clear();
    }

    void Instancing::AddRenderBatch(const std::string& key, const BatchCreationArgs& args) {
        if (batchCreationArgs_.contains(key)) {
            throw std::runtime_error("Batch with key '" + key + "' already exists");
        }
        batchCreationArgs_[key] = args;
    }

    Instancing::BatchObject* Instancing::createBatch(const std::string& key) {
        if (!batchCreationArgs_.contains(key)) {
            throw std::runtime_error("Batch with key '" + key + "' not exists");
        }
        const auto& args = batchCreationArgs_[key];
        auto* va = new video::render::VertexArray(*args.Mesh);
        vertexArrays_.push_back(va);

        GLsizei instanceSize = 0;
        for (const auto [Elements, Type] : args.VertexAttribDescriptors) {
            instanceSize += video::vertex::AttributePointer::GetSizeOfDataType(Type) * Elements;
        }

        const auto batchDescriptor =
            video::render::BatchDescriptor {.Vao = va->GetVao(),
                                            .Shader = args.Shader,
                                            .InstanceSize = instanceSize,
                                            .PreserveInstances = args.PreserveInstances,
                                            .DrawMode = args.Mesh->GetDrawMode(),
                                            .VertexAttribDescriptors = args.VertexAttribDescriptors,
                                            .State = args.State};
        return new BatchObject(batchDescriptor);
    }

    Instancing::BatchObject::BatchObject(const video::render::BatchDescriptor& descriptor) : Batch(descriptor) {}

    float Instancing::BatchObject::DistanceTo(const glm::vec3& point) {
        return 0.F;
    }

    void Instancing::BatchObject::Render(video::render::State& state) {
        Batch::Render(state);
    }

    bool Instancing::BatchObject::IsSortable() const {
        return false;
    }


} // namespace soil::stage::scene::render
